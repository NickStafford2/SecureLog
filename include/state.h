#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// Enum to represent the participant type (Employee or Guest)
enum class ParticipantType { EMPLOYEE, GUEST };

class Event {
public:
  int timestamp;
  std::string person;
  int from_location;
  int to_location;
  ParticipantType
      participantType; // Whether the event is for an employee or guest

  // Constructor to initialize the event
  Event(int ts, const std::string &p, int from, int to, ParticipantType type)
      : timestamp(ts), person(p), from_location(from), to_location(to),
        participantType(type) {}

  // Method to print the event details
  void printEvent() const {
    std::cout << "Timestamp: " << timestamp << ", Person: " << person
              << ", From Location: " << from_location
              << ", To Location: " << to_location << ", Type: "
              << (participantType == ParticipantType::EMPLOYEE ? "Employee"
                                                               : "Guest")
              << std::endl;
  }
};

class Gallery {
private:
  // Employees and Guests as key-value pairs where the key is the name and the
  // value is the room ID
  std::unordered_map<std::string, int> employees;
  std::unordered_map<std::string, int> guests;
  std::vector<Event> events;

  int mostRecentTimestamp = 0; // To keep track of the most recent timestamp

public:
  static const int GALLERY_ID = -1;
  static const int UNKNOWN = -2;
  Gallery() = default;

  int getEmployeeRoom(const std::string &employee) const {
    if (employees.find(employee) != employees.end()) {
      return employees.at(employee);
    }
    return UNKNOWN;
  }

  int getGuestRoom(const std::string &guest) const {
    if (guests.find(guest) != guests.end()) {
      return guests.at(guest);
    }
    return UNKNOWN;
  }
  // Validate timestamp to be larger than the most recent one
  void validateTimestamp(int timestamp) {
    if (timestamp <= mostRecentTimestamp) {
      throw std::invalid_argument(
          "Timestamp must be larger than the most recent timestamp.");
    }
    mostRecentTimestamp = timestamp; // Update the most recent timestamp
  }
  // Move a person based on an event
  void move(const Event &event) {
    validateTimestamp(event.timestamp);

    int currentRoom = UNKNOWN;

    // If the event is for an employee, get their room, else for a guest
    if (event.participantType == ParticipantType::EMPLOYEE) {
      currentRoom = getEmployeeRoom(event.person);
    } else if (event.participantType == ParticipantType::GUEST) {
      currentRoom = getGuestRoom(event.person);
    }

    // If the person is not in the 'from_location' room, throw an error
    if (currentRoom != event.from_location) {
      throw std::runtime_error("Error: " + event.person +
                               " is not in the from_location.");
    }

    // Add the event to the list
    events.push_back(event);

    // Change the person's room ID to the new room
    if (event.participantType == ParticipantType::EMPLOYEE) {
      employees[event.person] = event.to_location;
    } else if (event.participantType == ParticipantType::GUEST) {
      guests[event.person] = event.to_location;
    }
  }

  // Print the employees list
  void printEmployees() const {
    std::cout << "Employees: \n";
    for (const auto &entry : employees) {
      std::cout << "  " << entry.first << " (Room ID: " << entry.second << ")"
                << std::endl;
    }
  }

  // Print the guests list
  void printGuests() const {
    std::cout << "Guests: \n";
    for (const auto &entry : guests) {
      std::cout << "  " << entry.first << " (Room ID: " << entry.second << ")"
                << std::endl;
    }
  }

  // Print all events
  void printEvents() const {
    std::cout << "Events: \n";
    for (const auto &event : events) {
      event.printEvent();
    }
  }
};
