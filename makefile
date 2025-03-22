# Makefile for HTTP Server

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11
LDFLAGS = -pthread

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRCS = main.cpp \
       $(SRC_DIR)/HttpServer.cpp \
       $(SRC_DIR)/Response.cpp \
	   ${SRC_DIR}/Request.cpp

# Object files
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Target executable
TARGET = $(BIN_DIR)/http_server

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/$(SRC_DIR) $(BIN_DIR)

# Compile the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile main.cpp
$(BUILD_DIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean directories