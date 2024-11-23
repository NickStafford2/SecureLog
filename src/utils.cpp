// utils.cpp
#include <string>

#include "utils.h"

std::string participantTypeToString(ParticipantType participantType) {
  switch (participantType) {
  case ParticipantType::EMPLOYEE:
    return "EMPLOYEE";
  case ParticipantType::GUEST:
    return "GUEST";
  default:
    return "UNKNOWN";
  }
}
