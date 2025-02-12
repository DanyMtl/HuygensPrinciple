# Compiler
CXX = g++

TGUI_HEADERS=/opt/homebrew/include/ 
SFML_HEADERS=/opt/homebrew/include/ 
TGUI_LIBS=/opt/homebrew/lib/
SFML_LIBS=/opt/homebrew/lib/

# Compiler flags
CXXFLAGS = -std=c++23 -Wall -Wextra -O2  -I$(SFML_HEADERS) -I$(TGUI_HEADERS)

# Frameworks to link (macOS-specific)
LDFLAGS = -L$(SFML_LIBS) -L$(TGUI_LIBS) -ltgui -lsfml-graphics -lsfml-window -lsfml-system

# Target executable
TARGET = HuygensPrinciple

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source files
#SRCS = main.cpp WaveClasses.cpp GUIClass.cpp
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files (generated from source files)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Rule to build the target executable
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not actual files)
.PHONY: all clean
