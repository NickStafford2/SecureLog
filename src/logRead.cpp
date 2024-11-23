#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "inputValidationLogRead.h"

// check all the options like -K -S -R -E -G
void check_command(int argc,
                   char *argv[]) { // ./logread -K secret -R -E Fred log1
  if (argc == 5) {
    assert(std::strcmp(argv[1], "-K") == 0 && std::strcmp(argv[3], "-S") == 0);
    std::cout << "Argument content check successful!" << std::endl;
  } else if (argc == 7) {
    assert(
        std::strcmp(argv[1], "-K") == 0 && std::strcmp(argv[3], "-R") == 0 &&
        (std::strcmp(argv[4], "-E") == 0 || std::strcmp(argv[4], "-G") == 0));
    std::cout << "Argument content check successful!" << std::endl;
  }
}
// this function validates the name of the file
std::string file_validation(const std::string &filename) {
  for (char n : filename) {
    if (!std::isalnum(n) && n != '_' && n != '.') {
      std::cerr << "Invalid: Filename " << std::endl;
      exit(255);
    }
  }
  std::cout << "Filename is successfully validated" << std::endl;
  return filename;
}

// this function validates all secret token
void token_validation(const std::string &str) {
  for (char c : str) {
    if (!std::isalnum(c)) {
      std::cerr << "Invalid: Token" << std::endl;
      exit(255);
    }
  }
  std::cout << "Token is successfully validated!" << std::endl;
  return;
}

// name_validation validates the name of employee or guest
void name_validation(const std::string &name) {
  for (char n : name) {
    if (!std::isalpha(n)) {
      std::cerr << "Invalid: Name" << std::endl;
      exit(255);
    }
  }
  std::cout << "Name is successfully validated" << std::endl;
  return;
}


// processArgument prints all the names of employees and guests that went to the
// gallery as well as each person that entered in a particular room
void processArgument(const std::string &filename, const std::string &key) {
  std::vector<std::string> namesPeople; // List of unique people
  std::unordered_map<int, std::vector<std::string>>
      roomAssignments; // room -> people mapping

  std::fstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening the file!" << std::endl;
    exit(255);
  }

  std::string line;

  std::vector<std::string> wordList;
  std::istringstream iss(line);
  std::string word;

  // Split the line into words

  while (iss >> word) {
    wordList.push_back(word);
  }

  if (key == wordList[3]) {
    std::string name = wordList[6];

    // Add to namesPeople if not already present
    if (std::find(namesPeople.begin(), namesPeople.end(), name) ==
        namesPeople.end()) {
      namesPeople.push_back(name);
    }

    // Handle room assignment
    if (wordList.size() == 10) {
      int roomNumber = std::stoi(wordList[8]);
      if (std::find(roomAssignments[roomNumber].begin(),
                    roomAssignments[roomNumber].end(),
                    name) == roomAssignments[roomNumber].end()) {
        roomAssignments[roomNumber].push_back(name);
      }
    }
  }
}
// file.close();
//
// // printing names of people
// for (const auto &name : namesPeople) {
//   std::cout << name << std::endl;
// }

// // printing room assignments
// for (const auto &room : roomAssignments) {
//   std::cout << room.first << ": ";
//   for (size_t i = 0; i < room.second.size(); ++i) {
//     std::cout << room.second[i];
//     if (i < room.second.size() - 1) {
//       std::cout << ",";
//     }
//   }
//   std::cout << std::endl;
// }
// }


// processBigArguments(filename, key, (-E or -G), name);
// processBigArguments prints all the room that a person went
void processBigArgument(const std::string &filename, const std::string &key,
                        const std::string &type, const std::string &name) {
  std::vector<std::string> rooms;
  std::string previous_room = "";

  std::fstream file(filename);
  if (!file.is_open()) {
    std::cout << "Error opening file!" << std::endl;
    exit(255);
  }
  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> wordList;
    std::istringstream iss(line);
    std::string word;

    while (iss >> word) {
      wordList.push_back(word);
    }

    if (key == wordList[3] && type == wordList[5] && name == wordList[6] &&
        previous_room != wordList[8]) {
      rooms.push_back(wordList[8]);
      previous_room = wordList[8];
    }
  }

  std::cout << std::endl;
  for (size_t i = 0; i < rooms.size(); ++i) {
    std::cout << rooms[i];
    if (i < rooms.size() - 1) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  try {
    // LogReadArgs
    LogReadArgs args(argc, argv);
    std::cout << "First round of validation complete" << std::endl;
    args.print();

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 255;
  }
}
