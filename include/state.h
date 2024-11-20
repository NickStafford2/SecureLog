#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils.h"
// Enum to represent the participant type (Employee or Guest)

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
  // Serialize event to string
  std::string serialize() const {
    std::stringstream ss;
    ss << timestamp << "," << person << "," << from_location << ","
       << to_location << "," << static_cast<int>(participantType);
    return ss.str();
  }

  // Deserialize string to Event
  static Event deserialize(const std::string &data) {
    std::stringstream ss(data);
    int ts, from, to, typeInt;
    std::string person;
    char comma;

    ss >> ts >> comma >> person >> comma >> from >> comma >> to >> comma >>
        typeInt;

    // Create and return the Event object
    return Event(ts, person, from, to, static_cast<ParticipantType>(typeInt));
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
  static const int ERROR = -3;
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

    if (event.to_location != Gallery::GALLERY_ID &&
        event.to_location != Gallery::UNKNOWN &&
        currentRoom != Gallery::GALLERY_ID) {

      throw std::runtime_error("No employee or guest should enter a room "
                               "without first entering the gallery.");
    }

    // Add the event to the list
    events.push_back(event);

    // Change the person's room ID to the new room
    if (event.participantType == ParticipantType::EMPLOYEE) {
      employees[event.person] = event.to_location;
    } else if (event.participantType == ParticipantType::GUEST) {
      guests[event.person] = event.to_location;
    }
    std::cout << "Move Successfull" << std::endl;
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

  // Function to print out the gallery's current state
  void printGallery() const {
    std::cout << "Employees: \n";
    for (const auto &entry : employees) {
      std::cout << "  " << entry.first << " (Room ID: " << entry.second << ")"
                << std::endl;
    }

    std::cout << "Guests: \n";
    for (const auto &entry : guests) {
      std::cout << "  " << entry.first << " (Room ID: " << entry.second << ")"
                << std::endl;
    }

    std::cout << "Events: \n";
    for (const auto &event : events) {
      event.printEvent();
    }
  }
  // Serialize gallery to string
  std::string serialize() const {
    std::stringstream ss;

    // Serialize employees
    ss << "Employees:\n";
    for (const auto &entry : employees) {
      ss << entry.first << "," << entry.second << "\n";
    }

    // Serialize guests
    ss << "Guests:\n";
    for (const auto &entry : guests) {
      ss << entry.first << "," << entry.second << "\n";
    }

    // Serialize events
    ss << "Events:\n";
    for (const auto &event : events) {
      ss << event.serialize() << "\n";
    }

    return ss.str();
  }

  // Deserialize string to Gallery
  static Gallery deserialize(const std::string &data) {
    std::stringstream ss(data);
    std::string line;
    Gallery gallery;

    // Read employees
    while (std::getline(ss, line) && line != "Guests:") {
      if (line.empty() || line == "Employees:")
        continue;

      std::stringstream empStream(line);
      std::string name;
      int room;
      char comma;

      std::getline(empStream, name, ',');
      empStream >> room;
      gallery.employees[name] = room;
    }

    // Read guests
    while (std::getline(ss, line) && line != "Events:") {
      if (line.empty() || line == "Guests:")
        continue;

      std::stringstream guestStream(line);
      std::string name;
      int room;
      char comma;

      std::getline(guestStream, name, ',');
      guestStream >> room;
      gallery.guests[name] = room;
    }

    // Read events
    while (std::getline(ss, line)) {
      if (!line.empty()) {
        gallery.events.push_back(Event::deserialize(line));
      }
    }

    return gallery;
  }

  // Function to save to a file
  void saveToFile(const std::string &filename) const {
    std::ofstream outFile(filename);
    if (outFile) {
      outFile << serialize();
      outFile.close();
      std::cout << "Gallery data saved to " << filename << std::endl;
    } else {
      throw std::ios_base::failure("Failed to open file for writing.");
    }
  }

  // Function to load from a file
  static Gallery loadFromFile(const std::string &filename) {
    std::ifstream inFile(filename);
    if (inFile) {
      std::stringstream buffer;
      buffer << inFile.rdbuf();
      inFile.close();
      return deserialize(buffer.str());
    } else {
      throw std::ios_base::failure("Failed to open file for reading.");
    }
  }
};
