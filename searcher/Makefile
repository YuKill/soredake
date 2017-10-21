CXX = g++
EXE = searcher
#CXXFLAGS =
CXXFLAGS = -DDEBUG=1

SRC = *.cpp
OBJ = $(SRC:.cpp=.o)

all: $(OBJ)
	$(CXX) -Ofast $^ -o $(EXE) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -c -Ofast $? $(CXXFLAGS)
