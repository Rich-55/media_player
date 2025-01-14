# Compiler and flags
CC = g++
CFLAGS = -Iinclude -std=c++17 -Wall -Wextra -Werror 
LIBS = -ltag -lstdc++ -lncurses -lncursesw

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
SRC = $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
MAIN_OBJ = $(BUILD_DIR)/main.o

# Target executable
TARGET = MediaPlayer

# Rules
all: $(TARGET)

# Build target
$(TARGET): $(MAIN_OBJ) $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Build main.o explicitly
$(MAIN_OBJ): main.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
