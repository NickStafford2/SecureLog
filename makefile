
# Compiler and Flags
CXX := clang++
CXXFLAGS := -Wall -Wextra -std=c++11
CXXFLAGS += -I/usr/include/c++/11 \
            -I/usr/include/x86_64-linux-gnu/c++/11 \
            -I/usr/include/c++/11/backward \
            -I/usr/lib/gcc/x86_64-linux-gnu/11/include \
            -I/usr/local/include \
            -I/usr/include/x86_64-linux-gnu \
            -I/usr/include
#
LDFLAGS := -L/usr/lib/gcc/x86_64-linux-gnu/11 -lcrypto
INCLUDES := -Iinclude -I/usr/include/openssl

# Directories
SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin

# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate object file names dynamically from source files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Shared object files for both executables
SHARED_OBJS := $(OBJ_DIR)/crypto.o $(OBJ_DIR)/inputValidation.o $(OBJ_DIR)/inputValidationLogAppend.o


# Specify the executables you want to build
EXECUTABLES := $(BIN_DIR)/logAppend $(BIN_DIR)/logRead

# Default target
all: $(EXECUTABLES)

# Rule for linking the logAppend executable
$(BIN_DIR)/logAppend: $(OBJ_DIR)/logAppend.o $(SHARED_OBJS)
	@echo "Linking $@..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Successfully created $@"

# Rule for linking the logRead executable
$(BIN_DIR)/logRead: $(OBJ_DIR)/logRead.o $(SHARED_OBJS)
	@echo "Linking $@..."
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Successfully created $@"

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Successfully compiled $<"

# Clean build files
clean:
	@echo "Cleaning build files..."
	@rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
	@echo "Clean complete"

# Declare phony targets
.PHONY: all clean

# Include dependencies (generated automatically from source files)
-include $(OBJS:.o=.d)

