// gallery.cpp
#include <iostream>
#include <string>

#include "gallery.h"
#include "participantType.h"

const std::string Gallery::LOG_DIR = "logs/";

std::string Gallery::readifyLocation(int location) {
  switch (location) {
  case Gallery::UNKNOWN:
    return "Outside";
  case Gallery::GALLERY_ID:
    return "Gallery";
  case Gallery::ERROR:
    return "Error";
  default:
    return "Room " + std::to_string(location);
  }
}

void Gallery::printTimeFor(std::string name, ParticipantType participantType) {
  std::cout << "todo" << std ::endl;
  if (participantType == ParticipantType::GUEST) {
    if (guests.find(name) != guests.end()) {
      std::cerr << "guest does not exist" << std::endl;
    }
  } else {
    if (employees.find(name) != employees.end()) {
      std::cerr << "employee does not exist" << std::endl;
    }
  }
}
