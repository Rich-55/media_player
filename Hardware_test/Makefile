# Makefile for building the Media application

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -g $(shell pkg-config --cflags libavformat libavcodec libswresample libavutil libswscale sdl2 SDL2_mixer)
LDFLAGS = $(shell pkg-config --libs libavformat libavcodec libswresample libavutil libswscale sdl2 SDL2_mixer)

# Source files and target
SRCS = MediaModel.cpp MediaController.cpp MediaView.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = med

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
