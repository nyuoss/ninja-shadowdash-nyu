# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I./include

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_DIR)/build/*.cpp) \
          $(wildcard $(SRC_DIR)/lexer/*.cpp) \
          $(wildcard $(SRC_DIR)/parser/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Dependencies
DEPS = $(OBJECTS:.o=.d)

# Output executable
TARGET = ninja_converter

# Create build directory structure
$(shell mkdir -p $(BUILD_DIR) $(BUILD_DIR)/build $(BUILD_DIR)/lexer $(BUILD_DIR)/parser)

# Main target
all: $(BUILD_DIR)/$(TARGET)

# Linking
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependencies
-include $(DEPS)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Rebuild everything
rebuild: clean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all     : Build the project (default)"
	@echo "  clean   : Remove build files"
	@echo "  rebuild : Clean and rebuild project"
	@echo "  help    : Show this help message"

.PHONY: all clean rebuild help