# Makefile
# Compiler and Flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude

# OpenSSL Libraries
OPENSSL_LIBS = -lssl -lcrypto

# Directories
SRC_DIR = include
OBJ_DIR = build
INCLUDE_DIR = include

# Source Files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object Files (create object files in the 'build' directory)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable Names
EXEC_LOGAPPEND = logAppend
EXEC_LOGREAD = logRead

# Build Targets
all: $(OBJ_DIR) $(EXEC_LOGAPPEND) $(EXEC_LOGREAD)

$(EXEC_LOGAPPEND): $(OBJ_DIR)/logAppend.o $(OBJ_DIR)/crypto.o $(OBJ_DIR)/inputValidation.o
	@echo "Compiling $(EXEC_LOGAPPEND)..."
	$(CXX) -o $@ $^ $(OPENSSL_LIBS)
	@echo "$(EXEC_LOGAPPEND) compiled successfully!"

$(EXEC_LOGREAD): $(OBJ_DIR)/logRead.o $(OBJ_DIR)/crypto.o
	@echo "Compiling $(EXEC_LOGREAD)..."
	$(CXX) -o $@ $^ $(OPENSSL_LIBS)
	@echo "$(EXEC_LOGREAD) compiled successfully!"

# Compile Source Files to Object Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $< to $@..."
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled $< successfully!"

# Ensure build directory exists
$(OBJ_DIR):
	@echo "Creating build directory..."
	mkdir -p $(OBJ_DIR)
	@echo "Build directory created."

# Clean up build files
clean:
	@echo "Cleaning up build files..."
	rm -rf $(OBJ_DIR)/*.o logAppend logRead
	@echo "Clean up successful."

.PHONY: all clean $(OBJ_DIR)

