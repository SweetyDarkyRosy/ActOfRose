CXX := g++
CXXFLAGS := -Wall -Wextra -g -std=c++17 -fshort-wchar -fno-rtti
LDFLAGS := -Wall -Wextra -g
INCLUDE := -isystem ./SrcCode


release: CXXFLAGS += -O3 -D NDEBUG -D _NDEBUG -fdata-sections -ffunction-sections
release: LDFLAGS += -O3 -flto -Wl,--gc-sections -Wl,--no-as-needed
release: OBJ_DIR := Bin/Intermediates/Release
release: BIN_DIR := Bin
release: prepare compile
	$(CXX) $(LDFLAGS) $(OBJ_DIR)/*.o -o $(BIN_DIR)/ActOfRose

debug: CXXFLAGS += -O0 -D DEBUG -D _DEBUG
debug: LDFLAGS += -O0
debug: OBJ_DIR := Bin/Intermediates/Debug
debug: BIN_DIR := Bin
debug: prepare compile
	$(CXX) $(LDFLAGS) $(OBJ_DIR)/*.o -o $(BIN_DIR)/ActOfRose

prepare:
	if [ -d "$(OBJ_DIR)" ]; then rm -rf $(OBJ_DIR); fi
	mkdir -p $(OBJ_DIR)

compile:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Utility/StringConverting.cpp -o $(OBJ_DIR)/StringConverting.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Value/CIntegerValue.cpp -o $(OBJ_DIR)/CIntegerValue.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Value/CFloatValue.cpp -o $(OBJ_DIR)/CFloatValue.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Value/CBooleanValue.cpp -o $(OBJ_DIR)/CBooleanValue.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Value/CCharValue.cpp -o $(OBJ_DIR)/CCharValue.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Value/CStringValue.cpp -o $(OBJ_DIR)/CStringValue.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Value/CArrayValue.cpp -o $(OBJ_DIR)/CArrayValue.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Log.cpp -o $(OBJ_DIR)/Log.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/CScript.cpp -o $(OBJ_DIR)/CScript.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Keywords.cpp -o $(OBJ_DIR)/Keywords.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/SystemAPI.cpp -o $(OBJ_DIR)/SystemAPI.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/CLexer.cpp -o $(OBJ_DIR)/CLexer.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Context.cpp -o $(OBJ_DIR)/Context.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/CSequencer.cpp -o $(OBJ_DIR)/CSequencer.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Expression.cpp -o $(OBJ_DIR)/Expression.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/CExecutor.cpp -o $(OBJ_DIR)/CExecutor.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c SrcCode/Main.cpp -o $(OBJ_DIR)/Main.o
