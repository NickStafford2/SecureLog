// participantType.h
#pragma once

#include <string>

enum class ParticipantType { EMPLOYEE, GUEST };

std::string participantTypeToString(ParticipantType participantType);
