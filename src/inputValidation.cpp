// inputValidation.cpp
#include <fstream>
#include <iostream>
#include <string>

#include "inputValidation.h"
// Function definition for isNumeric
bool isNumeric(const std::string &str) {
  for (char c : str) {
    if (!std::isdigit(c))
      return false;
  }
  return true;
}

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
