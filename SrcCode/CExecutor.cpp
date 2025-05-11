/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CExecutor.cpp
	@Created:		20.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CExecutor.h"

#include "ReturnCodes.h"
#include "Global.h"
#include "Log.h"
#include "SystemAPI.h"
#include "Keywords.h"
#include "CVariable.h"
#include "Function.h"
#include "Expression.h"
#include "Operation.h"
#include "CPreProcessor.h"
#include "Utility/StringMisc.h"

#include "Value/Value.h"
#include "Value/CArrayValue.h"


// ----- ActOfRose::CExecutor class -----

// Constructor
ActOfRose::CExecutor::CExecutor()
{}


// Executes an operation based on a token group pointed to by tokenGroup
int ActOfRose::CExecutor::Execute(ActOfRose::Value::SValueReference* returnValueHolder, std::vector<ActOfRose::Token::SToken>* tokenGroup)
{
	_pCurrTokenGroup = tokenGroup;
	_mCurrTokenIndex = 0;

#ifdef _DEBUG
	ActOfRose::WriteLog(PREF_STRING("Execution"), (sizeof(PREF_STRING("Execution")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Debug);
#endif

	while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
	{
		int execResult = AOR_SUCCESS;

		switch ((*_pCurrTokenGroup)[_mCurrTokenIndex].type)
		{
			case ActOfRose::Token::ETokenType::ETTKeyword:
			{
				ActOfRose::Keyword::EKeywords keyword;
				if (GetKeyword(&keyword, &((*_pCurrTokenGroup)[_mCurrTokenIndex].value)) == false)
				{
					ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
						(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INTERNAL_ERROR;
				}

				switch (keyword)
				{
					case ActOfRose::Keyword::EKeywords::EK_True:
					case ActOfRose::Keyword::EKeywords::EK_False:
					{
						execResult = EvaluateExpression(nullptr);
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_Override:
					case ActOfRose::Keyword::EKeywords::EK_Var:
					case ActOfRose::Keyword::EKeywords::EK_Strict:
					case ActOfRose::Keyword::EKeywords::EK_Local:
					{
						execResult = DeclareAndInitialiseVariable();
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_Func:
					{
						execResult = DeclareAndDefineFunction();
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_If:
					{
						execResult = ProcessConditions(returnValueHolder);
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_While:
					{
						execResult = ProcessWhileLoop(returnValueHolder);
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_For:
					{
						execResult = ProcessForLoop(returnValueHolder);
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_Do:
					{
						execResult = ProcessDoWhileLoop(returnValueHolder);
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_Return:
					{
						if (returnValueHolder != nullptr)
						{
							_mCurrTokenIndex++;

							int exprEvalResult = EvaluateExpression(&(returnValueHolder->value.value));
							if (exprEvalResult != AOR_SUCCESS)
							{
								return exprEvalResult;
							}

							returnValueHolder->category = ActOfRose::Value::EValueCategories::EVC_PRValue;
						}

						execResult = AOR_LEAVE_EXECUTION;
						break;
					}

					default:
					{
						ActOfRose::WriteLog(PREF_STRING("Unexpected token"), (sizeof(PREF_STRING("Unexpected token")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

						return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
					}
				}

				break;
			}

			case ActOfRose::Token::ETokenType::ETTNumber:
			case ActOfRose::Token::ETokenType::ETTString:
			case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
			case ActOfRose::Token::ETokenType::ETTIdentifier:
			case ActOfRose::Token::ETokenType::ETTOperator:
			case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
			{
				execResult = EvaluateExpression(nullptr);
				break;
			}

			default:
			{
				ActOfRose::WriteLog(PREF_STRING("Unexpected token"), (sizeof(PREF_STRING("Unexpected token")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}
		}

		if (execResult != AOR_SUCCESS)
		{
			return execResult;
		}

		_mCurrTokenIndex++;
	}

	return AOR_SUCCESS;
}


// Retrieves a value starting in a token with the current index and saved into the value reference pointer to by valueRefHolder
int ActOfRose::CExecutor::RetrieveValue(ActOfRose::Value::SValueReference* valueRefHolder)
{
	switch ((*_pCurrTokenGroup)[_mCurrTokenIndex].type)
	{
		case ActOfRose::Token::ETokenType::ETTNumber:
		case ActOfRose::Token::ETokenType::ETTString:
		case ActOfRose::Token::ETokenType::ETTKeyword:
		{
			ActOfRose::Value::CValue* newValue;
			int createValueResult = AORSystemCreateValueFromToken(&newValue, &((*_pCurrTokenGroup)[_mCurrTokenIndex]));
			if (createValueResult != AOR_SUCCESS)
			{
				return AOR_ERROR_INTERNAL_ERROR;
			}

			valueRefHolder->value.value = newValue;
			valueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_RValue;

			break;
		}

		case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
		{
			ActOfRose::Value::CArrayValue* newArray = new ActOfRose::Value::CArrayValue();
			bool isExprExpected = true;

			_mCurrTokenIndex++;

			while (_mCurrTokenIndex != _pCurrTokenGroup->size())
			{
				if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
				{
					valueRefHolder->value.value = (ActOfRose::Value::CValue*)newArray;
					valueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_RValue;

					return AOR_SUCCESS;
				}

				if (isExprExpected == false)
				{
					if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type != ActOfRose::Token::ETokenType::ETTComma)
					{
						ActOfRose::WriteLog(PREF_STRING("Expected ','"), (sizeof(PREF_STRING("Expected ','")) / sizeof(PChar)),
							ActOfRose::ELogLevel::ELL_Error);

						delete newArray;

						return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
					}

					isExprExpected = true;

					_mCurrTokenIndex++;
				}
				else
				{
					switch ((*_pCurrTokenGroup)[_mCurrTokenIndex].type)
					{
						case ActOfRose::Token::ETokenType::ETTNumber:
						case ActOfRose::Token::ETokenType::ETTString:
						case ActOfRose::Token::ETokenType::ETTCurlyBracketLeft:
						case ActOfRose::Token::ETokenType::ETTIdentifier:
						case ActOfRose::Token::ETokenType::ETTOperator:
						case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
						{
							ActOfRose::Value::CValue* newValue;
							{
								int exprEvalResult = EvaluateExpression(&newValue);
								if (exprEvalResult != AOR_SUCCESS)
								{
									delete newArray;

									return exprEvalResult;
								}
							}

							newArray->AddValue(newValue);

							break;
						}

						case ActOfRose::Token::ETokenType::ETTKeyword:
						{
							ActOfRose::Keyword::EKeywords keyword;
							if (GetKeyword(&keyword, &((*_pCurrTokenGroup)[_mCurrTokenIndex].value)) == false)
							{
								ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
									(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

								return AOR_ERROR_INTERNAL_ERROR;
							}

							switch (keyword)
							{
								case ActOfRose::Keyword::EKeywords::EK_True:
								case ActOfRose::Keyword::EKeywords::EK_False:
								{
									ActOfRose::Value::CValue* newValue;
									{
										int exprEvalResult = EvaluateExpression(&newValue);
										if (exprEvalResult != AOR_SUCCESS)
										{
											delete newArray;

											return exprEvalResult;
										}
									}

									newArray->AddValue(newValue);

									break;
								}

								default:
								{
									std::string errorMsg = "Keyword '" + (*_pCurrTokenGroup)[_mCurrTokenIndex].value + "' is not a value";
									ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.size(), ActOfRose::ELogLevel::ELL_Error);

									return AOR_ERROR_EXEC_NON_VALUE_TOKEN;
								}
							}

							break;
						}

						default:
						{
							ActOfRose::WriteLog(PREF_STRING("Expected expression"), (sizeof(PREF_STRING("Expected expression")) / sizeof(PChar)),
								ActOfRose::ELogLevel::ELL_Error);

							delete newArray;

							return AOR_ERROR_EXEC_EXPRESSION_EXPECTED;
						}
					}

					isExprExpected = false;
				}
			}

			return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT;
		}

		case ActOfRose::Token::ETokenType::ETTIdentifier:
		{
			int idProcResult = ProcessIdentifier(valueRefHolder);
			
			return idProcResult;
		}

		default:
		{
			return AOR_ERROR_EXEC_NON_VALUE_TOKEN;
		}
	}

	return AOR_SUCCESS;
}


/**
	Builds an AST for expression evaluation based on given token array and saves a root of the expression AST to a pointer pointed
	to by treeRootNodeHolder
 */
int ActOfRose::CExecutor::BuildExpressionAST(ActOfRose::AST::CExprASTNode** treeRootNodeHolder)
{
	*treeRootNodeHolder = nullptr;

	ActOfRose::AST::CExprASTOperatorNode* currPrecedenceLastOperatorNode = nullptr;		// Parent node of a sequence of operands of current precedence
	ActOfRose::AST::CExprASTRoundBracketNode* lastRoundBracketNode = nullptr;			// Pointer to a last round bracket (left) node
	unsigned int depthLevel = 0;														// Current depth level based on how many left round brackets have been encountered

	while (_mCurrTokenIndex != _pCurrTokenGroup->size())
	{
		if (((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTSemicolon) ||
			((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTComma))
		{
			return AOR_SUCCESS;
		}
		else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
		{
			if (depthLevel == 0)
			{
				return AOR_SUCCESS;
			}
		}
		else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTSquareBracketRight)
		{
			if (depthLevel == 0)
			{
				return AOR_SUCCESS;
			}
		}
		else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
		{
			if (*treeRootNodeHolder == nullptr)
			{
				ActOfRose::AST::CExprASTRoundBracketNode* newRoundBracketNode = new ActOfRose::AST::CExprASTRoundBracketNode();

				*treeRootNodeHolder = newRoundBracketNode;
				lastRoundBracketNode = newRoundBracketNode;
			}
			else if ((lastRoundBracketNode != nullptr) && (lastRoundBracketNode->GetChild() == nullptr))
			{
				ActOfRose::AST::CExprASTRoundBracketNode* newRoundBracketNode = new ActOfRose::AST::CExprASTRoundBracketNode();

				lastRoundBracketNode->SetChild(newRoundBracketNode);
				lastRoundBracketNode = newRoundBracketNode;
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid expression. Left bracket cannot be placed after an operand"),
					(sizeof(PREF_STRING("Invalid expression. Left bracket cannot be placed after an operand")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_EXEC_INVALID_EXPRESSION;
			}
		
			depthLevel++;
		}
		else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
		{
			if (depthLevel == 0)
			{
				return AOR_SUCCESS;
			}

			if (*treeRootNodeHolder == lastRoundBracketNode)
			{
			}
			else if (lastRoundBracketNode->GetParent()->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket)
			{
				lastRoundBracketNode = (ActOfRose::AST::CExprASTRoundBracketNode*)(lastRoundBracketNode->GetParent());
			}
			else // if (lastRoundBracketNode->GetParent()->GetType() == ActOfRose::EExprASTNodeType::EESTNTOperator)
			{
				ActOfRose::AST::CExprASTOperatorNode* parentOperator = (ActOfRose::AST::CExprASTOperatorNode*)(lastRoundBracketNode->GetParent());
				{
					ActOfRose::AST::CExprASTOperatorNode* operatorNodeWalker = parentOperator;

					while ((operatorNodeWalker->GetLeftChild() == nullptr) && (operatorNodeWalker != *treeRootNodeHolder) &&
						(operatorNodeWalker->GetParent()->GetType() != ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket))
					{
						operatorNodeWalker = (ActOfRose::AST::CExprASTOperatorNode*)(operatorNodeWalker->GetParent());
					}

					currPrecedenceLastOperatorNode = operatorNodeWalker;
				}


				// ----- Deleting current last last round bracket and moving up through AST for finding another last round bracket if there is one at least -----

				if (depthLevel == 1)
				{
					//delete lastRoundBracketNode;
					lastRoundBracketNode = nullptr;
				}
				else
				{
					ActOfRose::AST::CExprASTNode* nodeWalker = parentOperator;

					while (nodeWalker->GetType() != ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket)
					{
						nodeWalker = nodeWalker->GetParent();
					}

					//delete lastRoundBracketNode;
					lastRoundBracketNode = (ActOfRose::AST::CExprASTRoundBracketNode*)nodeWalker;
				}
			}

			depthLevel--;
		}
		else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTOperator)
		{
			// ----- If an operator is encountered -----

			ActOfRose::Operation::EOperationTypes opType;
			if (GetOperationType(&opType, &((*_pCurrTokenGroup)[_mCurrTokenIndex].value)) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Unsupported operation encountered"),
					(sizeof(PREF_STRING("Unsupported operation encountered")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
			}

			if (*treeRootNodeHolder == nullptr)
			{
				if ((opType != ActOfRose::Operation::EOperationTypes::EO_Summation) && (opType != ActOfRose::Operation::EOperationTypes::EO_Subtraction))
				{
					{
						std::string msg = "Invalid expression. Operator ";
						msg += (*_pCurrTokenGroup)[_mCurrTokenIndex].value;
						msg += " cannot be used as unary operator";

						ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Error);
					}

					return AOR_ERROR_EXEC_INVALID_EXPRESSION;
				}
			}


			// New operator node
			ActOfRose::AST::CExprASTOperatorNode* newOperatorNode = new ActOfRose::AST::CExprASTOperatorNode(opType);

			if (*treeRootNodeHolder == nullptr)
			{
				*treeRootNodeHolder = newOperatorNode;
			}
			else if ((lastRoundBracketNode != nullptr) && (lastRoundBracketNode->GetChild() == nullptr))
			{
				lastRoundBracketNode->SetChild(newOperatorNode);
			}
			else if ((lastRoundBracketNode != nullptr) &&
				((lastRoundBracketNode->GetChild()->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTOperand) ||
					(lastRoundBracketNode->GetChild()->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket)))
			{
				newOperatorNode->SetLeftChild(lastRoundBracketNode->GetChild());
				lastRoundBracketNode->SetChild(newOperatorNode);
			}
			else if ((*treeRootNodeHolder)->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTOperand)
			{
				newOperatorNode->SetLeftChild(*treeRootNodeHolder);
				*treeRootNodeHolder = newOperatorNode;
			}
			else
			{
				if (newOperatorNode->GetPrecedence() < currPrecedenceLastOperatorNode->GetPrecedence())
				{
					newOperatorNode->SetLeftChild(currPrecedenceLastOperatorNode->GetRightChild());
					currPrecedenceLastOperatorNode->SetRightChild(newOperatorNode);
				}
				else if (newOperatorNode->GetPrecedence() == currPrecedenceLastOperatorNode->GetPrecedence())
				{
					if (*treeRootNodeHolder == currPrecedenceLastOperatorNode)
					{
						*treeRootNodeHolder = newOperatorNode;
					}
					else if (currPrecedenceLastOperatorNode->GetParent()->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket)
					{
						ActOfRose::AST::CExprASTRoundBracketNode* parentLeftRoundBracket = (ActOfRose::AST::CExprASTRoundBracketNode*)(currPrecedenceLastOperatorNode->GetParent());

						newOperatorNode->SetLeftChild(parentLeftRoundBracket->GetChild());
						parentLeftRoundBracket->SetChild(newOperatorNode);
					}
					else
					{
						ActOfRose::AST::CExprASTOperatorNode* higherParent = (ActOfRose::AST::CExprASTOperatorNode*)(currPrecedenceLastOperatorNode->GetParent());

						if (higherParent->GetLeftChild() == currPrecedenceLastOperatorNode)
						{
							higherParent->SetLeftChild(newOperatorNode);
						}
						else if (higherParent->GetRightChild() == currPrecedenceLastOperatorNode)
						{
							higherParent->SetRightChild(newOperatorNode);
						}
					}

					newOperatorNode->SetLeftChild(currPrecedenceLastOperatorNode);
				}
				else	// if (newOperatorNode->GetPrecedence() > currPrecedenceLastOperatorNode->GetPrecedence())
				{
					ActOfRose::AST::CExprASTOperatorNode* operatorNodeWalker = currPrecedenceLastOperatorNode;

					while ((operatorNodeWalker->GetParent() != nullptr) &&
						(((ActOfRose::AST::CExprASTOperatorNode*)(operatorNodeWalker->GetParent()))->GetType() != ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket) &&
						(newOperatorNode->GetPrecedence() >= ((ActOfRose::AST::CExprASTOperatorNode*)(operatorNodeWalker->GetParent()))->GetPrecedence()))
					{
						operatorNodeWalker = (ActOfRose::AST::CExprASTOperatorNode*)(operatorNodeWalker->GetParent());
					}

					if (operatorNodeWalker == *treeRootNodeHolder)
					{
						newOperatorNode->SetLeftChild(*treeRootNodeHolder);
						newOperatorNode->SetParent(nullptr);

						*treeRootNodeHolder = newOperatorNode;
					}
					else if (operatorNodeWalker->GetParent()->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket)
					{
						ActOfRose::AST::CExprASTRoundBracketNode* parentLeftRoundBracket = (ActOfRose::AST::CExprASTRoundBracketNode*)(operatorNodeWalker->GetParent());

						newOperatorNode->SetLeftChild(parentLeftRoundBracket->GetChild());
						parentLeftRoundBracket->SetChild(newOperatorNode);
					}
					else
					{
						ActOfRose::AST::CExprASTOperatorNode* higherParent = (ActOfRose::AST::CExprASTOperatorNode*)(operatorNodeWalker->GetParent());

						if (higherParent->GetLeftChild() == operatorNodeWalker)
						{
							higherParent->SetLeftChild(newOperatorNode);
						}
						else if (higherParent->GetRightChild() == operatorNodeWalker)
						{
							higherParent->SetRightChild(newOperatorNode);
						}
						
						newOperatorNode->SetLeftChild(operatorNodeWalker);
					}
				}
			}

			currPrecedenceLastOperatorNode = newOperatorNode;


			// ----- Retrieving of other operators in a sequence -----

			// Pointer to last operator node created
			ActOfRose::AST::CExprASTOperatorNode* lastOperatorNode = newOperatorNode;

			_mCurrTokenIndex++;

			while (((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTOperator) &&
				(_mCurrTokenIndex != _pCurrTokenGroup->size()))
			{
				if (GetOperationType(&opType, &((*_pCurrTokenGroup)[_mCurrTokenIndex].value)) == false)
				{
					ActOfRose::WriteLog(PREF_STRING("Unsupported operation encountered"),
						(sizeof(PREF_STRING("Unsupported operation encountered")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}

				if ((opType != ActOfRose::Operation::EOperationTypes::EO_Summation) && (opType != ActOfRose::Operation::EOperationTypes::EO_Subtraction))
				{
					{
						std::string msg = "Invalid expression. Operator ";
						msg += (*_pCurrTokenGroup)[_mCurrTokenIndex].value;
						msg += " cannot be used as unary operator";

						ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Error);
					}

					return AOR_ERROR_EXEC_INVALID_EXPRESSION;
				}

				newOperatorNode = new ActOfRose::AST::CExprASTOperatorNode(opType);

				lastOperatorNode->SetRightChild(newOperatorNode);
				lastOperatorNode = newOperatorNode;

				_mCurrTokenIndex++;
			}

			if (_mCurrTokenIndex == _pCurrTokenGroup->size())
			{
				break;
			}
			else if (((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTNumber) ||
				((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTString) ||
				((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTKeyword) ||
				((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft) ||
				((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTIdentifier))
			{
				ActOfRose::AST::CExprASTOperandNode* newOperandNode;
				{
					// Reference to a new value
					ActOfRose::Value::SValueReference valueRef;

					int valueRetrievingResult = RetrieveValue(&valueRef);
					if (valueRetrievingResult != AOR_SUCCESS)
					{
						return valueRetrievingResult;
					}

					// New operand node
					newOperandNode = new ActOfRose::AST::CExprASTOperandNode(&valueRef);
				}

				lastOperatorNode->SetRightChild(newOperandNode);
			}
			else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				ActOfRose::AST::CExprASTRoundBracketNode* newRoundBracketNode = new ActOfRose::AST::CExprASTRoundBracketNode();

				lastRoundBracketNode = newRoundBracketNode;
				depthLevel++;

				lastOperatorNode->SetRightChild(newRoundBracketNode);
			}
			else
			{
				ActOfRose::WriteLog(PREF_STRING("Invalid expression. Operand or left round bracket should be set after operator"),
					(sizeof(PREF_STRING("Invalid expression. Operand or left round bracket should be set after operator")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_EXEC_INVALID_EXPRESSION;
			}
		}
		else if (((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTNumber) ||
			((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTString) ||
			((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTKeyword) ||
			((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft) ||
			((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTIdentifier))
		{
			// ----- If an operand is encountered -----

			ActOfRose::AST::CExprASTOperandNode* newOperandNode;
			{
				// Reference to a new value
				ActOfRose::Value::SValueReference valueRef;

				int valueRetrievingResult = RetrieveValue(&valueRef);
				if (valueRetrievingResult != AOR_SUCCESS)
				{
					return valueRetrievingResult;
				}

				// New operand node
				newOperandNode = new ActOfRose::AST::CExprASTOperandNode(&valueRef);
			}

			if (*treeRootNodeHolder == nullptr)
			{
				*treeRootNodeHolder = newOperandNode;
			}
			else if (lastRoundBracketNode != nullptr)
			{
				if (lastRoundBracketNode->GetChild() == nullptr)
				{
					lastRoundBracketNode->SetChild(newOperandNode);
				}
				else
				{
					if (lastRoundBracketNode->GetChild()->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTOperand)
					{
						ActOfRose::WriteLog(PREF_STRING("Invalid expression. Operand after another operand detected"),
							(sizeof(PREF_STRING("Invalid expression. Operand after another operand detected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
					}
					else if (lastRoundBracketNode->GetChild()->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket)
					{
						ActOfRose::WriteLog(PREF_STRING("Invalid expression. Operand after left round bracket detected"),
							(sizeof(PREF_STRING("Invalid expression. Operand after left round bracket detected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
					}

					delete newOperandNode;

					return AOR_ERROR_EXEC_INVALID_EXPRESSION;
				}
			}
			else	// if ((*treeRootNodeHolder)->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTOperand) || ((*treeRootNodeHolder)->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket))
			{
				if ((*treeRootNodeHolder)->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTOperand)
				{
					ActOfRose::WriteLog(PREF_STRING("Invalid expression. Operand after another operand detected"),
					(sizeof(PREF_STRING("Invalid expression. Operand after another operand detected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
				}
				else if ((*treeRootNodeHolder)->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTRoundBracket)
				{
					ActOfRose::WriteLog(PREF_STRING("Invalid expression. Operand after left round bracket detected"),
							(sizeof(PREF_STRING("Invalid expression. Operand after left round bracket detected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
				}
				else // if ((*treeRootNodeHolder)->GetType() == ActOfRose::AST::EExprASTNodeType::EESTNTOperator)
				{
					ActOfRose::WriteLog(PREF_STRING("Internal error. Operand after operator in another routine found"),
							(sizeof(PREF_STRING("Internal error. Operand after operator in another routine found")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
				}

				delete newOperandNode;

				return AOR_ERROR_EXEC_INVALID_EXPRESSION;
			}
		}

		_mCurrTokenIndex++;
	}

	return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT;
}

// Evaluates an encountered expression
int ActOfRose::CExecutor::EvaluateExpression(ActOfRose::Value::CValue** valueHolder)
{
	ActOfRose::AST::CExprASTNode* exprRoot;
	{
		int result = BuildExpressionAST(&exprRoot);
		if (result != AOR_SUCCESS)
		{
			delete exprRoot;

			return result;
		}
	}

	ActOfRose::Value::SValueReference newValueRef;
	int result = exprRoot->RetrieveValue(&newValueRef);
	if (result != AOR_SUCCESS)
	{
		delete exprRoot;

		return result;
	}

	if (valueHolder != nullptr)
	{
		ActOfRose::Value::CValue* newValue;
		if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
		{
			newValue = AORSystemCopyValue(*(newValueRef.value.valueHolder));
		}
		else if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_RValue)
		{
			newValue = AORSystemCopyValue(newValueRef.value.value);
		}
		else if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
		{
			newValue = newValueRef.value.value;
		}
		else
		{
			newValue = nullptr;
		}

		delete exprRoot;

	#ifdef _DEBUG
		if (newValue != nullptr)
		{
			std::string msg = "Value ";
			msg += newValue->ConvertValueToByteString();
			msg += " has been retrieved while expression evaluation";

			ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif

		*valueHolder = newValue;
	}
	else
	{
		if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
		{
			delete newValueRef.value.value;
		}

		delete exprRoot;
	}

	return AOR_SUCCESS;
}


// Executes a variable declaration and initialisation
int ActOfRose::CExecutor::DeclareAndInitialiseVariable()
{
	// New value
	ActOfRose::Value::CValue* newValue = nullptr;
	// Flag indicating whether initial value is strictly necessary
	bool isStrict = false;
	// Flag indicating whether a new variable should be local (created for current scope)
	bool isLocal = false;

	{
		ActOfRose::Keyword::EKeywords keyword;
		if (GetKeyword(&keyword, &((*_pCurrTokenGroup)[_mCurrTokenIndex].value)) == false)
		{
			ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
				(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			return AOR_ERROR_INTERNAL_ERROR;
		}

		switch (keyword)
		{
			case ActOfRose::Keyword::EKeywords::EK_Strict:
			{
				isStrict = true;
			}

			case ActOfRose::Keyword::EKeywords::EK_Override:
			{
				gPreprocessor.ExtractPredefinedValue(&newValue, (*_pCurrTokenGroup)[_mCurrTokenIndex + 2].value.c_str());
				
				_mCurrTokenIndex += 2;
				break;
			}

			case ActOfRose::Keyword::EKeywords::EK_Local:
			{
				isLocal = true;
				
				_mCurrTokenIndex += 2;
				break;
			}

			default:
			{
				_mCurrTokenIndex++;
				break;
			}
		}
	}


	// Variable name
	const char* variableName = (*_pCurrTokenGroup)[_mCurrTokenIndex].value.c_str();
	{
		bool isUsed;
		if (isLocal == false)
		{
			isUsed = AORSystemIsIdentifierUsedGlobally(variableName);
		}
		else
		{
			isUsed = AORSystemIsIdentifierUsedInScope(variableName);
		}

		if (isUsed == true)
		{
			LogAlreadyUsedIdentifier(variableName);

			return AOR_ERROR_EXEC_IDENTIFIER_ALREADY_IN_USE;
		}
	}

	_mCurrTokenIndex++;


	// New variable
	ActOfRose::CVariable* newVariable;

	if (newValue == nullptr)
	{
		if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTSemicolon)
		{
			if (isStrict == true)
			{
				ActOfRose::WriteLog(PREF_STRING("Variable is strict. Initial value is expected"),
					(sizeof(PREF_STRING("Variable is strict. Initial value is expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_EXEC_INITIAL_VALUE_REQUIRED;
			}

			newVariable = new ActOfRose::CVariable();
		}
		else if (((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTOperator) &&
			((*_pCurrTokenGroup)[_mCurrTokenIndex].value.compare("=") == 0))
		{
			_mCurrTokenIndex++;

			{
				int exprEvalResult = EvaluateExpression(&newValue);
				if (exprEvalResult != AOR_SUCCESS)
				{
					return exprEvalResult;
				}
			}

			newVariable = new ActOfRose::CVariable(newValue);
		}
	}
	else
	{
		newVariable = new ActOfRose::CVariable(newValue);

		_mCurrTokenIndex = (unsigned int)(_pCurrTokenGroup->size() - 1);
	}


	ActOfRose::SElement* newElementPtr;
	if (isLocal == false)
	{
		newElementPtr = AORSystemRegisterGlobalIdentifierAndElement(variableName, ActOfRose::EElementType::EET_Variable, (void*)newVariable);
	}
	else
	{
		newElementPtr = AORSystemRegisterLocalIdentifierAndElement(variableName, ActOfRose::EElementType::EET_Variable, (void*)newVariable);
	}

	if (newElementPtr == nullptr)
	{
		delete newVariable;

		std::string errorMsg = "Runtime error. Failed to declare the ";
		errorMsg += variableName;
		errorMsg += " variable";

		ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_INTERNAL_ERROR;
	}

#ifdef _DEBUG
	{
		std::string msg = "Variable " + std::string(variableName) + " has been created";
		if (newValue != nullptr)
		{
			msg += " with value of " + newValue->ConvertValueToByteString() + " (" + newValue->GetTypeByteString() + ")";
		}

		ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Debug);
	}
#endif

	return AOR_SUCCESS;
}

// Executes a function declaration and definition
int ActOfRose::CExecutor::DeclareAndDefineFunction()
{
	_mCurrTokenIndex++;

	// Variable name
	const char* functionName = (*_pCurrTokenGroup)[_mCurrTokenIndex].value.c_str();

	_mCurrTokenIndex++;

	if (AORSystemIsIdentifierUsedGlobally(functionName) == true)
	{
		LogAlreadyUsedIdentifier(functionName);

		return AOR_ERROR_EXEC_IDENTIFIER_ALREADY_IN_USE;
	}

	// New function
	ActOfRose::CUserFunction* newFunction = new ActOfRose::CUserFunction();


	// ----- Collecting of parameter names -----

	while ((*_pCurrTokenGroup)[_mCurrTokenIndex].type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
	{
		_mCurrTokenIndex++;

		newFunction->AddParameter((*_pCurrTokenGroup)[_mCurrTokenIndex].value.c_str());

	#ifdef _DEBUG
		{
			std::string msg = "Parameter name " + (*_pCurrTokenGroup)[_mCurrTokenIndex].value + " for function " + functionName + " has been declared";
			ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif

		_mCurrTokenIndex++;
	}

	_mCurrTokenIndex += 2;


	// ----- Copying of function body-related tokens -----

	{
		// Another index holder for finding the position of a curly bracket that indicates the end of a body
		unsigned int bodyEndTokenIndex = _mCurrTokenIndex;
		
		unsigned int curlyBracketBlockCount = 0;

		while (bodyEndTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				curlyBracketBlockCount++;
			}
			else if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
			{
				if (curlyBracketBlockCount == 0)
				{
					break;
				}

				curlyBracketBlockCount--;
			}

			bodyEndTokenIndex++;
		}

		std::copy(_pCurrTokenGroup->begin() + _mCurrTokenIndex, _pCurrTokenGroup->begin() + bodyEndTokenIndex, std::back_inserter(*(newFunction->GetBodyTokens())));

		_mCurrTokenIndex = bodyEndTokenIndex;
	}


	if (AORSystemRegisterGlobalIdentifierAndElement(functionName, ActOfRose::EElementType::EET_Function, (void*)newFunction) == nullptr)
	{
		delete newFunction;

		std::string errorMsg = "Runtime error. Failed to declare the ";
		errorMsg += functionName;
		errorMsg += " function";

		ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_INTERNAL_ERROR;
	}

#ifdef _DEBUG
	{
		std::string msg = "Function " + std::string(functionName) + " has been declared";
		ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Debug);
	}
#endif

	return AOR_SUCCESS;
}

// Processes a sequence of conditions and executes a sequence of operations under a satisfying condition
int ActOfRose::CExecutor::ProcessConditions(ActOfRose::Value::SValueReference* returnValueHolder)
{
	bool isExecuted = false;
	int initialTokenIndex = _mCurrTokenIndex;

	while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
	{
		int result = AOR_SUCCESS;

		if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTKeyword)
		{
			ActOfRose::Keyword::EKeywords keyword;
			if (GetKeyword(&keyword, &((*_pCurrTokenGroup)[_mCurrTokenIndex].value)) == false)
			{
				ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
					(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_INTERNAL_ERROR;
			}

			if ((keyword == ActOfRose::Keyword::EKeywords::EK_If) || (keyword == ActOfRose::Keyword::EKeywords::EK_Elif))
			{
				if ((keyword == ActOfRose::Keyword::EKeywords::EK_If) && (initialTokenIndex != _mCurrTokenIndex))
				{
					_mCurrTokenIndex--;
					break;
				}

				_mCurrTokenIndex += 2;

				if (isExecuted == false)
				{
					// ----- Processing of condition -----

					ActOfRose::AST::CExprASTNode* exprRoot;
					{
						result = BuildExpressionAST(&exprRoot);
						if (result != AOR_SUCCESS)
						{
							delete exprRoot;
							return result;
						}
					}

					ActOfRose::Value::SValueReference condExprResult;
					result = exprRoot->RetrieveValue(&condExprResult);
					if (result != AOR_SUCCESS)
					{
						delete exprRoot;
						return result;
					}

					ActOfRose::Value::CValue* exprResultValue;
					if (condExprResult.category == ActOfRose::Value::EValueCategories::EVC_None)
					{
						ActOfRose::WriteLog(PREF_STRING("Null value cannot be a condition"),
							(sizeof(PREF_STRING("Null value cannot be a condition")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

						result = AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
					}
					else
					{
						if (condExprResult.category == ActOfRose::Value::EValueCategories::EVC_LValue)
						{
							exprResultValue = *(condExprResult.value.valueHolder);
						}
						else
						{
							exprResultValue = condExprResult.value.value;
						}

						if (exprResultValue->GetValueType() == ActOfRose::Value::EValueType::EVT_String)
						{
							ActOfRose::WriteLog(PREF_STRING("String cannot be a condition"),
								(sizeof(PREF_STRING("String cannot be a condition")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);
			
							result = AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
						}
						else if (exprResultValue->GetValueType() == ActOfRose::Value::EValueType::EVT_Array)
						{
							ActOfRose::WriteLog(PREF_STRING("Array cannot be a condition"),
								(sizeof(PREF_STRING("Array cannot be a condition")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

							result = AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
						}
					}

					_mCurrTokenIndex += 2;

					if (result == AOR_SUCCESS)
					{
						// Another index holder for finding the position of a curly bracket that indicates the end of a body
						unsigned int bodyEndTokenIndex = _mCurrTokenIndex;


						// ----- Finding the end of a body -----

						{
							unsigned int curlyBracketBlockCount = 0;

							while (bodyEndTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
							{
								if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
								{
									curlyBracketBlockCount++;
								}
								else if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
								{
									if (curlyBracketBlockCount == 0)
									{
										break;
									}

									curlyBracketBlockCount--;
								}

								bodyEndTokenIndex++;
							}
						}


						// ----- Checking the result value -----

						if (exprResultValue->IsZero() == false)
						{
							// ----- Extracting a body -----

							std::vector<ActOfRose::Token::SToken> bodyTokens;
							std::copy(_pCurrTokenGroup->begin() + _mCurrTokenIndex, _pCurrTokenGroup->begin() + bodyEndTokenIndex, std::back_inserter(bodyTokens));


							// ----- Execution -----

							if (bodyTokens.size() != 0)
							{
								// Creates a local scope
								ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes::ESIT_InheritingScope);

								ActOfRose::CExecutor executor;				// Local instance of executor
								result = executor.Execute(returnValueHolder, &bodyTokens);

								// Removes a local scope
								ActOfRose::AORSystemRemoveScope();

								isExecuted = true;
							}
						}

						_mCurrTokenIndex = bodyEndTokenIndex;
					}

					if (condExprResult.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
					{
						delete condExprResult.value.value;
					}

					delete exprRoot;

					if (result != AOR_SUCCESS)
					{
						return result;
					}
				}
				else
				{
					// ----- Skipping -----

					{
						unsigned int roundBracketBlockCount = 0;

						while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
						{
							if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
							{
								roundBracketBlockCount++;
							}
							else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
							{
								if (roundBracketBlockCount == 0)
								{
									break;
								}

								roundBracketBlockCount--;
							}

							_mCurrTokenIndex++;
						}
					}

					_mCurrTokenIndex += 2;

					{
						unsigned int curlyBracketBlockCount = 0;

						while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
						{
							if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
							{
								curlyBracketBlockCount++;
							}
							else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
							{
								if (curlyBracketBlockCount == 0)
								{
									break;
								}

								curlyBracketBlockCount--;
							}

							_mCurrTokenIndex++;
						}
					}
				}
			}
			else if (keyword == ActOfRose::Keyword::EKeywords::EK_Else)
			{
				_mCurrTokenIndex += 2;

				// Another index holder for finding the position of a curly bracket that indicates the end of a body
				unsigned int bodyEndTokenIndex = _mCurrTokenIndex;


				// ----- Finding the end of a body -----

				{
					unsigned int curlyBracketBlockCount = 0;

					while (bodyEndTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
					{
						if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
						{
							curlyBracketBlockCount++;
						}
						else if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
						{
							if (curlyBracketBlockCount == 0)
							{
								break;
							}

							curlyBracketBlockCount--;
						}

						bodyEndTokenIndex++;
					}
				}

				if (isExecuted == false)
				{
					// ----- Extracting a body -----

					std::vector<ActOfRose::Token::SToken> bodyTokens;
					std::copy(_pCurrTokenGroup->begin() + _mCurrTokenIndex, _pCurrTokenGroup->begin() + bodyEndTokenIndex, std::back_inserter(bodyTokens));


					// ----- Execution -----

					if (bodyTokens.size() != 0)
					{
						// Creates a function's local scope
						ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes::ESIT_InheritingScope);

						ActOfRose::CExecutor executor;				// Local instance of executor
						result = executor.Execute(returnValueHolder, &bodyTokens);

						// Removes a function's local scope
						ActOfRose::AORSystemRemoveScope();

						isExecuted = true;
					}
				}

				_mCurrTokenIndex = bodyEndTokenIndex;

				if (result != AOR_SUCCESS)
				{
					return result;
				}
			}
			else
			{
				_mCurrTokenIndex--;
				break;
			}
		}
		else
		{
			_mCurrTokenIndex--;
			break;
		}

		_mCurrTokenIndex++;
	}

	return AOR_SUCCESS;
}

// Processes a 'while' loop and executes its body until a condition is not satisfied
int ActOfRose::CExecutor::ProcessWhileLoop(ActOfRose::Value::SValueReference* returnValueHolder)
{
	int result = AOR_SUCCESS;

	_mCurrTokenIndex += 2;


	// ----- Processing of conditional expression -----

	// Index holding an index of a conditional expression
	unsigned int condExprFirstTokenIndex = _mCurrTokenIndex;
	{
		unsigned int roundBracketBlockCount = 0;

		while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				roundBracketBlockCount++;
			}
			else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				if (roundBracketBlockCount == 0)
				{
					break;
				}

				roundBracketBlockCount--;
			}

			_mCurrTokenIndex++;
		}
	}


	_mCurrTokenIndex += 2;


	// ----- Extracting a body -----

	// Another index holder for finding the position of a curly bracket that indicates the end of a body
	unsigned int bodyEndTokenIndex = _mCurrTokenIndex;

	std::vector<ActOfRose::Token::SToken> bodyTokens;
	{
		// ----- Finding the end of a body -----

		unsigned int curlyBracketBlockCount = 0;

		while (bodyEndTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				curlyBracketBlockCount++;
			}
			else if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
			{
				if (curlyBracketBlockCount == 0)
				{
					break;
				}

				curlyBracketBlockCount--;
			}

			bodyEndTokenIndex++;
		}


		// ----- Copying of body-related tokens -----

		std::copy(_pCurrTokenGroup->begin() + _mCurrTokenIndex, _pCurrTokenGroup->begin() + bodyEndTokenIndex, std::back_inserter(bodyTokens));
	}


	// ----- Execution -----

	while (true)
	{
		// ----- Cehcking the condition -----

		_mCurrTokenIndex = condExprFirstTokenIndex;

		ActOfRose::AST::CExprASTNode* exprRoot;
		{
			result = BuildExpressionAST(&exprRoot);
			if (result != AOR_SUCCESS)
			{
				delete exprRoot;
				return result;
			}
		}

		ActOfRose::Value::SValueReference resultValueRef;
		result = exprRoot->RetrieveValue(&resultValueRef);
		if (result != AOR_SUCCESS)
		{
			delete exprRoot;
			return result;
		}
		

		ActOfRose::Value::CValue* resultValue;
		if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_None)
		{
			delete exprRoot;

			ActOfRose::WriteLog(PREF_STRING("Null value cannot be a condition"),
				(sizeof(PREF_STRING("Null value cannot be a condition")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			result = AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;

			break;
		}
		else if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
		{
			resultValue = *(resultValueRef.value.valueHolder);
		}
		else
		{
			resultValue = resultValueRef.value.value;
		}

		bool isConditionSatisfied = (resultValue->IsZero() == false);

		if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
		{
			delete resultValue;
		}

		delete exprRoot;


		if (isConditionSatisfied == false)
		{
			break;
		}
		else
		{
			// Creates a loop's local scope
			ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes::ESIT_InheritingScope);

			ActOfRose::CExecutor executor;				// Local instance of executor
			result = executor.Execute(returnValueHolder, &bodyTokens);

			// Removes a loop's local scope
			ActOfRose::AORSystemRemoveScope();

			if (result != AOR_SUCCESS)
			{
				break;
			}
		}
	}

	_mCurrTokenIndex = bodyEndTokenIndex;

	return result;
}

// Processes a 'for' loop and executes its body until a condition is not satisfied
int ActOfRose::CExecutor::ProcessForLoop(ActOfRose::Value::SValueReference* returnValueHolder)
{
	// Creates a loop's local scope
	ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes::ESIT_InheritingScope);

	int result = AOR_SUCCESS;

	_mCurrTokenIndex += 2;


	// ----- Processing and evaluation of an initial expression -----

	if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type != ActOfRose::Token::ETokenType::ETTSemicolon)
	{
		result = EvaluateExpression(nullptr);
		if (result != AOR_SUCCESS)
		{
			// Removes a loop's local scope
			ActOfRose::AORSystemRemoveScope();
			return result;
		}
	}

	_mCurrTokenIndex++;


	// ----- Extraction of a conditional expression -----

	// Index holding an index of a conditional expression
	int condExprFirstTokenIndex;
	if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTSemicolon)
	{
		condExprFirstTokenIndex = -1;
	}
	else
	{
		condExprFirstTokenIndex = (int)_mCurrTokenIndex;

		while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTSemicolon)
			{
				break;
			}

			_mCurrTokenIndex++;
		}
	}

	_mCurrTokenIndex++;


	// ----- Extraction of an advancement expression -----
	
	int advancementExprFirstTokenIndex;
	if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
	{
		advancementExprFirstTokenIndex = -1;
	}
	else
	{
		advancementExprFirstTokenIndex = (int)_mCurrTokenIndex;

		unsigned int roundBracketBlockCount = 0;

		while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				roundBracketBlockCount++;
			}
			else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				if (roundBracketBlockCount == 0)
				{
					break;
				}

				roundBracketBlockCount--;
			}

			_mCurrTokenIndex++;
		}
	}

	_mCurrTokenIndex += 2;


	// ----- Extracting a body -----

	// Another index holder for finding the position of a curly bracket that indicates the end of a body
	unsigned int bodyEndTokenIndex = _mCurrTokenIndex;

	std::vector<ActOfRose::Token::SToken> bodyTokens;
	{
		// ----- Finding the end of a body -----

		unsigned int curlyBracketBlockCount = 0;

		while (bodyEndTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				curlyBracketBlockCount++;
			}
			else if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
			{
				if (curlyBracketBlockCount == 0)
				{
					break;
				}

				curlyBracketBlockCount--;
			}

			bodyEndTokenIndex++;
		}


		// ----- Copying of body-related tokens -----

		std::copy(_pCurrTokenGroup->begin() + _mCurrTokenIndex, _pCurrTokenGroup->begin() + bodyEndTokenIndex, std::back_inserter(bodyTokens));
	}


	// ----- Loop -----

	while (true)
	{
		// ----- Cehcking the condition -----

		bool isConditionSatisfied = true;
		if (condExprFirstTokenIndex != (-1))
		{
			_mCurrTokenIndex = (unsigned int)condExprFirstTokenIndex;

			ActOfRose::AST::CExprASTNode* exprRoot;
			{
				result = BuildExpressionAST(&exprRoot);
				if (result != AOR_SUCCESS)
				{
					delete exprRoot;
					return result;
				}
			}

			ActOfRose::Value::SValueReference resultValueRef;
			result = exprRoot->RetrieveValue(&resultValueRef);
			if (result != AOR_SUCCESS)
			{
				delete exprRoot;
				return result;
			}
			

			ActOfRose::Value::CValue* resultValue;
			if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_None)
			{
				delete exprRoot;

				ActOfRose::WriteLog(PREF_STRING("Null value cannot be a condition"),
					(sizeof(PREF_STRING("Null value cannot be a condition")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				result = AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;

				break;
			}
			else if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
			{
				resultValue = *(resultValueRef.value.valueHolder);
			}
			else
			{
				resultValue = resultValueRef.value.value;
			}

			isConditionSatisfied = (resultValue->IsZero() == false);

			if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
			{
				delete resultValue;
			}

			delete exprRoot;
		}

		if (isConditionSatisfied == false)
		{
			break;
		}
		else
		{
			// ----- Execution -----

			// Creates a body's local scope
			ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes::ESIT_InheritingScope);

			ActOfRose::CExecutor executor;				// Local instance of executor
			result = executor.Execute(returnValueHolder, &bodyTokens);

			// Removes a body's local scope
			ActOfRose::AORSystemRemoveScope();

			if (result != AOR_SUCCESS)
			{
				break;
			}


			// ----- Advancement -----

			if (advancementExprFirstTokenIndex != (-1))
			{
				_mCurrTokenIndex = (unsigned int)advancementExprFirstTokenIndex;

				result = EvaluateExpression(nullptr);
				if (result != AOR_SUCCESS)
				{
					break;
				}
			}
		}
	}

	_mCurrTokenIndex = bodyEndTokenIndex;

	// Removes a loop's local scope
	ActOfRose::AORSystemRemoveScope();

	return result;
}

// Processes a 'do-while' loop and executes its body until a condition is not satisfied
int ActOfRose::CExecutor::ProcessDoWhileLoop(ActOfRose::Value::SValueReference* returnValueHolder)
{
	int result = AOR_SUCCESS;

	_mCurrTokenIndex += 2;


	// ----- Extracting a body -----

	std::vector<ActOfRose::Token::SToken> bodyTokens;
	{
		// ----- Finding the end of a body -----

		// Another index holder for finding the position of a curly bracket that indicates the end of a body
		unsigned int bodyEndTokenIndex = _mCurrTokenIndex;

		unsigned int curlyBracketBlockCount = 0;

		while (bodyEndTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketLeft)
			{
				curlyBracketBlockCount++;
			}
			else if ((*_pCurrTokenGroup)[bodyEndTokenIndex].type == ActOfRose::Token::ETokenType::ETTCurlyBracketRight)
			{
				if (curlyBracketBlockCount == 0)
				{
					break;
				}

				curlyBracketBlockCount--;
			}

			bodyEndTokenIndex++;
		}


		// ----- Copying of body-related tokens -----

		std::copy(_pCurrTokenGroup->begin() + _mCurrTokenIndex, _pCurrTokenGroup->begin() + bodyEndTokenIndex, std::back_inserter(bodyTokens));


		_mCurrTokenIndex = bodyEndTokenIndex;
	}


	_mCurrTokenIndex += 3;


	// ----- Processing of conditional expression -----

	// Index holding an index of a conditional expression
	unsigned int condExprFirstTokenIndex = _mCurrTokenIndex;
	{
		unsigned int roundBracketBlockCount = 0;

		while (_mCurrTokenIndex < (unsigned int)(_pCurrTokenGroup->size()))
		{
			if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketLeft)
			{
				roundBracketBlockCount++;
			}
			else if ((*_pCurrTokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTRoundBracketRight)
			{
				if (roundBracketBlockCount == 0)
				{
					break;
				}

				roundBracketBlockCount--;
			}

			_mCurrTokenIndex++;
		}
	}


	// ----- Loop -----

	while (true)
	{
		// ----- Execution -----

		// Creates a loop's local scope
		ActOfRose::AORSystemAddScope(ActOfRose::EScopeVisibilityTypes::ESIT_InheritingScope);

		ActOfRose::CExecutor executor;				// Local instance of executor
		result = executor.Execute(returnValueHolder, &bodyTokens);

		// Removes a loop's local scope
		ActOfRose::AORSystemRemoveScope();

		if (result != AOR_SUCCESS)
		{
			break;
		}


		// ----- Cehcking the condition -----

		_mCurrTokenIndex = condExprFirstTokenIndex;

		ActOfRose::AST::CExprASTNode* exprRoot;
		{
			result = BuildExpressionAST(&exprRoot);
			if (result != AOR_SUCCESS)
			{
				delete exprRoot;
				return result;
			}
		}

		ActOfRose::Value::SValueReference resultValueRef;
		result = exprRoot->RetrieveValue(&resultValueRef);
		if (result != AOR_SUCCESS)
		{
			delete exprRoot;
			return result;
		}
		

		ActOfRose::Value::CValue* resultValue;
		if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_None)
		{
			delete exprRoot;

			ActOfRose::WriteLog(PREF_STRING("Null value cannot be a condition"),
				(sizeof(PREF_STRING("Null value cannot be a condition")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

			result = AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;

			break;
		}
		else if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
		{
			resultValue = *(resultValueRef.value.valueHolder);
		}
		else
		{
			resultValue = resultValueRef.value.value;
		}

		bool isConditionSatisfied = (resultValue->IsZero() == false);

		if (resultValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
		{
			delete resultValue;
		}

		delete exprRoot;


		if (isConditionSatisfied == false)
		{
			break;
		}
	}


	_mCurrTokenIndex++;

	return result;
}

// Processes an encountered identifier and returns a value or a reference to value if possible
int ActOfRose::CExecutor::ProcessIdentifier(ActOfRose::Value::SValueReference* valueRefHolder)
{
	ActOfRose::SElement* element = ActOfRose::AORSystemGetElementByIdentifier((*_pCurrTokenGroup)[_mCurrTokenIndex].value.c_str());
	if (element == nullptr)
	{
		std::string errorMsg = (*_pCurrTokenGroup)[_mCurrTokenIndex].value + " was not declared";
		ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

		return AOR_ERROR_EXEC_UNDECLARED_IDENTIFIER;
	}

	if (_mCurrTokenIndex == (unsigned int)(_pCurrTokenGroup->size() - 1))
	{
		return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT;
	}
	else
	{
		switch ((*_pCurrTokenGroup)[_mCurrTokenIndex + 1].type)
		{
			case ActOfRose::Token::ETokenType::ETTRoundBracketLeft:
			{
				// ----- If a function call is encountered -----

				if (element->type != ActOfRose::EElementType::EET_Function)
				{
					std::string errorMsg = "Invalid operand type. ";
					errorMsg += (*_pCurrTokenGroup)[_mCurrTokenIndex].value;
					errorMsg += " is not function";

					ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}

				_mCurrTokenIndex++;
				if (((unsigned int)(_pCurrTokenGroup->size()) - _mCurrTokenIndex) < 2)
				{
					return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT;
				}

				std::vector<ActOfRose::Value::CValue*> paramArr;		// Array of values as parameters
				int result = AOR_SUCCESS;

				if ((*_pCurrTokenGroup)[_mCurrTokenIndex + 1].type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
				{
					while ((*_pCurrTokenGroup)[_mCurrTokenIndex].type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
					{
						_mCurrTokenIndex++;
						if (_mCurrTokenIndex >= (unsigned int)(_pCurrTokenGroup->size()))
						{
							return AOR_ERROR_TOKEN_PREMATURE_END_OF_SCRIPT;
						}

						ActOfRose::AST::CExprASTNode* exprRoot;
						{
							result = BuildExpressionAST(&exprRoot);
							if (result != AOR_SUCCESS)
							{
								delete exprRoot;
								break;
							}
						}

						ActOfRose::Value::SValueReference newValueRef;
						result = exprRoot->RetrieveValue(&newValueRef);
						if (result != AOR_SUCCESS)
						{
							delete exprRoot;
							break;
						}

						if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_LValue)
						{
							paramArr.push_back(AORSystemCopyValue(*(newValueRef.value.valueHolder)));
						}
						else if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_RValue)
						{
							paramArr.push_back(AORSystemCopyValue(newValueRef.value.value));
						}
						else if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_PRValue)
						{
							paramArr.push_back(newValueRef.value.value);
						}
						else if (newValueRef.category == ActOfRose::Value::EValueCategories::EVC_None)
						{
							paramArr.push_back(nullptr);
						}

						delete exprRoot;
					}
				}
				else
				{
					_mCurrTokenIndex++;
				}


				if (result == AOR_SUCCESS)
				{
					// ----- Execution -----

					ActOfRose::IFunction* func = (ActOfRose::IFunction*)(element->addr);
					result = func->Execute(valueRefHolder, &paramArr);
					if (result != AOR_SUCCESS)
					{
						return result;
					}
				}
				else
				{
					// ----- Cleanup in case of error before function execution -----

					for (unsigned int paramIt = 0; paramIt < (unsigned int)(paramArr.size()); paramIt++)
					{
						delete paramArr[paramIt];
					}

					return result;
				}

				break;
			}

			case ActOfRose::Token::ETokenType::ETTSquareBracketLeft:
			{
				// ----- If an access to an inner element through index is encountered -----

				if ((element->type != ActOfRose::EElementType::EET_Variable) &&
					(element->type != ActOfRose::EElementType::EET_Constant))
				{
					std::string errorMsg = "Invalid operand type. ";
					errorMsg += (*_pCurrTokenGroup)[_mCurrTokenIndex].value;
					errorMsg += " is not variable or constant";

					ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}

				_mCurrTokenIndex += 2;

				ActOfRose::Value::SValueReference indexValueHolder;
				int result;

				result = EvaluateExpression(&(indexValueHolder.value.value));
				if (result != AOR_SUCCESS)
				{
					return result;
				}

				indexValueHolder.category = ActOfRose::Value::EValueCategories::EVC_PRValue;

				if (element->type == ActOfRose::EElementType::EET_Variable)
				{
					ActOfRose::CVariable* var = (ActOfRose::CVariable*)(element->addr);
					ActOfRose::Value::CValue* value = var->GetValue();

					result = value->ExecuteOperation(valueRefHolder, ActOfRose::Operation::EOperationTypes::EO_ElementAccess,
						&indexValueHolder);
				}
				else if (element->type != ActOfRose::EElementType::EET_Constant)
				{
					/*!
						TODO: Implement support of constants later
					*/
				}

				delete indexValueHolder.value.value;

				if (result != AOR_SUCCESS)
				{
					return result;
				}

				break;
			}

			default:
			{
				// ----- If a variable or a constant is encountered -----

				if (element->type == ActOfRose::EElementType::EET_Variable)
				{
					valueRefHolder->value.valueHolder = ((ActOfRose::CVariable*)(element->addr))->GetValueHolder();
					valueRefHolder->category = ActOfRose::Value::EValueCategories::EVC_LValue;
				}
				else if (element->type != ActOfRose::EElementType::EET_Constant)
				{
					/*!
						TODO: Implement support of constants later
					 */
				}
				else
				{
					std::string errorMsg = "Invalid operand type. ";
					errorMsg += (*_pCurrTokenGroup)[_mCurrTokenIndex].value;
					errorMsg += " is not variable or constant";

					ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_EXEC_UNSUPPORTED_OPERATION;
				}

				break;
			}
		}
	}

	return AOR_SUCCESS;
}


// Logs information about that a specified identifier already used
void ActOfRose::CExecutor::LogAlreadyUsedIdentifier(const char* identifier)
{
	std::string errorMsg = "Identifier ";
	errorMsg += identifier;
	errorMsg += " is already in use";

	ActOfRose::WriteLog(errorMsg.c_str(), errorMsg.length(), ActOfRose::ELogLevel::ELL_Error);
}
