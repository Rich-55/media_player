# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -pthread -g
PKG_CONFIG_FLAGS = $(shell pkg-config --cflags --libs libavformat libavcodec libswresample libavutil libswscale sdl2 SDL2_mixer)

# Source files and target
SRC = MediaModel.cpp MediaController.cpp MediaView.cpp main.cpp
TARGET = med

# Default rule
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CXX) -o $(TARGET) $(SRC) $(CXXFLAGS) $(PKG_CONFIG_FLAGS)

# Clean up build files
clean:
	rm -f $(TARGET)

# Convenience rule to recompile
rebuild: clean all
