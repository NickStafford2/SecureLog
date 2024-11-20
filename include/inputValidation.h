// inputValidation.h
#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include <fstream>
#include <iostream>
#include <string>

bool isNumeric(const std::string &str);

bool fileExistsAndIsReadable(const std::string &filename) {
  std::ifstream file(filename); // Try to open the file for reading

  // Check if the file was opened successfully
  if (file.is_open()) {
    file.close(); // Close the file as we only needed to check for existence and
                  // readability
    return true;
  }
  return false;
}
#endif
