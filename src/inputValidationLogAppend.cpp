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
