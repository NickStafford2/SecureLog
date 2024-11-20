// inputValidation.cpp
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
