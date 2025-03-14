CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iparser -Imodel -Ivisualization
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = .
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/model/*.cpp $(SRC_DIR)/visualization/*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)
TARGET = plotter2d

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean run