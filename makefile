# Makefile

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
MAKEFLAGS += --no-print-directory

# Directories
SRC_DIR := src
LOG_DIR := logs
OBJ_DIR := build
BIN_DIR := bin

# Find all source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate object file names dynamically from source files
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Shared object files for both executables
SHARED_OBJS := $(OBJ_DIR)/crypto.o $(OBJ_DIR)/inputValidation.o $(OBJ_DIR)/logAppendArgs.o $(OBJ_DIR)/logReadArgs.o $(OBJ_DIR)/gallery.o $(OBJ_DIR)/participantType.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/event.o

# Specify the executables you want to build
EXECUTABLES := $(BIN_DIR)/logAppend $(BIN_DIR)/logRead $(BIN_DIR)/generateTestData

# Default target
all: build $(EXECUTABLES)

# Rule for linking the logAppend executable
$(BIN_DIR)/logAppend: $(OBJ_DIR)/logAppend.o $(SHARED_OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Rule for linking the logRead executable
$(BIN_DIR)/logRead: $(OBJ_DIR)/logRead.o $(SHARED_OBJS)
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Rule for linking the genBatch executable
$(BIN_DIR)/generateTestData: $(OBJ_DIR)/generateTestData.o
	@$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Successfully compiled $<"

# Clean build files
clean:
	@rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
	@echo "Clean complete"

build:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LOG_DIR)
	@mkdir -p $(OBJ_DIR)

########################################################################
# Tests to use in presentation

testCorruptedFile:
	@echo "Testing logAppend on corrupted File"
	./$(BIN_DIR)/logAppend -T 1001001 -K secret -A -G newPerson corruptedFile.txt
	@echo "\n"
	
testAppendBatch:
	@echo "Testing normal logAppend batch"
	@rm -rf logs/nsTest1.txt 
	./$(BIN_DIR)/logAppend -B nsTestBatch.txt 
	@echo "\n"

# Rule for the testData target that runs the genBatch rule
genBigData: genBatch
	@rm -rf batches/bigBatch.txt 
	./$(BIN_DIR)/generateTestData
	@echo "Test data generation complete."

testAppendBigBatch:
	@echo "Testing big random data logAppend batch"
	@rm -rf logs/biglog.txt 
	./$(BIN_DIR)/logAppend -B bigBatch.txt 
	@echo "\n"
	
testReadBigBatch:
	@echo "Testing bigBatch logRead"
	./$(BIN_DIR)/logRead -K secret -S biglog.txt

test:
	@echo "\n"
	@$(MAKE) testAppendBatch
	@$(MAKE) testCorruptedFile
	@$(MAKE) genBigData 
	@$(MAKE) testAppendBigBatch
	@$(MAKE) testReadBigBatch



########################################################################





testAppend:
	@echo "Testing Normal logAppend"
	@rm -rf logs/nsTest1.txt && \
	./$(BIN_DIR)/logAppend -B nsTestBatch.txt && \ 

testRead:
	@echo "Testing Normal logRead"
	./$(BIN_DIR)/logRead -K secret -S nsTest1.txt

testOther:
	./$(BIN_DIR)/logRead -K secret -G nick nsTest1.txt
	./$(BIN_DIR)/logRead -K secret -R -G nick nsTest1.txt



# Declare phony targets
.PHONY: all clean genBatch testData build

# Include dependencies (generated automatically from source files)
-include $(OBJS:.o=.d)

