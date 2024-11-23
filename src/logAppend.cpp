// logAppend.cpp
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
#include "state.h"

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

      if (!newArgs.isBatch /*&& fileExistsAndIsReadable(newArgs.logFile)*/) {
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

void execute(LogAppendArgs args) {
  int fromLocation = Gallery::ERROR;
  int toLocation = Gallery::ERROR;

  args.print();
  if (args.isArrival) {
    if (args.roomDeclared) {
      toLocation = args.roomId;
      fromLocation = Gallery::GALLERY_ID;
    } else {
      toLocation = Gallery::GALLERY_ID;
      fromLocation = Gallery::UNKNOWN;
    }
  } else if (args.isLeaving) {
    if (args.roomDeclared) {
      toLocation = Gallery::GALLERY_ID;
      fromLocation = args.roomId;
    } else {
      toLocation = Gallery::UNKNOWN;
      fromLocation = Gallery::GALLERY_ID;
    }
  }
  if (fromLocation == Gallery::ERROR || toLocation == Gallery::ERROR) {

    std::cout << fromLocation << toLocation << std::endl;
    throw std::runtime_error("Program error. Invalid State");
  }

  Event event(args.timestamp, args.name, fromLocation, toLocation,
              args.participantType);
  // std::cout << event << std::endl;
  event.printEvent();
  Gallery gallery;
  gallery.move(event);
  gallery.saveToFile("gallery_data.txt", args.token);
  // Load the gallery data back from the file
  try {
    Gallery loadedGallery =
        Gallery::loadFromFile("gallery_data.txt", args.token);
    std::cout << "Success: " << std::endl;
    loadedGallery.printGallery();
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}

int main(int argc, char *argv[]) {
  try {
    LogAppendArgs args(argc, argv);
    std::cout << "First round of validation complete" << std::endl;
    args.print();

    std::vector<LogAppendArgs> toExecute = {};
    if (args.isBatch) {
      auto batch = readBatches(args);
      std::cout << "All Batch arguments read. Executing batch now. ";
      toExecute.insert(toExecute.end(), batch.begin(), batch.end());
    } else {
      toExecute.push_back(args);
    }

    for (size_t i = 0; i < toExecute.size(); ++i) {
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