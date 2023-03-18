CXX = g++
CXX_FLAGS = -g -std=c++17 -Wall -pedantic -Wextra
RM = rm -f

SRC = .
EXEC = run.me
OBJ = $(patsubst %.cpp, %.o, $(wildcard $(SRC)/*.cpp))

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXX_FLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

clean:
	$(RM) $(EXEC) $(OBJ)

rebuild: clean all

test1:
	./$(EXEC) Tests/test-1.txt < Tests/input_string.txt

test2:
	./$(EXEC) Tests/test-2.txt < Tests/input_string.txt