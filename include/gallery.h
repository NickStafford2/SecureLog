// gallery.h
#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "crypto.h"
#include "event.h"
#include "participantType.h"
#include "utils.h"

class Gallery {
private:
  // Employees and Guests as key-value pairs where the key is the name and the
  // value is a list of rooms the person has gone to
  std::unordered_map<std::string, std::vector<int>> employees;
  std::unordered_map<std::string, std::vector<int>> guests;
  std::vector<Event> events;

  int mostRecentTimestamp = 0; // To keep track of the most recent timestamp

public:
  static const int GALLERY_ID = -1;
  static const int UNKNOWN = -2;
  static const int ERROR = -3;
  static const std::string LOG_DIR;
  Gallery() = default;
  static std::string readifyLocation(int location);
  void printTimeFor(std::string name, ParticipantType participantType);

  int getNumberOfEvents() { return events.size(); }

  std::vector<int> getEmployeeRooms(const std::string &guest) const {
    return guests.at(guest);
  }

  std::vector<int> getGuestRooms(const std::string &guest) const {
    return guests.at(guest);
  }

  int getEmployeeRoom(const std::string &employee) const {
    return getEmployeeRooms(employee).back();
  }

  int getGuestRoom(const std::string &guest) const {
    return getGuestRooms(guest).back();
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
    // std::cout << "Moving " << event.person << " from " << event.from_location
    //           << " to " << event.to_location << std::endl;
    validateTimestamp(event.timestamp);

    int currentRoom = UNKNOWN;

    // If the event is for an employee, get their room, else for a guest
    if (event.participantType == ParticipantType::EMPLOYEE) {
      if (employees.find(event.person) != employees.end()) {
        currentRoom = getEmployeeRoom(event.person);
      }
    } else if (event.participantType == ParticipantType::GUEST) {
      if (guests.find(event.person) != guests.end()) {
        currentRoom = getGuestRoom(event.person);
      }
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
      employees[event.person].push_back(event.to_location);
    } else if (event.participantType == ParticipantType::GUEST) {
      guests[event.person].push_back(event.to_location);
    }
    // std::cout << "Move Successfull" << std::endl;
  }

  void printEmployee(const std::string &employeeName) const {
    auto it = employees.find(employeeName);
    if (it != employees.end()) {
      std::cout << "Employee: " << it->first
                << " (Rooms: " << intArrayToString(it->second) << ")"
                << std::endl;
    } else {
      std::cout << "Employee with name '" << employeeName << "' not found."
                << std::endl;
    }
  }

  void printGuest(const std::string &guestName) const {
    auto it = guests.find(guestName);
    if (it != guests.end()) {
      std::cout << "Guest: " << it->first
                << " (Rooms: " << intArrayToString(it->second) << ")"
                << std::endl;
    } else {
      std::cout << "Guest with name '" << guestName << "' not found."
                << std::endl;
    }
  }

  void printEmployees() const {
    std::cout << "Employees: \n";
    for (const auto &entry : employees) {
      std::cout << "  " << entry.first
                << " (Rooms: " << intArrayToString(entry.second) << ")"
                << std::endl;
    }
  }

