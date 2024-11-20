// generateTestData.cpp
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

bool fileExists(const std::string &fileName) {
  std::ifstream file(fileName);
  return file.good();
}

std::string generateTestData(int index) {
  std::stringstream ss;
  std::string timestamp =
      "-T " + std::to_string(1000 + index); // Ensure increasing timestamps
  std::string token = "-K secret"; // Use a constant token for simplicity

  // List of 20 random names
  std::vector<std::string> names = {
      "Alice",  "Bob",  "Charlie", "David", "Eve",   "Frank", "Grace",
      "Hannah", "Ivy",  "Jack",    "Kathy", "Louis", "Megan", "Nathan",
      "Olivia", "Paul", "Quincy",  "Rita",  "Sam",   "Tina"};

  // Randomly select a name from the list
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, names.size() - 1);
  std::string name = names[dis(gen)];

  // Choose employee or guest
  std::string personFlag = (index % 2 == 0) ? "-E" : "-G";

  // Action: either Arrival (A) or Leaving (L)
  std::string action = (index % 2 == 0) ? "-A" : "-L";

  // Optional room ID: 1 to 10 for guests and employees, or fuzz values
  std::string room =
      (index % 5 == 0) ? "" : " -R " + std::to_string((index % 10) + 1);

  // Random log file name
  std::vector<std::string> logFiles = {"log1", "log2", "log3", "log4", "log5",
                                       "log6", "log7", "log8", "log9", "log10"};
  std::string logFile = logFiles[index % logFiles.size()];

  // For fuzz testing, introduce some "invalid" fields and randomness
  if (index % 10 ==
      0) { // 10% invalid data, like missing fields, extra symbols, etc.
    ss << "-T INVALID_TIMESTAMP -K SECRET -A -E " << name << " logfile";
  } else if (index % 15 == 0) { // Introduce completely weird input
    ss << "-X " << (1000 + index) << " -Y secret -Z -A -E " << name
       << " ## random_log";
  } else {
    ss << timestamp << " " << token << " " << personFlag << " " << name << " "
       << action << room << " " << logFile;
  }

  return ss.str();
}

std::string getUniqueBatchFileName() {
  int batchNumber = 1;
  std::string fileName = "batch" + std::to_string(batchNumber) + ".txt";

  // Check if file already exists
  while (fileExists(fileName)) {
    batchNumber++;
    fileName = "batch" + std::to_string(batchNumber) + ".txt";
  }

  return "./batches/" + fileName;
}

int main() {
  // Generate a unique file name
  std::string fileName = getUniqueBatchFileName();

  // Open the file for writing
  std::ofstream outputFile(fileName);
  if (!outputFile.is_open()) {
    std::cerr << "Failed to open the file for writing!" << std::endl;
    return 1;
  }

  // Generate and write 100 test data entries to the file
  for (int i = 0; i < 100; ++i) {
    std::string testData = generateTestData(i);
    outputFile << testData << std::endl;
  }

  // Close the file
  outputFile.close();

  std::cout << "Test data saved to " << fileName << std::endl;

  return 0;
}
