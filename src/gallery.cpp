// gallery.cpp
#include <iostream>
#include <string>

#include "gallery.h"

const std::string Gallery::LOG_DIR = "logs/";

std::string Gallery::readifyLocation(int location) {
  switch (location) {
  case Gallery::UNKNOWN:
    return "Unknown";
  case Gallery::GALLERY_ID:
    return "Gallery";
  case Gallery::ERROR:
    return "Error";
  default:
    return std::to_string(location);
  }
}
