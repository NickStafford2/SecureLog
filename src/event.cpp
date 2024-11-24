// event.cpp
#include <string>

#include "event.h"
#include "gallery.h"

const char Event::DELIMITER = ';';

void Event::printEvent(std::string s) const {
  std::cout << s << "Timestamp: " << timestamp << ", Person: \"" << person
            << "\", From Location: " << Gallery::readifyLocation(from_location)
            << ", To Location: " << Gallery::readifyLocation(to_location)
            << ", Type: "
            << (participantType == ParticipantType::EMPLOYEE ? "Employee"
                                                             : "Guest")
            << std::endl;
}
