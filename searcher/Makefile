CXX = g++
#CXXFLAGS =
CXXFLAGS = -DDEBUG=1

SRC = searcher.cpp
OBJ = $(SRC:.cpp=.o)
EXE = searcher

TSTSRC = bitarray_test.cpp
TSTOBJ = $(TSTSRC:.cpp=.o)
TSTEXE = test

all: $(OBJ)
	$(CXX) -Ofast $^ -o $(EXE) $(CXXFLAGS)

test: $(TSTOBJ)
	$(CXX) -O0 $^ -o $(TSTEXE)

%.o: %.cpp
	$(CXX) -c -O0 $? $(CXXFLAGS)
