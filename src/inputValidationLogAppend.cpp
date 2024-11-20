#include "inputValidationLogAppend.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

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
  /* -T timestamp Time the event is recorded. This timestamp is formatted as the
   * number of seconds since the gallery opened and is a non-negative integer
   * (ranging from 1 to 1073741823 inclusively). Time should always increase,
   * invoking logappend with an event at a time that is prior to the most recent
   * event already recorded is an error.
   */
  int timestamp = -1;
  /* -K token Token used to authenticate the log. This token consists of an
   * arbitrary- sized string of alphanumeric (a-z, A-Z, and 0-9) characters. The
   * user supplies this token via the command line when creating a new log. Once
   * a log is created with a specific token, any subsequent appends to that log
   * must use the same token. You may assume the token contains sufficient
   * entropy that it cannot be guessed in a reasonable amount of time.
   */
  std::string token;
  bool isEmployee = false;
  bool isGuest = false;
  bool isArrival = false;
  bool isLeaving = false;
  std::string roomId;
  bool isBatch = false;
  std::string batchFile;

  LogAppendArgs(int argc, char *argv[]) {
    std::cout << "its working" << std::endl;
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
      args.push_back(argv[i]);
    }

    for (size_t i = 0; i < arguments.size(); ++i) {
      const std::string &arg = arguments[i];

      if (arg == "-B") {
        this.isBatch = true;
        if (i + 1 < arguments.size()) {
          this.batchFile = arguments[++i];
        } else {
          throw std::invalid_argument("Missing batch file argument for -B");
        }
      }
    }
    if (this.isBatch && args.size() > 3) {
      throw std::invalid_argument("Too many arguments for -B");
    }
    if (!this.isBatch) {
      for (size_t i = 0; i < arguments.size(); ++i) {
        if (arg == "-T") {
          if (i + 1 < arguments.size()) {
            try {
              this.timestamp = std::stoi(arguments[++i]);
              if (this.timestamp < 0) {
                throw std::invalid_argument("Timestamp must be non-negative");
              }
            } catch (const std::exception &e) {
              throw std::invalid_argument("Invalid timestamp format");
            }
          } else {
            throw std::invalid_argument("Missing timestamp value");
          }
        } else if (arg == "-K") {
          if (i + 1 < arguments.size()) {
            this.token = arguments[++i];
          } else {
            throw std::invalid_argument("Missing token value");
          }
        } else if (arg == "-E") {
          this.isEmployee = true;
        } else if (arg == "-G") {
          this.isGuest = true;
        } else if (arg == "-A") {
          this.isArrival = true;
        } else if (arg == "-L") {
          this.isLeaving = true;
        } else if (arg == "-R") {
          if (i + 1 < args.size()) {
            this.roomId = args[++i];
          } else {
            throw std::invalid_argument("Missing room ID value");
          }
        } else {
          throw std::invalid_argument("Unknown argument: " + arg);
        }
      }
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
      if (!isEmployee && !isGuest) {
        throw std::invalid_argument(
            "Must specify either employee (-E) or guest (-G)");
      }
      if (isEmployee && isGuest) {
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
// }

class LogAppendParser {
public:
  LogAppendArgs parse(int argc, char *argv[]) {
    LogAppendArgs args;
    std::vector<std::string> arguments(argv + 1, argv + argc);

    for (size_t i = 0; i < arguments.size(); ++i) {
      const std::string &arg = arguments[i];

      if (arg == "-B") {
        args.isBatch = true;
        if (i + 1 < arguments.size()) {
          args.batchFile = arguments[++i];
        } else {
          throw std::invalid_argument("Missing batch file argument for -B");
        }
        continue;
      }

      if (!args.isBatch) {
        if (arg == "-T") {
          if (i + 1 < arguments.size()) {
            try {
              args.timestamp = std::stoi(arguments[++i]);
              if (args.timestamp < 0) {
                throw std::invalid_argument("Timestamp must be non-negative");
              }
            } catch (const std::exception &e) {
              throw std::invalid_argument("Invalid timestamp format");
            }
          } else {
            throw std::invalid_argument("Missing timestamp value");
          }
        } else if (arg == "-K") {
          if (i + 1 < arguments.size()) {
            args.token = arguments[++i];
          } else {
            throw std::invalid_argument("Missing token value");
          }
        } else if (arg == "-E") {
          args.isEmployee = true;
        } else if (arg == "-G") {
          args.isGuest = true;
        } else if (arg == "-A") {
          args.isArrival = true;
        } else if (arg == "-L") {
          args.isLeaving = true;
        } else if (arg == "-R") {
          if (i + 1 < arguments.size()) {
            args.roomId = arguments[++i];
          } else {
            throw std::invalid_argument("Missing room ID value");
          }
        } else {
          throw std::invalid_argument("Unknown argument: " + arg);
        }
      }
    }

    args.validate();
    return args;
  }
};
