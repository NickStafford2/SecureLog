#include <iostream>
#include <sstream>
#include <vector>

#include "utils.h"

std::string intArrayToString(const std::vector<int> &arr) {
  std::ostringstream
      oss; // Create a string stream for building the output string
  for (size_t i = 0; i < arr.size(); ++i) {
    oss << arr[i];             // Add each integer to the stream
    if (i != arr.size() - 1) { // If it's not the last element, add a comma
      oss << ", ";
    }
  }
  return oss.str(); // Return the final string
}
