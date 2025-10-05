# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS =

# Source and object files
SRC_DIR = src
OBJ_DIR = obj
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Executable name
EXECUTABLE = sequence_game

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

# Make without warnings by default; use `make warnings` to build with -Wall.
.DEFAULT_GOAL := build_nowarn

build_nowarn:
	$(MAKE) all CXXFLAGS='-std=c++17 -Iinclude'

warnings:
	$(MAKE) clean all CXXFLAGS='-std=c++17 -Wall -Iinclude'

run: build_nowarn
	./$(EXECUTABLE)

.PHONY: all clean run
