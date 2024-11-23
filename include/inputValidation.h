// inputValidation.h
#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H
#include <fstream>
#include <iostream>
#include <string>

bool isNumeric(const std::string &str);

bool fileExistsAndIsReadable(const std::string &filename);
#endif
