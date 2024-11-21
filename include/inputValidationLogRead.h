#pragma once
#pragma once
#include <cassert>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

#include "utils.h"
class LogReadArgs {
public:
  std::string tokenDetails =
      "-K token Token used to authenticate the log. This token consists of an "
      "arbitrary sized string of alphanumeric characters and will be the same "
      "between executions of logappend and logread. You may assume the token "
      "contains sufficient entropy so that it cannot be guessed in a "
      "reasonable amount of time. If the log cannot be authenticated with the "
      "token (i.e., it is not the same token that was used to create the "
      "file), then \"integrity violation\" should be printed to stdout and 255 "
      "should be returned.";
  std::string token = "";

  std::string printDetails =
      "-S Print the current state of the log to stdout. The state should be "
      "printed to stdout on at least two lines, with lines separated by the \n "
      "(newline) character. The first line should be a comma-separated list of "
      "employees currently in the gallery. The second line should be a "
      "comma-separated list of guests currently in the gallery. The remaining "
      "lines should provide room-by-room information indicating which guest or "
      "employee is in which room. Each line should begin with a room ID, "
      "printed as a decimal integer, followed by a colon, followed by a space, "
      "followed by a comma-separated list of guests and employees. Room IDs "
      "should be printed in ascending integer order, all guest/employee names "
      "should be printed in ascending lexicographic string order.";
  std::string printSymbol = "-S";

  std::string roomDetails =
      "-R Print the current state of the log to stdout. The state should be "
      "printed to stdout on at least two lines, with lines separated by the \n "
      "(newline) character. The first line should be a comma-separated list of "
      "employees currently in the gallery. The second line should be a "
      "comma-separated list of guests currently in the gallery. The remaining "
      "lines should provide room-by-room information indicating which guest or "
      "employee is in which room. Each line should begin with a room ID, "
      "printed as a decimal integer, followed by a colon, followed by a space, "
      "followed by a comma-separated list of guests and employees. Room IDs "
      "should be printed in ascending integer order, all guest/employee names "
      "should be printed in ascending lexicographic string order.";
  std::string roomSymbol = "-S";

  std::string timeDetails =
      "-T Gives the total time spent in the gallery by an employee or guest. "
      "If the employee or guest is still in the gallery, print the time spent "
      "so far. Output in an integer on a single line. This feature is "
      "optional. If the specified employee or guest does not appear in the "
      "gallery, then nothing is printed.";
  int time = -1;

  std::string IDetails =
      "-I Prints the rooms, as a comma-separated list of room IDs, that were "
      "occupied by all the specified employees and guests at the same time "
      "over the complete history of the gallery. Room IDs should be printed in "
      "ascending numerical order. This feature is optional. If a specified "
      "employee or guest does not appear in the gallery, it is ignored. If no "
      "room ever contained all of the specified persons, then nothing is "
      "printed.";
  std::string ISymbol = "-I";

  std::string EmployeeDetails =
      "-E Employee name. May be specified multiple times when used with -I.";
  std::string EmployeeSymbol = "-E";

  std::string GuestDetails =
      "-G Guest name. May be specified multiple times when used with -I.";
  std::string GSymbol = "-G";

  std::string logDetails =
      "log The name of the file log used for recording events. The filename "
      "may be specified with a string of alphanumeric characters (including "
      "underscores and periods).";
  std::string logFile = "";

  ParticipantType
      participantType; // Whether the event is for an employee or guest
  std::string name = "";

  LogReadArgs(int argc, char *argv[]) {

    std::cout << "\nCreateing LogAppendArgs from: " << argc << std::endl;
    // Print all arguments in argv
    std::cout << "Arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
      std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }

