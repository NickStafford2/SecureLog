#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

#include "inputValidationLogAppend.h"

void printHelp() {
  std::cout << "Usage: logread [OPTIONS] <log>\n\n";

  std::cout << "Description:\n";
  std::cout
      << "logread is a tool for viewing and analyzing log data, specifically "
         "for tracking the activities of employees and guests in a gallery. "
      << "It provides functionality to check various states, list rooms, "
         "calculate time spent, and more. Use the options below to filter and "
         "customize the log output.\n\n";

  std::cout << "Options:\n";
  std::cout << "  -K <token>          Token used to authenticate log access. "
               "Alphanumeric.\n";
  std::cout << "  -S                  Print the current state of the log to "
               "stdout.\n";
  std::cout << "  -R                  List all rooms entered by an employee or "
               "guest. "
            << "If specified, either -E or -G must also be specified.\n";
  std::cout << "  -T                  Shows the total time spent in the "
               "gallery by an employee or guest. "
            << "If they are still in the gallery, the time since their arrival "
               "is provided. Optional.\n";
  std::cout << "  -I                  Prints the rooms that were occupied by "
               "all the specified employees and guests "
            << "at the same time over the complete history of the gallery.\n";
  std::cout << "  -E <name>           Employee name. May be specified multiple "
               "times when used with -I.\n";
  std::cout << "  -G <name>           Guest name. May be specified multiple "
               "times when used with -I.\n";
  std::cout << "  <log>               The name of the log file used for "
               "recording events. Alphanumeric, underscores, and periods.\n\n";

  std::cout << "Examples:\n";
  std::cout << "  logread -K <token> -T (-E <name> | -G <name>) <log>\n";
  std::cout << "  logread -K <token> -I (-E <name> | -G <name>) [(-E <name> | "
               "-G <name>) ...] <log>\n\n";

  std::cout << "Note:\n";
  std::cout
      << "  - When using the -R option, either -E or -G must be specified.\n";
  std::cout << "  - The -T option is optional, and if used, it will provide "
               "the time spent in the gallery by the person(s) specified.\n";
}

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
  std::string roomId;

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
        } else {
          throw std::invalid_argument("Missing batch file argument for -B");
        }
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
          } else {
            throw std::invalid_argument("Missing token value");
          }
        } else if (arg == "-E") {
          if (i + 1 < args.size()) {
            this->employeeName = std::stoi(args[++i]);
          } else {
            throw std::invalid_argument("Missing employee name");
          }
        } else if (arg == "-G") {
          if (i + 1 < args.size()) {
            this->guestName = std::stoi(args[++i]);
          } else {
            throw std::invalid_argument("Missing guest name");
          }
        } else if (arg == "-A") {
          this->isArrival = true;
        } else if (arg == "-L") {
          this->isLeaving = true;
        } else if (arg == "-R") {
          if (i + 1 < args.size()) {
            this->roomId = args[++i];
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
  bool validate() const {
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
    }
    return true;
  }
};

int parseArgs(int argc, char *argv[]) {
  std::cout << "test" << std::endl;
  if (argc < 2) {
    std::cerr << "Usage: logappend -T <timestamp> -K <token> (-E | -G) (-A | "
                 "-L) [-R <room>]\n";
    std::cerr << "       logappend -B <batch-file>\n";
    return 1;
  }

  try {
    LogAppendArgs args(argc, argv);

    // LogAppendParser parser;
    // LogAppendArgs args = parser.parse(argc, argv);
    //
    // // Use the parsed arguments
    // if (args.isBatch) {
    //     std::cout << "Processing batch file: " << args.batchFile <<
    //     std::endl;
    // } else {
    //     std::cout << "Timestamp: " << args.timestamp << std::endl;
    //     std::cout << "Token: " << args.token << std::endl;
    //     std::cout << "Type: " << (args.isEmployee ? "Employee" : "Guest") <<
    //     std::endl; std::cout << "Action: " << (args.isArrival ? "Arrival" :
    //     "Leaving") << std::endl; if (!args.roomId.empty()) {
    //         std::cout << "Room: " << args.roomId << std::endl;
    //     }
    // }
    //
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 1;
}

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
