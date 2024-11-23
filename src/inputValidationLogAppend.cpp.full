#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

#include "inputValidationLogAppend.h"

class LogAppendArgs {
public:
  std::string timestampDetails =
      "-T timestamp: Time the event is recorded. This timestamp is formatted "
      "as the number of seconds since the gallery opened and is a non-negative "
      "integer (ranging from 1 to 1073741823 inclusively). Time should always "
      "increase, invoking logappend with an event at a time that is prior to "
      "the most recent event already recorded is an error.";
  int timestamp = -1;

  std::string tokenDetails =
      "-K token Token used to authenticate the log. This token consists of an "
      "arbitrary-sized string of alphanumeric (a-z, A-Z, and 0-9) characters. "
      "The user supplies this token via the command line when creating a new "
      "log. Once a log is created with a specific token, any subsequent "
      "appends to that log must use the same token. You may assume the token "
      "contains sufficient entropy that it cannot be guessed in a reasonable "
      "amount of time.";
  std::string token;

  std::string employeeDetails =
      "-E employee-name Name of employee. Names are alphabetic characters "
      "(a-z, A-Z) in upper and lower case. Names may not contain spaces. Names "
      "are case sensitive. Employees and guests can have the same name. The "
      "names can be arbitrarily long. More concretely, if you make an "
      "assumption about lengths, and an attacker can successfully land a "
      "correctness, or even confidentiality or integrity attack on the contest "
      "server by using a longer name, this will be considered a valid "
      "attack.";
  std::string employeeName;

  std::string guestDetails =
      "-G guest-name Name of guest. Names are alphabetic characters (a-z, A-Z) "
      "in upper and lower case. Names may not contain spaces. Names are case "
      "sensitive. Employees and guests can have the same name. The names can "
      "be arbitrarily long. More concretely, if you make an assumption about "
      "lengths, and an attacker can successfully land a correctness, or even "
      "confidentiality or integrity attack on the contest server by using a "
      "longer name, this will be considered a valid attack.";
  std::string guestName;

  std::string arrivalDetails =
      "-A Specify that the current event is an arrival; can be used with -E, "
      "-G, and -R. This option can be used to signify the arrival of an "
      "employee or guest to the gallery, or, to a specific room with -R. If -R "
      "is not provided, -A indicates an arrival to the gallery as a whole. No "
      "employee or guest should enter a room without first entering the "
      "gallery. No employee or guest should enter a room without having left a "
      "previous room. Violation of either of these conditions implies "
      "inconsistency with the current log state and should result in logappend "
      "exiting with an error condition.";
  bool isArrival = false;

  std::string leavingDetails =
      "-L Specify that the current event is a departure, can be used with -E, "
      "-G, and -R.This option can be used to signify the departure of an "
      "employee or guest from the gallery, or, from a specific room with -R. "
      "If -R is not provided, -L indicates a deparature from the gallery as a "
      "whole. No employee or guest should leave the gallery without first "
      "leaving the last room they entered. No employee or guest should leave a "
      "room without entering it. Violation of either of these conditions "
      "implies inconsistency with the current log state and should result in "
      "logappend exiting with an error condition.";
  bool isLeaving = false;

  std::string roomDetails =
      "-R room-id Specifies the room ID for an event. Room IDs are "
      "non-negative integer characters with no spaces (ranging from 0 to "
      "1073741823 inclusively). Leading zeros in room IDs should be dropped, "
      "such that 003, 03, and 3 are all equivalent room IDs. A gallery is "
      "composed of multiple rooms. A complete list of the rooms of the gallery "
      "is not available and rooms will only be described when an employee or "
      "guest enters or leaves one. A room cannot be left by an employee or "
      "guest unless that employee or guest has previously entered that room. "
      "An employee or guest may only occupy one room at a time. If a room ID "
      "is not specified, the event is for the entire art gallery.";
  int roomId;

  std::string batchDetails =
      "-B file Specifies a batch file of commands. file contains one or more "
      "command lines, not including the logappend command itself (just its "
      "options); the lines are separated by \n (newlines), but the final "
      "newline in a file is optional. These commands should be processed by "
      "logappend individually, in order. This allows logappend to add data to "
      "the file without forking or re-invoking. Of course, option -B cannot "
      "itself appear in one of these command lines. Commands specified in a "
      "batch file include the log name. If a single line in a batch file is "
      "invalid, print the appropriate error message for that line and continue "
      "processing the rest of the batch file. Here is an example (the last "
      "one).";
  bool isBatch = false;
  std::string batchFile;

