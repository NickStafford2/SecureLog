#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "inputValidation.h"
#include "inputValidationLogAppend.h"

// check_constraints checks all of the constraints to of the state of the
// gallery
// bool check_constraints(char *argv[], std::string filename) {
//   // std::cout << "check_constraint is working" << std::endl;
//   std::vector<std::string> inputList;
//   for (int i = 0; argv[i] != nullptr; ++i) {
//     inputList.push_back(argv[i]);
//   }
//
//   std::vector<std::string> lastList;
//   std::fstream file(filename);
//
//   if (!file.is_open()) {
//     std::cout << "Error opening the file!" << std::endl;
//     exit(255);
//   }
//   std::string line;
//   while (std::getline(file, line)) {
//
//     std::vector<std::string> wordList;
//     std::istringstream iss(line);
//     std::string word;
//     // Split line into words and store each word in the list
//     while (iss >> word) {
//       wordList.push_back(word);
//     }
//     if (wordList[3] == argv[4] && wordList[5] == argv[6] &&
//         wordList[6] == argv[7]) {
//       lastList = wordList;
//     }
//   }
//   file.close();
//   // constraint checking
//
//   if (lastList.size() == 8 && lastList[4] == "-L" && inputList.size() == 9 &&
//       inputList[5] == "-A") {
//     return true;
//   }
//   if (lastList.size() == 8 && lastList[4] == "-L") {
//     std::cout << "Not in the Gallery" << std::endl;
//     exit(255);
//   }
//   if (lastList.size() == 0 && inputList.size() == 9 && inputList[5] == "-A")
//   {
//     std::cout << "New Employee or Guest coming in!" << std::endl;
//     return true;
//   }
//   if (lastList.size() == 0 && inputList.size() == 9 && inputList[5] == "-L")
//   {
//     std::cout << "Invalid" << std::endl;
//     exit(255);
//   }
//   if (lastList.size() == 0 && inputList.size() == 11) {
//     std::cout << "Invalid" << std::endl;
//     exit(255);
//   }
//
//   if (lastList.size() == 8 && inputList.size() == 11 && lastList[4] == "-A"
//   &&
//       inputList[5] == "-L") {
//     std::cout << "Invalid" << std::endl;
//     exit(255);
//   }
//   if (lastList.size() == 8 && inputList.size() == 11 && lastList[4] == "-A"
//   &&
//       inputList[5] == "-A") {
//     return true;
//   }
//
//   if (lastList.size() == 10 && inputList.size() == 9 && lastList[4] == "-A"
//   &&
//       inputList[5] == "-A") {
//     std::cout << "Invalidd, cannot arrive at gallery when in room" <<
//     std::endl; exit(255);
//   }
//   if (lastList.size() == 10 && inputList.size() == 9 && lastList[4] == "-L"
//   &&
//       inputList[5] == "-L") {
//     return true;
//   }
//
//   if (lastList.size() == 10 && inputList.size() == 11 && lastList[4] == "-A"
//   &&
//       inputList[5] == "-L" && lastList[8] == inputList[9]) {
//     return true;
//   }
//   if (lastList.size() == 10 && inputList.size() == 11 && lastList[4] == "-A"
//   &&
//       inputList[5] == "-L" && lastList[8] != inputList[9]) {
//     std::cout << "Invalid, cannot leave from another room" << std::endl;
//     exit(255);
//   }
//   if (lastList.size() == 10 && inputList.size() == 11 && lastList[4] == "-A"
//   &&
//       inputList[5] == "-A" && lastList[8] != inputList[9]) {
//     std::cout << "Invalid, cannot go to one room to another" << std::endl;
//     exit(255);
//   }
//   if (std::equal(lastList.begin() + 2, lastList.end(), inputList.begin() +
//   3)) {
//     if (lastList.size() == 8) {
//       std::cout << "Already in the gallery" << std::endl;
//     } else if (lastList.size() == 10) {
//       std::cout << "Already in the room" << std::endl;
//     }
//
//     exit(255);
//   }
//   return false;
// }
// // executor runs the single line command
// void executor(int argc, char *argv[]) {
//   std::cout << std::endl;
//   std::string filename;
//   std::cout << "Argument length: " << argc << std::endl;
//   if (argc == 9 || argc == 11) {
//     //   std::cout << "Argument length check successful!" << std::endl;
//     //   // all the leftover code goes here
//     //   // check if it is 9, 3, or 11
//     //   if (argc == 9) {
//     //     check_command(argc, argv);
//     //     filename = file_validation(argv[8]) + ".txt";
//     //     validate_timestamp(argc, std::stoi(argv[2]), filename);
//     //     token_validation(argv[4]);
//     //     name_validation(argv[7]);
//     //     check_constraints(argv, filename);
//     //
//     //     std::cout << "It is a single command with no room in it." <<
//     //     std::endl;
//     //
//     //   } else if (argc == 11) {
//     //     check_command(argc, argv);
//     //     filename = file_validation(argv[10]) + ".txt";
//     //     validate_timestamp(argc, std::stoi(argv[2]), filename);
//     //     token_validation(argv[4]);
//     //     name_validation(argv[7]);
//     //     room_validation(std::stoi(argv[9]));
//     //     check_constraints(argv, filename);
//     //     std::cout << "It is a single command with room in it" <<
//     std::endl;
//     //   }
//     //
//     //   // lets try to insert into the file
//     // open the file for appending
//     std::ofstream writeFile(filename, std::ios::app);
//
//     while (!writeFile.is_open()) {
//       std::cout << "Error opening the file for writing" << std::endl;
//       exit(255);
//     }
//
//     for (int i = 1; i < argc; i++)
//       writeFile << argv[i] << " ";
//     writeFile << "\n";
//     writeFile.close();
//   } else {
//     std::cout << "INVALID!" << std::endl;
//     std::cout << "Program exited due to invalid arguments. Please provide "
//                  "correct number of arguments."
//               << std::endl;
//     std::exit(255);
//   }
// }
// // process_single_command process each line from the batch file
// void process_single_command(const std::string &command) {
//   if (command.empty() || command.find("-B") != std::string::npos) {
//     std::cerr << "Invalid command: " << command << std::endl;
//     return;
//   }
//
//   // Split command into arguments (argc and argv format)
//   std::istringstream iss(command);
//   std::vector<std::string> args;
//   std::string arg;
//   args.push_back("logappend");
//   while (iss >> arg) {
//     args.push_back(arg);
//   }
//
//   // Prepare argc and argv for the executor
//   int argc = args.size();
//   std::vector<char *> argv(argc);
//
//   for (size_t i = 0; i < args.size(); ++i) {
//     argv[i] = const_cast<char *>(args[i].c_str());
//   }
//
//   // Call the executor function
//   std::cout << std::endl;
//   for (int i = 0; i < argc; ++i) {
//     std::cout << argv[i] << " ";
//   }
//   executor(argc, argv.data());
// }
//
// get_most_recent_time grabs the most recent time present in that file
int get_most_recent_time(std::string logFile) {
  // std::cout << "get most recent time is working" << std::endl;
  int time = 1;
  // if (argc == 9) {
  //   std::ofstream writeFile(filename, std::ios::app);
  //   writeFile.close();
  // }

  std::fstream file(logFile);
  if (!file.is_open()) {
    std::cout << "Error opening the file!" << std::endl;
    exit(255);
  }

  std::string line;
  while (std::getline(file, line)) {
    // std::cout << line << std::endl;
    std::list<std::string> wordList;
    std::istringstream iss(line);
    std::string word;
    // Split line into words and store each word in the list
    while (iss >> word) {
      wordList.push_back(word);
    }
    auto it = wordList.begin();
    std::advance(it, 1);

    if (std::stoi(*it) > time) {
      time = std::stoi(*it);
    }
  }
  file.close();
  // std::cout << "most recent time is: " << time << std::endl;
  return time;
}

