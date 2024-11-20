# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iinclude

# OpenSSL Libraries
OPENSSL_LIBS = -lssl -lcrypto

# Source Files
SRCS = include/logAppend.cpp include/logRead.cpp include/crypto.cpp

# Object Files (from corresponding .cpp files)
OBJS = logAppend.o logRead.o crypto.o

# Executable Names
EXEC_LOGAPPEND = logAppend
EXEC_LOGREAD = logRead

# Build Targets
all: $(EXEC_LOGAPPEND) $(EXEC_LOGREAD)

$(EXEC_LOGAPPEND): logAppend.o crypto.o
	$(CXX) -o $@ $^ $(OPENSSL_LIBS)

$(EXEC_LOGREAD): logRead.o crypto.o
	$(CXX) -o $@ $^ $(OPENSSL_LIBS)

# Compile Source Files to Object Files
%.o: include/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f *.o $(EXEC_LOGAPPEND) $(EXEC_LOGREAD)

.PHONY: all clean