  std::string logDetails =
      "log The name of the file containing the event log. The log’s filename "
      "may be specified with a string of alphanumeric characters (including "
      "underscores and periods). If the log does not exist, logappend should "
      "create it. logappend should add data to the log, preserving the history "
      "of the log such that queries from logread can be answered. If the log "
      "file cannot be created due to an invalid name, or any other error, "
      "logappend should print “invalid” and return 255.";
  std::string logFile;

  LogAppendArgs(int argc, char *argv[]) {
    if (argc < 2) {
      std::cerr << "Not enough arguments. Usage: logappend -T <timestamp> -K "
                   "<token> (-E | -G) (-A | "
                   "-L) [-R <room>]\n";
      std::cerr << "       logappend -B <batch-file>\n";
    }
    std::cout << "its working" << std::endl;
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
        if (arg == "-T") {
          if (i + 1 < args.size()) {
            this->timestamp = std::stoi(args[++i]);
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
            this->employeeName = std::stoi(args[++i]);
            name_validation(employeeName);
          } else {
            throw std::invalid_argument("Missing employee name");
          }
        } else if (arg == "-G") {
          if (i + 1 < args.size()) {
            this->guestName = std::stoi(args[++i]);
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
            room_validation();
          } else {
            throw std::invalid_argument("Missing room ID value");
          }
        } else {
          throw std::invalid_argument("Unknown argument: " + arg);
        }
      }
      this->validate();
    }
  }

  // Validate that the arguments are consistent
  bool validate() {
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

    validate_timestamp();
    room_validation();
    return true;
  }

  // file_validation validates the name of the file
  // it does not validate if there is any file relates to that name
  void file_validation() {
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
    for (char c : batchFile) {
      if (!std::isalnum(c)) {
        std::cerr << "Invalid: Batch file" << std::endl;
        exit(255);
      }

      std::fstream file(batchFile);
      if (!file.is_open()) {
        std::cout << "Error opening the batch file!" << std::endl;
        exit(255);
      }
      std::string line;
      int count = 0;
      while (std::getline(file, line)) {
        if (line.empty() || line.find("-B") != std::string::npos) {
          std::cerr << "Invalid command on line: " << count << ") " << line
                    << std::endl;
          return;
        }
        count++;
      }
      file.close();

      // todo. check each line of the batch file and ensure that it has no other
      // -B commands in it
    }
  }
  // get_most_recent_time grabs the most recent time present in that file
  int get_most_recent_time() {
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

  // room_validation validates the room and is under the integer constraints.
  void room_validation() {
    if (roomId < 1 || roomId > 1073741823) {
      std::cerr << "Invalid: Room" << std::endl;
      exit(255);
    }
    // std::cout << "Room validation successful!" << std::endl;
  }

  void name_validation(const std::string name) {
    for (char n : name) {
      if (!std::isalpha(n)) {
        std::cerr << "Invalid: Name" << std::endl;
        exit(255);
      }
    }
    std::cout << "Name is successfully validated" << std::endl;
    return;
  }

  // token_validation validates the secret key
  void token_validation() {
    // maybe make them choose a difficult key
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
    int mostRecentTimestamp = get_most_recent_time();
    // std::cout << "this line" << std::endl;
    if (timestamp <= mostRecentTimestamp || timestamp < 1 ||
        timestamp > 1073741823) {
      std::cerr << "Invalid: Timestamp" << std::endl;
      exit(255);
    }
  }

  void printHelp() {
    std::cout << "Usage: logappend [OPTIONS] <log>\n\n";

    std::cout << "Description:\n";
    std::cout << "logappend is a tool for appending events to a log. It is "
                 "used to record "
                 "the activities of employees and guests in a gallery, "
                 "including their "
                 "arrivals, departures, and movements between rooms. You can "
                 "specify a "
                 "timestamp and authentication token for each event, and the "
                 "tool will either "
                 "create a new log or append to an existing one. The options "
                 "below provide "
                 "various ways to filter and customize the log entries.\n\n";

    std::cout << "Options:\n";
    std::cout
        << "  -T <timestamp>      Time the event is recorded, formatted as the "
           "number of seconds since the gallery opened. Non-negative integer, "
           "increasing order is required.\n";
    std::cout << "  -K <token>          Token used to authenticate log access. "
                 "Alphanumeric.\n";
    std::cout << "  -E <employee-name>   Employee name. Alphabetic characters "
                 "only, case-sensitive.\n";
    std::cout << "  -G <guest-name>      Guest name. Alphabetic characters "
                 "only, case-sensitive.\n";
    std::cout << "  -A                   Indicates an arrival event. Can be "
                 "used with -E, -G, and -R.\n";
    std::cout << "  -L                   Indicates a departure event. Can be "
                 "used with -E, -G, and -R.\n";
    std::cout << "  -R <room-id>         Room ID for the event. Non-negative "
                 "integer, no spaces.\n";
    std::cout << "  <log>                The name of the log file used for "
                 "recording events. "
                 "Alphanumeric, underscores, and periods allowed.\n";
    std::cout << "  -B <file>            Specifies a batch file containing "
                 "multiple commands.\n\n";

    std::cout << "Examples:\n";
    std::cout << "  logappend -T <timestamp> -K <token> (-E <name> | -G "
                 "<name>) (-A | -L) [-R <room-id>] <log>\n";
    std::cout << "  logappend -B <batch-file>\n\n";

    std::cout << "Note:\n";
    std::cout << "  - When using the -R option, the employee or guest must "
                 "enter the gallery first before entering a room.\n";
    std::cout << "  - The -T timestamp must always be greater than the "
                 "previous timestamp to avoid inconsistency.\n";
    std::cout << "  - If the log cannot be created or an invalid name is used, "
                 "the command will print \"invalid\" and return 255.\n";
    std::cout << "  - If a batch file is specified, each command in the file "
                 "will be processed in order. If one command fails, the rest "
                 "will continue to be processed.\n";
  }
};

