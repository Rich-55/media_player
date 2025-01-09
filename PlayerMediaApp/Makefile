# Compiler and flags
CC = g++
CFLAGS = -Iinclude -std=c++17 -Wall -Wextra -Werror
#taglib
LIBS = -L/path/to/taglib -ltag


# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
SRC = $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Target executable
TARGET = MediaPlayer

# Rules
all: $(TARGET)

# Build target
$(TARGET): $(OBJ)
	$(CC) -o $@ main.cpp $^ $(CFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
