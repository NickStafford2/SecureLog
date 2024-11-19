#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <cassert>
#include <cstring>
#include <list>
#include <sstream>
#include <vector>

void check_command(int argc, char *argv[])
{
    if (argc == 5)
    {
        assert(
            std::strcmp(argv[1], "-K") == 0 &&
            std::strcmp(argv[3], "-S") == 0);
        std::cout << "Argument content check successful!" << std::endl;
    }
    else if (argc == 7)
    {
        assert(
            std::strcmp(argv[1], "-K") == 0 &&
            std::strcmp(argv[3], "-R") == 0 &&
            (std::strcmp(argv[4], "-E") == 0 || std::strcmp(argv[4], "-G") == 0));
        std::cout << "Argument content check successful!" << std::endl;
    }
}

std::string file_validation(const std::string filename)
{
    for (char n : filename)
    {
        if (!std::isalnum(n) && n != '_' && n != '.')
        {
            std::cerr << "Invalid: Filename " << std::endl;
            exit(255);
        }
    }
    std::cout << "Filename is successfully validated" << std::endl;
    return filename;
}

void token_validation(const std::string str)
{
    for (char c : str)
    {
        if (!std::isalnum(c))
        {
            std::cerr << "Invalid: Token" << std::endl;
            exit(255);
        }
    }
    std::cout << "Token is successfully validated!" << std::endl;
    return;
}

void name_validation(const std::string name)
{
    for (char n : name)
    {
        if (!std::isalpha(n))
        {
            std::cerr << "Invalid: Name" << std::endl;
            exit(255);
        }
    }
    std::cout << "Name is successfully validated" << std::endl;
    return;
}



void processArgument(const std::string& filename, const std::string& key) {
    std::vector<std::string> namesPeople; // List of unique people
    std::unordered_map<int, std::vector<std::string> > roomAssignments; // room -> people mapping

    std::fstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        exit(255);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> wordList;
        std::istringstream iss(line);
        std::string word;

        // Split the line into words
        while (iss >> word) {
            wordList.push_back(word);
        }

        // Validate key match
        if (key == wordList[3]) {
            std::string name = wordList[6];

            // Add to namesPeople if not already present
            if (std::find(namesPeople.begin(), namesPeople.end(), name) == namesPeople.end()) {
                namesPeople.push_back(name);
            }

            // Handle room assignment if applicable
            if (wordList.size() == 10) {
                int roomNumber = std::stoi(wordList[8]);
                if (std::find(roomAssignments[roomNumber].begin(), roomAssignments[roomNumber].end(), name) == roomAssignments[roomNumber].end()) {
                    roomAssignments[roomNumber].push_back(name);
                }
            }
        }
    }
    file.close();

    // Output names of people
    for (const auto& name : namesPeople) {
        std::cout << name << std::endl;
    }

    // Output room assignments
    for (const auto& room : roomAssignments) {
        std::cout << room.first << ": ";
        for (size_t i = 0; i < room.second.size(); ++i) {
            std::cout << room.second[i] << ", ";
            if (i < room.second.size() - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
}


int main(int argc, char *argv[])
{
    // all the code goes here
    std::string filename;
    if (std::string(argv[3]) == "-T" || std::string(argv[3]) == "-I")
    {
        std::cout << "Unimplemented!" << std::endl;
        std::exit(255);
    }
    else if (argc == 5 || argc == 7)
    {
        if (argc == 5)
        {
            // std::cout << "this line is working" << std::endl;
            check_command(argc, argv);
            filename = file_validation(argv[4]) + ".txt";
            // std::cout << filename <<std::endl;
            token_validation(argv[2]);
            processArgument(filename, std::string(argv[2]));

        }
        else if (argc == 7)
        {
            check_command(argc, argv);
            filename = file_validation(argv[6]) + ".txt";
            token_validation(argv[2]);
            name_validation(argv[5]);
        }
    }
    else
    {
        std::cout << "INVALID!" << std::endl;
        std::cout << "Program exited with incorrect arguments! Please provide correct number of arguments!" << std::endl;
        std::exit(255);
    }
}