  void printGuests() const {
    std::cout << "Guests: \n";
    for (const auto &entry : guests) {
      std::cout << "  " << entry.first
                << " (Rooms: " << intArrayToString(entry.second) << ")"
                << std::endl;
    }
  }

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
    printEmployees();
    printGuests();
    printEvents();
    std::cout << "============================================================="
                 "======\n";
  }

  void printStateSimple() const {

    // Print Employees List
    std::cout << "Employees: ";
    bool first = true;
    for (const auto &entry : employees) {
      if (!first)
        std::cout << ", ";
      std::cout << entry.first;
      first = false;
    }
    std::cout << "\n";

    // Print Guests List
    std::cout << "Guests: ";
    first = true;
    for (const auto &entry : guests) {
      if (!first)
        std::cout << ", ";
      std::cout << entry.first;
      first = false;
    }
    std::cout << "\n";

    // Create a map to store people in each room
    std::unordered_map<int, std::set<std::string>> roomOccupants;

    // Add employees to the roomOccupants map
    for (const auto &entry : employees) {
      for (int room : entry.second) {
        roomOccupants[room].insert(entry.first);
      }
    }

    // Add guests to the roomOccupants map
    for (const auto &entry : guests) {
      for (int room : entry.second) {
        roomOccupants[room].insert(entry.first);
      }
    }

    // Print room-by-room information
    for (const auto &room : roomOccupants) {
      std::cout << room.first << ": ";
      bool firstPerson = true;
      for (const auto &person : room.second) {
        if (!firstPerson)
          std::cout << ", ";
        std::cout << person;
        firstPerson = false;
      }
      std::cout << "\n";
    }
  }

  // Serialize a vector of rooms into a comma-separated string
  static std::string serializeRooms(const std::vector<int> &rooms) {
    std::stringstream ss;
    for (size_t i = 0; i < rooms.size(); ++i) {
      ss << rooms[i];
      if (i != rooms.size() - 1) {
        ss << ",";
      }
    }
    return ss.str();
  }

  // Deserialize a comma-separated string into a vector of rooms
  static std::vector<int> deserializeRooms(const std::string &roomsStr) {
    std::vector<int> roomNumbers;
    std::stringstream roomStream(roomsStr);
    std::string room;
    while (std::getline(roomStream, room, ',')) {
      roomNumbers.push_back(std::stoi(room)); // Convert each room to int
    }
    return roomNumbers;
  }

  std::string serialize() const {
    std::stringstream ss;

    // Serialize employees
    ss << "Employees:\n";
    for (const auto &entry : employees) {
      ss << entry.first << ":";
      ss << serializeRooms(entry.second)
         << "\n"; // Use the helper function to serialize rooms
    }

    // Serialize guests
    ss << "Guests:\n";
    for (const auto &entry : guests) {
      ss << entry.first << ":";
      ss << serializeRooms(entry.second)
         << "\n"; // Use the helper function to serialize rooms
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
      std::string roomsStr;

      std::getline(empStream, name, ':');
      std::getline(empStream, roomsStr);

      std::vector<int> roomNumbers = deserializeRooms(roomsStr);
      gallery.employees[name] = roomNumbers;
    }

    // Read guests
    while (std::getline(ss, line) && line != "Events:") {
      if (line.empty() || line == "Guests:")
        continue;

      std::stringstream guestStream(line);
      std::string name;
      std::string roomsStr;

      std::getline(guestStream, name, ':');
      std::getline(guestStream, roomsStr);

      std::vector<int> roomNumbers = deserializeRooms(roomsStr);
      gallery.guests[name] = roomNumbers;
    }

    // Read events
    while (std::getline(ss, line)) {
      if (!line.empty()) {
        gallery.events.push_back(Event::deserialize(line));
      }
    }

    // std::cout << "Created Gallery with " << gallery.getNumberOfEvents()
    //           << " events." << std::endl;
    return gallery;
  }

  // Function to save to a file
  void saveToFile(const std::string &filename, const std::string &key) const {

    std::string fullPath = Gallery::LOG_DIR + filename;
    // std::cout << "Gallery data saving to " << fullPath << std::endl;
    std::ofstream outFile(fullPath);
    if (outFile) {
      std::string message = serialize();
      std::string encrypted = CryptoUtils::encrypt(message, key);
      outFile << encrypted;
      outFile.close();
      // std::cout << "Gallery data saved to " << fullPath << std::endl;
    } else {
      throw std::ios_base::failure("Failed to open file for writing.");
    }
  }

  // Function to load from a file
  static Gallery loadFromFile(const std::string &filename,
                              const std::string &key) {
    std::string fullPath = Gallery::LOG_DIR + filename;
    // std::cout << "Gallery data reading from " << fullPath << std::endl;
    std::ifstream inFile(fullPath);
    if (inFile) {
      std::stringstream buffer;
      buffer << inFile.rdbuf();
      inFile.close();
      std::string encrypted = buffer.str();
      std::string decrypted = CryptoUtils::decrypt(encrypted, key);
      return deserialize(decrypted);
      // std::cout << "Gallery read from " << fullPath << std::endl;
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
      // std::cout << "File exists. Loading data from " << fullPath <<
      // std::endl;
      inFile.close();
      return loadFromFile(filename, key);
    } else {
      // File doesn't exist, create a new Gallery
      // std::cout << "File does not exist. Creating new Gallery." << std::endl;
      return Gallery(); // Create a new Gallery instance
    }
  }
};
