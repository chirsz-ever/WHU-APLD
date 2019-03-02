SRC=./src
OBJ=./obj
BIN=./bin
CXX=g++
CC_FLAG=-std=c++11 
LIB= 
EXE_SUFFIX=.exe

BASE_LIB=$(OBJ)/random_wander.o
WANDER1=wander_sample1
WANDER2=wander_sample2
WANDER1_EXE=$(WANDER1)$(EXE_SUFFIX)
WANDER2_EXE=$(WANDER2)$(EXE_SUFFIX)

all:$(BIN)/$(WANDER1_EXE) $(BIN)/$(WANDER2_EXE)

$(BIN)/$(WANDER1_EXE):$(OBJ)/$(WANDER1).o $(BASE_LIB)
	@if [ ! -d $(BIN) ]; then mkdir -p $(BIN); fi;
	$(CXX) $^ -o $@ $(LIB)

$(BIN)/$(WANDER2_EXE):$(OBJ)/$(WANDER2).o $(BASE_LIB)
	@if [ ! -d $(BIN) ]; then mkdir -p $(BIN); fi;
	$(CXX) $^ -o $@ $(LIB)

$(OBJ)/%.o:${SRC}/%.cpp
	@if [ ! -d $(OBJ) ]; then mkdir -p $(OBJ); fi;
	$(CXX) $(CC_FLAG) -o $@ -c $<

.PRONY:clean
clean:
	@echo "Removing linked and compiled files......"
	-rm -rf $(OBJ) $(BIN)
