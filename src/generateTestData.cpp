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

int getRandomNumber(int min, int max) { return rand() % (max - min + 1) + min; }

bool returnFalseXPercent(int x) {
  // Generate a random number between 1 and 100
  int randomNumber = rand() % 100 + 1;
  return randomNumber > x;
}
// Random number generator initialized once for performance
std::random_device rd;  // Obtain a random number from hardware
std::mt19937 gen(rd()); // Mersenne Twister engine
std::uniform_int_distribution<> nameDistrib(0, 19); // For names (index 0 to 19)
std::uniform_int_distribution<> roomDistrib(0, 6);  // For rooms (index 0 to 3)

std::vector<std::string> names = {
    "Alice",  "Bob",  "Charlie", "David", "Eve",   "Frank", "Grace",
    "Hannah", "Ivy",  "Jack",    "Kathy", "Louis", "Megan", "Nathan",
    "Olivia", "Paul", "Quincy",  "Rita",  "Sam",   "Tina"};

// -1 and -2 for gallery and outside
std::vector<int> rooms = {-2, -1, -1, -1, 1, 2};

std::string getRandomName() { return names[nameDistrib(gen)]; }

std::string getRandomRoom() {
  int room = rooms[roomDistrib(gen)];
  // std::cout << room << std::endl;
  if (room > 0)
    return " -R " + std::to_string(room);
  return "";
}
std::string getTimestamp(int index) {
  // if (returnFalseXPercent(5)) {
  //   int randomSmallNumber = getRandomNumber(-4, index - 1);
  //   return "-T " + std::to_string(randomSmallNumber); // bad timestamp
  // }
  return "-T " + std::to_string(1000 + index); // Ensure increasing timestamps
}

bool isPersonEmployee(const std::string &name) {
  char firstLetter = name[0];
  return (firstLetter < 'K');
}

std::string generateTestData(int index) {
  std::stringstream ss;
  std::string timestamp = getTimestamp(index);
  std::string token = "-K secret"; // Use a constant token for simplicity
  std::string name = getRandomName();
  std::string personFlag = (isPersonEmployee(name)) ? "-E" : "-G";
  std::string action = (index % 2 == 0) ? "-A " : "-L ";
  std::string room = getRandomRoom();

  // For fuzz testing, introduce some "invalid" fields and randomness
  if (returnFalseXPercent(
          100)) { // 10% invalid data, like missing fields, extra symbols, etc.
    ss << "-T -33 -K SECRET -A -E " << name << " logfile";
  } else if (index % 15 == 0) { // Introduce completely weird input
    ss << "-X " << (1000 + index) << " -Y secret -Z -A -E " << name
       << " ## random_log";
  } else {
    ss << timestamp << " " << token << " " << personFlag << " " << name << " "
       << action << room << " " << "biglog.txt";
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
  std::string fileName = "batches/bigBatch.txt";

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
