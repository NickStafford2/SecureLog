#include <iostream>
#include <fstream>
#include <vector>
#include "crypto.h"

void logread(const std::string& token) {
    std::ifstream logFile("logfile.log", std::ios::binary);
    if (!logFile) {
        std::cerr << "integrity violation" << std::endl;
        return;
    }

    std::vector<unsigned char> encryptedLog((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
    logFile.close();

    // Retrieve the encryption key (for simplicity, using a fixed key here)
    std::vector<unsigned char> key = Crypto::generateKey(); // Replace with a secure key retrieval method

    // Decrypt the log entries
    std::string decryptedLog = Crypto::decrypt(encryptedLog, key);
    
    // Output the decrypted log
    std::cout << decryptedLog << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "invalid" << std::endl;
        return 255;
    }

    std::string token = argv[2]; // e.g., from -K
    logread(token);
    return 0;
}

