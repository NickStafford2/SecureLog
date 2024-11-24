// event.cpp
#include <string>

#include "event.h"
#include "gallery.h"
#include "participantType.h"

const char Event::DELIMITER = ';';

void Event::printEvent(std::string s) const {
  std::cout << s << "Timestamp: " << timestamp << ", Person: \"" << person
            << "\", From: " << Gallery::readifyLocation(from_location)
            << ", To: " << Gallery::readifyLocation(to_location)
            << ", Role: " << participantTypeToString(participantType)
            << std::endl;
}
