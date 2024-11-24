// logReadArgs.cpp
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

#include "logReadArgs.h"

void printHelp() {
  std::cout << "Usage: logread [OPTIONS] <log>\n\n";

  std::cout << "Description:\n";
  std::cout
      << "logread is a tool for viewing and analyzing log data, specifically "
         "for tracking the activities of employees and guests in a gallery. "
      << "It provides functionality to check various states, list rooms, "
         "calculate time spent, and more. Use the options below to filter "
         "and "
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
void LogReadArgs::print() const {
  std::cout << "LogAppendArgs {" << std::endl;

  // Print token
  std::cout << "  token: " << (token.empty() ? "N/A" : token) << std::endl;

  // Print employeeName
  std::cout << "  employeeName: "
            << (employeeName.empty() ? "N/A" : employeeName) << std::endl;

  // Print guestName
  std::cout << "  guestName: " << (guestName.empty() ? "N/A" : guestName)
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
