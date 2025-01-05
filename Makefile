# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
MAIN_DIR = main
BUILD_DIR = build

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
MAIN_OBJ = $(BUILD_DIR)/main.o

# Output binary
TARGET = Media_player.exe

# Default target
all: $(TARGET)

# Generate only object files
objects: $(OBJS) $(MAIN_OBJ)
	@echo "Object files generated in $(BUILD_DIR)"

# Build the target
$(TARGET): $(OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build main object file
$(BUILD_DIR)/main.o: $(MAIN_DIR)/main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/main.o

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Clean up build files
clean:
	del /q /s build\* || rmdir /s /q build
	del $(TARGET)

.PHONY: all clean
