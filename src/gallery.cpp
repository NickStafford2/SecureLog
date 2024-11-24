// gallery.cpp
#include <iostream>
#include <string>

#include "gallery.h"

const std::string Gallery::LOG_DIR = "logs/";

std::string Gallery::readifyLocation(int location) {
  return std::to_string(location);
}