// Tokenizer function to split the string into individual arguments
std::vector<std::string> tokenize(const std::string &line) {
  std::vector<std::string> tokens;
  std::stringstream ss(line);
  ss << "logAppend " << line; // logAppend command must be added to the start
  std::string token;

  // Split by spaces
  while (ss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

std::vector<LogAppendArgs> readBatches(LogAppendArgs &args) {
  std::vector<LogAppendArgs> output;
  std::fstream file(args.batchFile);
  std::string line;

  int count = 0;
  while (std::getline(file, line)) {
    std::vector<std::string> tokens = tokenize(line);
    int argc = tokens.size();

    // Create an array of char* for argv
    std::vector<char *> argv(argc);

    for (int i = 0; i < argc; ++i) {
      argv[i] = &tokens[i][0]; // Convert string to char*
    }
    try {
      LogAppendArgs newArgs(argc, argv.data());

      if (!newArgs.isBatch && fileExistsAndIsReadable(newArgs.logFile)) {
        count++;
        output.push_back(newArgs);
      } else {
        std::cerr << "Skipping bad command in batch: must have readable "
                     "logfile and can not be batch command"
                  << std::endl;
      }

    } catch (const std::exception &e) {
      std::cerr << "Skipping bad command in batch: " << e.what() << std::endl;
    }

    std::cout << "Added " << count << " commands to the queue" << std::endl;
    return output;
  }

  file.close();
  return output;
}

void createOrAppendFile(const std::string &filename, const std::string &line) {
  // Open the file in append mode, create if it doesn't exist
  std::ofstream file(filename, std::ios::app);
  if (!file) {
    std::cerr << "Error opening the file!" << std::endl;
    return;
  }
  file << line << std::endl;
  file.close();
  std::cout << "Line successfully written to the file." << std::endl;
}

void execute(LogAppendArgs args) {}

int main(int argc, char *argv[]) {
  try {
    LogAppendArgs args(argc, argv);
    std::cout << "First round of validation complete" << std::endl;
    args.print();

    std::vector<LogAppendArgs> toExecute = {args};
    if (args.isBatch) {
      auto batch = readBatches(args);
      std::cout << "All Batch arguments read. Executing batch now. ";
      toExecute.insert(toExecute.end(), batch.begin(), batch.end());
    }

    for (int i = 0; i < toExecute.size(); ++i) {
      LogAppendArgs command = toExecute[i];
      try {
        execute(command);
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 255;
  }
}

// file_validation validates the name of the file
// it does not validate if there is any file relates to that name
void file_validation(std::string logFile) {
  // try to open
}

void validate_timestamp(int timestamp) {
  // std::cout << "this line" << std::endl;
  int mostRecentTimestamp = get_most_recent_time("test");
  if (timestamp <= mostRecentTimestamp) {
    std::cerr << "Invalid: Timestamp" << std::endl;
    exit(255);
  }
}
//
// if (argc == 3) {
//   // run the batch command
//   check_command(argc, argv);
//   std::string filename = batch_validation(argv[2]) + ".txt";
//   std::cout << filename << std::endl;
//
//   std::fstream file(filename);
//   std::string line;
//   while (std::getline(file, line)) {
//     process_single_command(line);
//   }
//   file.close();
//
// } else {
//   // run the sinle line command
//   executor(argc, argv);
// }
// }

//
// // check all the options like -T -K (-A or -L)-R (-E or -G)
// void check_command(int argc, char *argv[]) {
//   if (argc == 3) {
//     assert(std::strcmp(argv[1], "-B") == 0);
//     std::cout << "-B check success" << std::endl;
//   }
//   if (argc == 9) {
//     assert(
//         std::strcmp(argv[1], "-T") == 0 && std::strcmp(argv[3], "-K") == 0 &&
//         (std::strcmp(argv[5], "-A") == 0 || std::strcmp(argv[5], "-L") == 0)
//         && (std::strcmp(argv[6], "-E") == 0 || std::strcmp(argv[6], "-G") ==
//         0));
//     std::cout << "Arguments content check successful!" << std::endl;
//     return;
//   }
//   if (argc == 11) {
//     assert(
//         std::strcmp(argv[1], "-T") == 0 && std::strcmp(argv[3], "-K") == 0 &&
//         (std::strcmp(argv[5], "-A") == 0 || std::strcmp(argv[5], "-L") == 0)
//         && (std::strcmp(argv[6], "-E") == 0 || std::strcmp(argv[6], "-G") ==
//         0) && std::strcmp(argv[8], "-R") == 0);
//     std::cout << "Arguments content check successful!" << std::endl;
//     return;
//   }
// }
//
