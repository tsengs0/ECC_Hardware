CXX = g++

CXXFLAGS= -O0 -std=c++11 -g -lm -Wall -pedantic
EXE_NAME=huffman_en_de

SRC = $(wildcard *.cpp)
# $(info SRC is $(SRC))
OBJ = $(addsuffix .o, $(basename $(SRC)))

LIB_SRC := $(filter-out main.cpp,$(SRC))
# basename take the basic file name
LIB_OBJ := $(addsuffix .o, $(basename $(LIB_SRC)))

REBUILDABLES=$(OBJ) $(EXE_NAME)

all: ${EXE_NAME}
	mv *.o obj/

run:
	./${EXE_NAME}
#   $@ target name, $^ target deps, $< matched pattern
$(EXE_NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ 
	@echo "Built $@ successfully" 

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean: 
	rm -f $(REBUILDABLES) *.dep *.deppo *~ *.o *.txt
	rm obj/*.o
	rm ${EXE_NAME}
