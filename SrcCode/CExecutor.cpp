/*
	Source code of ActOfRose global script execution system. Viktoriia Pashchenko. 2025.
	
	This file is part of system source code.
	
	@Name:			CExecutor.cpp
	@Created:		20.03.2025
	@Programmer:	Viktoriia Pashchenko (SweetyDarkyRosy)
	
	Implementations. */

#include "CExecutor.h"

#include "ReturnCodes.h"
#include "Log.h"
#include "SystemAPI.h"
#include "Keywords.h"
#include "CVariable.h"
#include "CFunction.h"
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

		switch ((*tokenGroup)[0].type)
		{
			case ActOfRose::Token::ETokenType::ETTKeyword:
			{
				ActOfRose::Keyword::EKeywords keyword;
				if (GetKeyword(&keyword, &((*tokenGroup)[_mCurrTokenIndex].value)) == false)
				{
					ActOfRose::WriteLog(PREF_STRING("Keyword not found. Internal error"),
						(sizeof(PREF_STRING("Keyword not found. Internal error")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

					return AOR_ERROR_INTERNAL_ERROR;
				}

				switch (keyword)
				{
					case ActOfRose::Keyword::EKeywords::EK_Override:
					case ActOfRose::Keyword::EKeywords::EK_Var:
					case ActOfRose::Keyword::EKeywords::EK_Strict:
					{
						execResult = DeclareAndInitialiseVariable(tokenGroup);
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_Func:
					{
						execResult = DeclareAndDefineFunction(tokenGroup);
						break;
					}

					case ActOfRose::Keyword::EKeywords::EK_Return:
					{
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

			default:
			{
				ActOfRose::WriteLog(PREF_STRING("Unexpected token"), (sizeof(PREF_STRING("Unexpected token")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_TOKEN_UNEXPECTED_TOKEN;
			}
		}

		if (execResult != AOR_SUCCESS)
		{
			if (execResult == AOR_LEAVE_EXECUTION)
			{
				break;
			}
			else
			{
				return execResult;
			}
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

	return AOR_SUCCESS;
}


// Executes a variable declaration and initialisation
int ActOfRose::CExecutor::DeclareAndInitialiseVariable(std::vector<ActOfRose::Token::SToken>* tokenGroup)
{
	// New value
	ActOfRose::Value::CValue* newValue = nullptr;
	// Flag indicating whether initial value is strictly necessary
	bool isStrict = false;

	{
		ActOfRose::Keyword::EKeywords keyword;
		if (GetKeyword(&keyword, &((*tokenGroup)[_mCurrTokenIndex].value)) == false)
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
				gPreprocessor.ExtractPredefinedValue(&newValue, (*tokenGroup)[_mCurrTokenIndex + 2].value.c_str());
				
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
	const char* variableName = (*tokenGroup)[_mCurrTokenIndex].value.c_str();


	if (AORSystemIsIdentifierUsed(variableName) == true)
	{
		LogAlreadyUsedIdentifier(variableName);

		return AOR_ERROR_EXEC_IDENTIFIER_ALREADY_IN_USE;
	}

	_mCurrTokenIndex++;


	// New variable
	ActOfRose::CVariable* newVariable;

	if (newValue == nullptr)
	{
		if ((*tokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTSemicolon)
		{
			if (isStrict == true)
			{
				ActOfRose::WriteLog(PREF_STRING("Variable is strict. Initial value is expected"),
					(sizeof(PREF_STRING("Variable is strict. Initial value is expected")) / sizeof(PChar)), ActOfRose::ELogLevel::ELL_Error);

				return AOR_ERROR_EXEC_INITIAL_VALUE_REQUIRED;
			}

			newVariable = new ActOfRose::CVariable();
		}
		else if (((*tokenGroup)[_mCurrTokenIndex].type == ActOfRose::Token::ETokenType::ETTOperator) &&
			((*tokenGroup)[_mCurrTokenIndex].value.compare("=") == 0))
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

		_mCurrTokenIndex = (unsigned int)(tokenGroup->size() - 1);
	}

	if (AORSystemRegisterIdentifierAndElement(variableName, ActOfRose::EElementType::EET_Variable, (void*)newVariable) == nullptr)
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
int ActOfRose::CExecutor::DeclareAndDefineFunction(std::vector<ActOfRose::Token::SToken>* tokenGroup)
{
	_mCurrTokenIndex++;

	// Variable name
	const char* functionName = (*tokenGroup)[_mCurrTokenIndex].value.c_str();

	_mCurrTokenIndex++;

	if (AORSystemIsIdentifierUsed(functionName) == true)
	{
		LogAlreadyUsedIdentifier(functionName);

		return AOR_ERROR_EXEC_IDENTIFIER_ALREADY_IN_USE;
	}

	// New function
	ActOfRose::CFunction* newFunction = new ActOfRose::CFunction();


	// ----- Collecting of parameter names -----

	while ((*tokenGroup)[_mCurrTokenIndex].type != ActOfRose::Token::ETokenType::ETTRoundBracketRight)
	{
		_mCurrTokenIndex++;

		newFunction->AddParameter((*tokenGroup)[_mCurrTokenIndex].value.c_str());

	#ifdef _DEBUG
		{
			std::string msg = "Parameter name " + (*tokenGroup)[_mCurrTokenIndex].value + " for function " + functionName + " has been declared";
			ActOfRose::WriteLog(msg.c_str(), msg.length(), ActOfRose::ELogLevel::ELL_Debug);
		}
	#endif

		_mCurrTokenIndex++;
	}

	_mCurrTokenIndex += 2;


	// ----- Copying of function body-related tokens -----

	std::copy(tokenGroup->begin() + _mCurrTokenIndex, tokenGroup->end() - 1, std::back_inserter(*(newFunction->GetBodyTokens())));


	_mCurrTokenIndex = (unsigned int)(tokenGroup->size() - 1);


	if (AORSystemRegisterIdentifierAndElement(functionName, ActOfRose::EElementType::EET_Function, (void*)newFunction) == nullptr)
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
				int result;

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


				if (result == AOR_SUCCESS)
				{
					// ----- Execution -----

					ActOfRose::CFunction* func = (ActOfRose::CFunction*)(element->addr);
					func->Execute(valueRefHolder, &paramArr);
				}
				else
				{
					// ----- Cleanup in case of error before function execution -----

					for (unsigned int paramIt = 0; paramIt < (unsigned int)(paramArr.size()); paramIt++)
					{
						delete paramArr[paramIt];
					}
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
