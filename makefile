# Compiler
CXX = clang++

TGUI_HEADERS=/usr/local/include/ 
SFML_HEADERS=/usr/local/include/ 
TGUI_LIBS=/usr/local/lib/
SFML_LIBS=/usr/local/lib/

ifeq ($(shell uname), Darwin)
  MACOS_HEADER=$(shell xcrun --show-sdk-path)/usr/include/c++/v1/
else
  MACOS_HEADER=''
endif
# MACOS_HEADER=/Library/Developer/CommandLineTools/SDKs/MacOSX15.1.sdk/usr/include/c++/v1/cstdint

# Compiler flags
CXXFLAGS = -std=c++20 -Wall -Wextra -O2  -I$(SFML_HEADERS) -I$(TGUI_HEADERS) -I$(MACOS_HEADER)

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
