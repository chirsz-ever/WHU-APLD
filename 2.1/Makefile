SRC=./src
OBJ=./obj
BIN=./bin
CXX=g++
C_FLAG=-std=c++11 -I./src -Wall
LINK_FLAGS= 
EXE_SUFFIX=.exe

WANDER1=wander_sample1
WANDER2=wander_sample2
WANDER1_OBJ=${OBJ}/${WANDER1}.o
WANDER2_OBJ=${OBJ}/${WANDER2}.o
WANDER1_BIN=${BIN}/$(WANDER1)$(EXE_SUFFIX)
WANDER2_BIN=${BIN}/$(WANDER2)$(EXE_SUFFIX)

OBJECTS=$(patsubst ${SRC}/%.cpp, ${OBJ}/%.o,$(wildcard ${SRC}/*.cpp))
BASE_OBJS=$(filter-out ${OBJ}/$(WANDER1).o ${OBJ}/$(WANDER2).o,$(OBJECTS))
SUBDIRS=${OBJ} ${BIN}

.PHONY:clean cleanall all

all: ${SUBDIRS} ${WANDER1_BIN} ${WANDER2_BIN}
	

${SUBDIRS}:%:
	@mkdir -p $@

${WANDER1_BIN}: ${BASE_OBJS} ${WANDER1_OBJ}
	${CXX} $^ -o $@ ${LINK_FLAGS}
	@echo "build $@ successfully"

${WANDER2_BIN}: ${BASE_OBJS} ${WANDER2_OBJ}
	${CXX} $^ -o $@ ${LINK_FLAGS}
	@echo "build $@ successfully"

sinclude ${OBJECTS:.o=.d} 

$(OBJ)/%.o:${SRC}/%.cpp 
	$(CXX) $(C_FLAG) -o $@ -c $<

$(OBJ)/%.d:${SRC}/%.cpp 
	@mkdir -p ${OBJ}
	@echo "Making $@ ..."
	@${CXX}  $< ${C_FLAG} -MM -MT"$(OBJ)/$*.o" -MT"$@" -MF"$@" 

clean:
	@echo "Removing binary files......"
	-rm -rf $(OBJ)/*.o $(BIN)/*.*

cleanall:
	@echo "Removing all produced files......"
	-rm -rf $(OBJ) $(BIN) 

