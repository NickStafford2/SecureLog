// inputValidation.cpp
#include "inputValidation.h"
#include <string>

// Function definition for isNumeric
bool isNumeric(const std::string &str) {
    for (char c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

