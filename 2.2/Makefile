SRC=./src
OBJ=./obj
BIN=./bin
CXX=g++
C_FLAG=-std=c++11 -I./src -Wall
LINK_FLAGS= 
EXE_SUFFIX=.exe

OBJECTS=$(patsubst ${SRC}/%.cpp, ${OBJ}/%.o,$(wildcard ${SRC}/*.cpp))
TARGET=${BIN}/expr_analysis${EXE_SUFFIX}
SUBDIRS=${OBJ} ${BIN}

.PHONY:clean cleanall all

all: ${SUBDIRS} ${TARGET}

${SUBDIRS}:%:
	@if [ ! -d $@ ]; then mkdir -p $@; fi;

${TARGET}: ${OBJECTS}
	${CXX} $^ -o $@ ${LINK_FLAGS}
	@echo "build $@ successfully"

sinclude ${OBJECTS:.o=.d} 

$(OBJ)/%.o:${SRC}/%.cpp 
	$(CXX) $(C_FLAG) -o $@ -c $<

$(OBJ)/%.d:${SRC}/%.cpp 
	@if [ ! -d ${OBJ} ]; then mkdir -p ${OBJ}; fi;
	@echo "Making $@ ..."
	@${CXX}  $< ${C_FLAG} -MM -MT"$(OBJ)/$*.o" -MT"$@" -MF"$@" 

.PRONY:clean cleanall all
clean:
	@echo "Removing binary files......"
	-rm -rf $(OBJ)/*.o $(BIN)/*.*

cleanall:
	@echo "Removing all produced files......"
	-rm -rf $(OBJ) $(BIN) 
