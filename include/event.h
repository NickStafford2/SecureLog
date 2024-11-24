#include <iostream>
#include <sstream>
#include <string>

#include "participantType.h"

class Event {
public:
  int timestamp;
  std::string person;
  int from_location;
  int to_location;
  ParticipantType participantType;

  static const char DELIMITER;

  Event(int ts, const std::string &p, int from, int to, ParticipantType type)
      : timestamp(ts), person(p), from_location(from), to_location(to),
        participantType(type) {}

  void printEvent(std::string s = "\t  ") const {
    std::cout << s << "Timestamp: " << timestamp << ", Person: \"" << person
              << "\", From Location: " << from_location
              << ", To Location: " << to_location << ", Type: "
              << (participantType == ParticipantType::EMPLOYEE ? "Employee"
                                                               : "Guest")
              << std::endl;
  }

  std::string serialize() const {
    std::stringstream ss;
    ss << timestamp << DELIMITER << person << DELIMITER << from_location
       << DELIMITER << to_location << DELIMITER
       << static_cast<int>(participantType);
    return ss.str();
  }

  static Event deserialize(const std::string &data) {
    std::stringstream ss(data);
    int ts, from, to, typeInt;
    std::string person;
    std::string temp;

    std::getline(ss, temp, DELIMITER);
    ts = std::stoi(temp);

    std::getline(ss, person, DELIMITER);
    std::getline(ss, temp, DELIMITER);
    from = std::stoi(temp);

    std::getline(ss, temp, DELIMITER);
    to = std::stoi(temp);

    std::getline(ss, temp);
    typeInt = std::stoi(temp);

    // Return a newly constructed Event object using the parsed data
    return Event(ts, person, from, to, static_cast<ParticipantType>(typeInt));
  }
};
