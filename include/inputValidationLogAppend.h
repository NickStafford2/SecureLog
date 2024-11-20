#pragma once

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

void printHelp();
void check_command(int argc, char *argv[]);
int parseArgs(int argc, char *argv[]);

class LogAppendArgs {
public:
  std::string timestampDetails;
  int timestamp;
  std::string tokenDetails;
  std::string token;
  std::string employeeDetails;
  std::string employeeName;
  std::string guestDetails;
  std::string arrivalDetails;
  bool isArrival;
  std::string leavingDetails;
  bool isLeaving;
  std::string roomDetails;
  int roomId;

  std::string batchDetails;
  bool isBatch;
  std::string batchFile;

  std::string logDetails;
  std::string logFile;

  LogAppendArgs(int argc, char *argv[]);
  bool validate();
  void file_validation();
  void batch_validation();
  int get_most_recent_time();
  void room_validation();
  void name_validation(const std::string name);
  void token_validation();
  void validate_timestamp();
  void printHelp();
};
