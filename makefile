# Compiler and Flags
CXX := clang++
CXXFLAGS := -Wall -Wextra -std=c++11 -stdlib=libstdc++
# Add include paths
CXXFLAGS += -I/usr/include/c++/11 \
            -I/usr/include/x86_64-linux-gnu/c++/11 \
            -I/usr/include/c++/11/backward \
            -I/usr/lib/gcc/x86_64-linux-gnu/11/include \
            -I/usr/local/include \
            -I/usr/include/x86_64-linux-gnu \
            -I/usr/include
#
# Add OpenSSL flags
CXXFLAGS += -I/usr/include/openssl
# Add linker path for C++ standard library
LDFLAGS := -L/usr/lib/gcc/x86_64-linux-gnu/11
# Add OpenSSL library
LDFLAGS += -lcrypto
# Project-specific includes
INCLUDES := -Iinclude

# Directories
SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := tests
OBJ_DIR := build
BIN_DIR := bin# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)

# Generate object file names
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/test_%.o)

# Main executables
MAIN_TARGETS := logAppend logRead
EXECUTABLES := $(addprefix $(BIN_DIR)/,$(MAIN_TARGETS))
TEST_EXECUTABLE := $(BIN_DIR)/runTests

# Ensure the build directories exist
DIRS := $(OBJ_DIR) $(BIN_DIR)

# Default target
all: makedirs $(EXECUTABLES) $(TEST_EXECUTABLE)

# Create necessary directories
makedirs:
	@mkdir -p $(DIRS)

# Pattern rule for main executables
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(OBJ_DIR)/inputValidation.o $(OBJ_DIR)/inputValidationLogAppend.o
	@echo "Linking $@..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Successfully created $@"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Successfully compiled $<"

# Compile test files
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.cpp
	@echo "Compiling test $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Successfully compiled test $<"

# Link test executable
$(TEST_EXECUTABLE): $(TEST_OBJS) $(filter-out $(OBJ_DIR)/logAppend.o $(OBJ_DIR)/logRead.o,$(OBJS))
	@echo "Linking tests..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ -lgtest -lgtest_main -pthread $(LDFLAGS)
	@echo "Successfully created test executable"

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Run tests
test: $(TEST_EXECUTABLE)
	@echo "Running tests..."
	@./$(TEST_EXECUTABLE)

# Declare phony targets
.PHONY: all clean test makedirs

# Include dependencies
-include $(OBJS:.o=.d)