// // Example usage
// int main(int argc, char* argv[]) {

//     return 0;
// }
// void parseArgs(int argc, char *argv[]) {
//
//
//

// class LogAppendParser {
// public:
//   LogAppendArgs parse(int argc, char *argv[]) {
//     LogAppendArgs args;
//     std::vector<std::string> arguments(argv + 1, argv + argc);
//
//     for (size_t i = 0; i < arguments.size(); ++i) {
//       const std::string &arg = arguments[i];
//
//       if (arg == "-B") {
//         args.isBatch = true;
//         if (i + 1 < arguments.size()) {
//           args.batchFile = arguments[++i];
//         } else {
//           throw std::invalid_argument("Missing batch file argument for -B");
//         }
//         continue;
//       }
//
//       if (!args.isBatch) {
//         if (arg == "-T") {
//           if (i + 1 < arguments.size()) {
//             try {
//               args.timestamp = std::stoi(arguments[++i]);
//               if (args.timestamp < 0) {
//                 throw std::invalid_argument("Timestamp must be
//                 non-negative");
//               }
//             } catch (const std::exception &e) {
//               throw std::invalid_argument("Invalid timestamp format");
//             }
//           } else {
//             throw std::invalid_argument("Missing timestamp value");
//           }
//         } else if (arg == "-K") {
//           if (i + 1 < arguments.size()) {
//             args.token = arguments[++i];
//           } else {
//             throw std::invalid_argument("Missing token value");
//           }
//         } else if (arg == "-E") {
//           args.isEmployee = true;
//         } else if (arg == "-G") {
//           args.isGuest = true;
//         } else if (arg == "-A") {
//           args.isArrival = true;
//         } else if (arg == "-L") {
//           args.isLeaving = true;
//         } else if (arg == "-R") {
//           if (i + 1 < arguments.size()) {
//             args.roomId = arguments[++i];
//           } else {
//             throw std::invalid_argument("Missing room ID value");
//           }
//         } else {
//           throw std::invalid_argument("Unknown argument: " + arg);
//         }
//       }
//     }
//
//     args.validate();
//     return args;
//   }
// };
