#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "crypto.h"

void logappend(int timestamp, const std::string& token, const std::string& logEntry) {
    // Validate timestamp and token here (omitted for brevity)

    std::string formattedLogEntry = std::to_string(timestamp) + " " + token + " " + logEntry;

    // Generate or retrieve encryption key (for simplicity, using a fixed key here)
    std::vector<unsigned char> key = Crypto::generateKey(); // Replace with a secure key retrieval method

    // Encrypt the log entry
    auto encryptedLog = Crypto::encrypt(formattedLogEntry, key);

    // Write to log file
    std::ofstream logFile("logfile.log", std::ios::app | std::ios::binary);
    if (!logFile) {
        std::cerr << "invalid" << std::endl;
        return;
    }
    logFile.write((char*)encryptedLog.data(), encryptedLog.size());
    logFile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "invalid" << std::endl;
        return 255;
    }

    int timestamp = std::stoi(argv[1]); // e.g., from -T
    std::string token = argv[2];          // e.g., from -K
    std::string logEntry = argv[3];       // e.g., the actual log entry

    logappend(timestamp, token, logEntry);
    return 0;
}

