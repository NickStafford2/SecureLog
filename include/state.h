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

#include "crypto.h"
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
        participantType(type) {
    std::cout << "EVENT CREATED WITH PERSON: " << person << std::endl;
  }

  // Method to print the event details
  void printEvent(std::string s = "\t  ") const {
    std::cout << s << "Timestamp: " << timestamp << ", Person: \"" << person
              << "\", From Location: " << from_location
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

    // Read timestamp, expecting an integer followed by a comma
    ss >> ts >> comma;

    // Read the 'person' field which could contain commas (so we need to read
    // until the next comma)
    std::getline(ss, person, ',');

    // Read from_location and to_location (integers)
    ss >> from >> comma >> to >> comma;

    // Read participant type (integer value)
    ss >> typeInt;

    // Return a newly constructed Event object using the parsed data
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
  static const std::string LOG_DIR;
  Gallery() = default;

  int getEmployeeRoom(const std::string &employee) const {
    if (employees.find(employee) != employees.end()) {
      return employees.at(employee);
    }
    return UNKNOWN;
  }

  int getNumberOfEvents() { return events.size(); }

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
    std::cout << "Moving " << event.person << " from " << event.from_location
              << " to " << event.to_location << std::endl;
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
      std::cerr << currentRoom << " != " << event.from_location << std::endl;
      throw std::runtime_error("Error: " + event.person +
                               " is not in the from_location: "); // +
      // event.from_location + " " + event.person + " was in " + currentRoom);
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
  void print() const {
    std::cout << "Printing Gallary: "
                 "=================================================\n";
    std::cout << "\tEmployees: \n";
    for (const auto &entry : employees) {
      std::cout << "\t  " << entry.first << " (Room ID: " << entry.second << ")"
                << std::endl;
    }

    std::cout << "\tGuests: \n";
    for (const auto &entry : guests) {
      std::cout << "\t  " << entry.first << " (Room ID: " << entry.second << ")"
                << std::endl;
    }

    std::cout << "\tEvents: \n";
    for (const auto &event : events) {
      event.printEvent();
    }
    std::cout << "============================================================="
                 "======\n";
  }
  // Serialize gallery to string
  std::string serialize() const {

    // this->print();
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

    // gallery.print();
    std::cout << "Created Gallery with " << gallery.getNumberOfEvents()
              << " events." << std::endl;
    return gallery;
  }

  // Function to save to a file
  void saveToFile(const std::string &filename, const std::string &key) const {

    std::string fullPath = Gallery::LOG_DIR + filename;
    std::cout << "Gallery data saving to " << fullPath << std::endl;
    std::ofstream outFile(fullPath);
    if (outFile) {
      std::string message = serialize();
      std::string encrypted = CryptoUtils::encrypt(message, key);
      outFile << encrypted;
      outFile.close();
      std::cout << "Gallery data saved to " << fullPath << std::endl;
    } else {
      throw std::ios_base::failure("Failed to open file for writing.");
    }
  }

  // Function to load from a file
  static Gallery loadFromFile(const std::string &filename,
                              const std::string &key) {
    std::string fullPath = Gallery::LOG_DIR + filename;
    std::cout << "Gallery data reading from " << fullPath << std::endl;
    std::ifstream inFile(fullPath);
    if (inFile) {
      std::stringstream buffer;
      buffer << inFile.rdbuf();
      inFile.close();
      std::string encrypted = buffer.str();
      std::string decrypted = CryptoUtils::decrypt(encrypted, key);
      return deserialize(decrypted);
      std::cout << "Gallery read from " << fullPath << std::endl;
    } else {
      throw std::ios_base::failure("Failed to open file for reading.");
    }
  }

  // Function to check if file exists, and load or create a new Gallery
  static Gallery loadOrCreate(const std::string &filename,
                              const std::string &key) {
    std::string fullPath = Gallery::LOG_DIR + filename;

    std::ifstream inFile(fullPath);
    if (inFile) {
      // File exists, load the data
      std::cout << "File exists. Loading data from " << fullPath << std::endl;
      inFile.close();
      return loadFromFile(filename, key);
    } else {
      // File doesn't exist, create a new Gallery
      std::cout << "File does not exist. Creating new Gallery." << std::endl;
      return Gallery(); // Create a new Gallery instance
    }
  }
};
