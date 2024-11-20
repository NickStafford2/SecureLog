# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude

# OpenSSL Libraries
OPENSSL_LIBS = -lssl -lcrypto

# Directories
SRC_DIR = include
OBJ_DIR = build
INCLUDE_DIR = include

# Source Files (with .cpp files located in the 'include' directory)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object Files (create object files in the 'build' directory)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable Names
EXEC_LOGAPPEND = logAppend
EXEC_LOGREAD = logRead

# Build Targets
all: $(EXEC_LOGAPPEND) $(EXEC_LOGREAD)

$(EXEC_LOGAPPEND): $(OBJ_DIR)/logAppend.o $(OBJ_DIR)/crypto.o
	$(CXX) -o $@ $^ $(OPENSSL_LIBS)

$(EXEC_LOGREAD): $(OBJ_DIR)/logRead.o $(OBJ_DIR)/crypto.o
	$(CXX) -o $@ $^ $(OPENSSL_LIBS)

# Compile Source Files to Object Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC_LOGAPPEND) $(EXEC_LOGREAD)

.PHONY: all clean

