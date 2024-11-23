// inputValidationLogAppend.cpp
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
// #include "utils.h"

void LogAppendArgs::printHelp() const {
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
void LogAppendArgs::print() const {
  std::cout << "LogAppendArgs {" << std::endl;

  // Print timestamp
  std::cout << "  timestamp: "
            << (timestamp == 0 ? "N/A" : std::to_string(timestamp))
            << std::endl;

  // Print token
  std::cout << "  token: " << (token.empty() ? "N/A" : token) << std::endl;

  // Print employeeName
  std::cout << "  employeeName: "
            << (employeeName.empty() ? "N/A" : employeeName) << std::endl;

  // Print guestName
  std::cout << "  guestName: " << (guestName.empty() ? "N/A" : guestName)
            << std::endl;

  // Print isArrival
  std::cout << "  isArrival: " << (isArrival ? "true" : "false") << std::endl;

  // Print isLeaving
  std::cout << "  isLeaving: " << (isLeaving ? "true" : "false") << std::endl;

  // Print roomId
  std::cout << "  roomId: " << (roomId == 0 ? "N/A" : std::to_string(roomId))
            << std::endl;

  // Print isBatch
  std::cout << "  isBatch: " << (isBatch ? "true" : "false") << std::endl;

  // Print batchFile
  std::cout << "  batchFile: " << (batchFile.empty() ? "N/A" : batchFile)
            << std::endl;

  // Print logFile
  std::cout << "  logFile: " << (logFile.empty() ? "N/A" : logFile)
            << std::endl;

  // Print logFile
  // std::cout << "  ParticipantType: " <<
  // participantTypeToString(participantType)
  //           << std::endl;

  // Print name
  std::cout << "  name: " << name << std::endl;

  std::cout << "}" << std::endl;
}

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