    if (argc < 2) {
      std::cerr << "Not enough arguments. Usage: logappend -T <timestamp> -K "
                   "<token> (-E | -G) (-A | "
                   "-L) [-R <room>]\n";
      std::cerr << "       logappend -B <batch-file>\n";
      throw std::invalid_argument("");
    }

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
      args.push_back(argv[i]);
    }

    for (size_t i = 0; i < args.size(); ++i) {
      const std::string &arg = args[i];

      if (arg == "-B") {
        this->isBatch = true;
        if (i + 1 < args.size()) {
          this->batchFile = args[++i];
          this->batch_validation();
        } else {
          throw std::invalid_argument("Missing batch file argument for -B");
        }
      } else if (arg == "--help") {
        printHelp();
        throw std::invalid_argument("");
      }
    }
    if (this->isBatch && args.size() > 3) {
      throw std::invalid_argument("Too many arguments for -B");
    }
    if (!this->isBatch) {
      for (size_t i = 0; i < args.size(); ++i) {
        const std::string &arg = args[i];
        // std::cout << arg << std::endl;
        if (arg == "-T") {
          if (i + 1 < args.size()) {
            try {
              this->timestamp = std::stoi(args[++i]);
              validate_timestamp();
            } catch (const std::exception e) {
              throw std::invalid_argument("timestamp must be a number");
            }
          } else {
            throw std::invalid_argument("Missing timestamp value");
          }
        } else if (arg == "-K") {
          if (i + 1 < args.size()) {
            this->token = args[++i];
            token_validation();
          } else {
            throw std::invalid_argument("Missing token value");
          }
        } else if (arg == "-E") {
          if (i + 1 < args.size()) {
            this->employeeName = args[++i];
            name_validation(employeeName);
          } else {
            throw std::invalid_argument("Missing employee name");
          }
        } else if (arg == "-G") {
          if (i + 1 < args.size()) {
            this->guestName = args[++i];
            name_validation(guestName);
          } else {
            throw std::invalid_argument("Missing guest name");
          }
        } else if (arg == "-A") {
          this->isArrival = true;
        } else if (arg == "-L") {
          this->isLeaving = true;
        } else if (arg == "-R") {
          if (i + 1 < args.size()) {
            this->roomId = std::stoi(args[++i]);
            this->roomDeclared = true;
            room_validation();
          } else {
            throw std::invalid_argument("Missing room ID value");
          }
        } else if (i == args.size() - 1) {
          // logFile is the last item
          this->logFile = args[i];
          file_validation();
        } else {
          throw std::invalid_argument("Unknown argument: " + arg);
        }
      }
      this->validateNonBatch();
    }

    std::cout << "Successfully Created LogAppendArgs" << std::endl;
    this->print();
  }

  // Validate that the arguments are consistent
  void validateNonBatch() {
    // Basic validation rules
    if (timestamp < 0 && !isBatch) {
      throw std::invalid_argument(
          "Timestamp is required for non-batch operations");
    }
    if (token.empty()) {
      throw std::invalid_argument("Authentication token is required");
    }
    if (!isBatch) {
      if (employeeName.empty() && guestName.empty()) {
        throw std::invalid_argument(
            "Must specify either employee (-E) or guest (-G)");
      }
      if (!employeeName.empty() && !guestName.empty()) {
        throw std::invalid_argument("Cannot be both employee and guest");
      }
      if (!isArrival && !isLeaving) {
        throw std::invalid_argument(
            "Must specify either arrival (-A) or leaving (-L)");
      }
      if (isArrival && isLeaving) {
        throw std::invalid_argument("Cannot be both arriving and leaving");
      }
      file_validation();
    }

    // std::cout << "sssss" << guestName << std::endl;
    // std::cout << "iiiii" << employeeName << std::endl;
    if (guestName.empty()) {
      this->participantType = ParticipantType::EMPLOYEE;
      this->name = employeeName;
    } else {
      this->participantType = ParticipantType::GUEST;
      this->name = guestName;
    }
    // std::cout << "kkkk" << name << std::endl;
  }

  // file_validation validates the name of the file
  // it does not validate if there is any file relates to that name
  void file_validation() {
    if (logFile.empty()) {
      throw std::invalid_argument("Invalid Filename");
    }
    for (char n : logFile) {
      if (!std::isalnum(n) && n != '_' && n != '.') {
        std::cerr << "Invalid: Filename " << std::endl;
        exit(255);
      }
    }
    // std::cout << "Filename is successfully validated" << std::endl;
  }

  // batch_validation validates if the batch filename is correct
  // if does not check if there is any file or not
  void batch_validation() {
    if (batchFile.empty()) {
      throw std::invalid_argument("No Batch File");
    }
    for (char c : batchFile) {
      if (!std::isalnum(c) && c != '.' && c != '_') {
        std::cerr << "Invalid batch file name" << std::endl;
        exit(255);
      }
    }
  }
  // room_validation validates the room and is under the integer constraints.
  void room_validation() {
    if (roomId < 0 || roomId >= 1073741823) {
      std::cerr << "Invalid: Room" << std::endl;
      exit(255);
    }
    // std::cout << "Room validation successful!" << std::endl;
  }

  void name_validation(const std::string name) {
    if (name.empty()) {
      throw std::invalid_argument("Must enter a name.");
    }
    for (char n : name) {
      if (!std::isalpha(n)) {
        std::cerr << "Invalid: Name" << std::endl;
        exit(255);
      }
    }
    // std::cout << "Name is successfully validated" << std::endl;
    return;
  }

  // token_validation validates the secret key
  void token_validation() {
    // maybe make them choose a difficult key
    if (token.empty()) {
      throw std::invalid_argument("Must enter a token.");
    }
    for (char c : token) {
      if (!std::isalnum(c)) {
        std::cerr << "Invalid: Token" << std::endl;
        exit(255);
      }
    }
    return;
  }

  // validate_timestamp validates the time constraints.
  void validate_timestamp() {
    // std::cout << "this line" << std::endl;
    if (timestamp < 1 || timestamp > 1073741823) {
      std::cerr << "Invalid: Timestamp" << std::endl;
      exit(255);
    }
  }

  void printHelp() const;
  void print() const;
};
