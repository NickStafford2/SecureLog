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
#include "inputValidation.h"

// check all the options like -T -K (-A or -L)-R (-E or -G)
void check_command(int argc, char *argv[]){
	if (argc == 3){
		assert(
			std::strcmp(argv[1], "-B") == 0	
		);
		std::cout << "-B check success" << std::endl;
	}
	if (argc == 9){
		assert(
			std::strcmp(argv[1], "-T") == 0 &&
			std::strcmp(argv[3], "-K") == 0 &&
			(std::strcmp(argv[5], "-A") == 0 || std::strcmp(argv[5], "-L") == 0) &&
			(std::strcmp(argv[6], "-E") == 0 || std::strcmp(argv[6], "-G") == 0)	
				);
		std::cout << "Arguments content check successful!"<< std::endl;
		return;
	}
	if (argc == 11){
		assert(
				std::strcmp(argv[1], "-T") == 0 &&
				std::strcmp(argv[3], "-K") == 0 &&
				(std::strcmp(argv[5], "-A") == 0 || std::strcmp(argv[5], "-L") == 0) &&
				(std::strcmp(argv[6], "-E") == 0 || std::strcmp(argv[6], "-G") == 0) &&
				std::strcmp(argv[8], "-R") == 0
    		);
		std::cout << "Arguments content check successful!"<< std::endl;
		return;

	}
}


// batch_validation validates if the batch filename is correct
// if does not check if there is any file or not
std::string batch_validation(const std::string& str){
	for (char c: str){
		if(!std::isalnum(c)){
			std::cerr << "Invalid: Batch" << std::endl;
			exit(255);
		}
	}
	std::cout << "Batch is successfully validated!" << std::endl;
	return str;
}

// get_most_recent_time grabs the most recent time present in that file
int get_most_recent_time(int argc, const std::string& filename){
	// std::cout << "get most recent time is working" << std::endl;
	int time = 1;
	if (argc == 9){
		std::ofstream writeFile(filename,std::ios::app);
		writeFile.close();
	}
	
	std::fstream file(filename);
	if(!file.is_open()){
		std::cout << "Error opening the file!" << std::endl;

		exit(255); 
	}
	std::string line;
	while(std::getline(file, line)){
		// std::cout << line << std::endl;
		std::list<std::string> wordList;
        std::istringstream iss(line);
		std::string word;
		// Split line into words and store each word in the list
		while (iss >> word) {
			wordList.push_back(word);
		}
		auto it = wordList.begin();
        std::advance(it, 1);
	
		if ( std::stoi(*it) > time ){
			time = std::stoi(*it);
		}
	}
	file.close();
	// std::cout << "most recent time is: " << time << std::endl;
	return time;

}

// validate_timestamp validates the time constraints. 
void validate_timestamp(const int&  argc, const int& currentTimestamp, const std::string& filename) {
    int mostRecentTimestamp = get_most_recent_time(argc, filename);
	// std::cout << "this line" << std::endl;
    if (currentTimestamp <= mostRecentTimestamp || currentTimestamp < 1 || currentTimestamp > 1073741823) {
        std::cerr << "Invalid: Timestamp" << std::endl;
        exit(255);
    }
	std::cout << "Timestamp validation successful!"<< std::endl;
    return;
}

// token_validation validates the secret key
void token_validation(const std::string str){
	for (char c: str){
		if(!std::isalnum(c)){
			std::cerr << "Invalid: Token" << std::endl;
			exit(255);
		}
	}
	std::cout << "Token is successfully validated!" << std::endl;
	return;
}

// name_validation validates the name of the person
void name_validation(const std::string name){
	for(char n: name){
		if (!std::isalpha(n)){
			std::cerr << "Invalid: Name" << std::endl;
			exit(255);
		}
	}
	std::cout << "Name is successfully validated" << std::endl;
	return;
}
// file_validation validates the name of the file
// it does not validate if there is any file relates to that name
std::string file_validation(const std::string filename){
	for(char n: filename){
		if (!std::isalnum(n) && n != '_' && n != '.'){
			std::cerr << "Invalid: Filename " << std::endl;
			exit(255);
		}
	}
	std::cout << "Filename is successfully validated" << std::endl;
	return filename;
}
// room_validation validates the room and is under the integer constraints.
void room_validation(int room){
	if (room < 1 || room > 1073741823) {
        std::cerr << "Invalid: Room" << std::endl;
        exit(255);
    }
	std::cout << "Room validation successful!"<< std::endl;
    return;

}
// check_constraints checks all of the constraints to of the state of the gallery
bool check_constraints(char *argv[],std::string filename){
	// std::cout << "check_constraint is working" << std::endl;
	std::vector<std::string> inputList;
	for(int i = 0; argv[i] != nullptr; ++i){
		inputList.push_back(argv[i]);
	}

	std::vector<std::string> lastList;
	std::fstream file(filename);

	if(!file.is_open()){
		std::cout << "Error opening the file!" << std::endl;
		exit(255); 
	}
	std::string line;
	while(std::getline(file, line)){

	
		std::vector<std::string> wordList;
        std::istringstream iss(line);
		std::string word;
		// Split line into words and store each word in the list
		while (iss >> word) {
			wordList.push_back(word);
		}
		if (wordList[3] == argv[4] && wordList[5] == argv[6] && wordList[6] == argv[7]){
			lastList = wordList;
		}
	
	}
	file.close();
	// constraint checking

	if (lastList.size() == 8 && lastList[4] == "-L" && inputList.size() == 9 && inputList[5] == "-A"){
		return true;
	}
	if (lastList.size() == 8 && lastList[4] == "-L"){
		std::cout << "Not in the Gallery" << std::endl;
		exit(255);
	}
	if (lastList.size() == 0 && inputList.size() == 9 && inputList[5] == "-A"){
		std::cout << "New Employee or Guest coming in!" << std::endl;
		return true;
	}
	if (lastList.size() == 0 && inputList.size() == 9 && inputList[5] == "-L"){
		std::cout << "Invalid" << std::endl;
		exit(255);
	}
	if(lastList.size() == 0 && inputList.size() == 11){
		std::cout << "Invalid" << std::endl;
		exit(255);
	}

	if(lastList.size() == 8 && inputList.size() == 11 && lastList[4] == "-A" && inputList[5] == "-L"){
		std::cout << "Invalid"<< std::endl;
		exit(255);
	}
	if(lastList.size() == 8 && inputList.size() == 11 && lastList[4] == "-A" && inputList[5] == "-A"){
		return true;
	}

	if(lastList.size() == 10 && inputList.size() == 9 && lastList[4] == "-A" && inputList[5]== "-A"){
		std::cout << "Invalidd, cannot arrive at gallery when in room"<< std::endl;
		exit(255);
	}
	if(lastList.size() == 10 && inputList.size() == 9 && lastList[4] == "-L" && inputList[5]== "-L"){
		return true;
	}
	
	if(lastList.size() == 10 && inputList.size() == 11 && lastList[4] == "-A" && inputList[5] == "-L" && lastList[8] == inputList[9]){
		return true;
	}
	if(lastList.size() == 10 && inputList.size() == 11 && lastList[4] == "-A" && inputList[5] == "-L" && lastList[8] != inputList[9]){
		std::cout << "Invalid, cannot leave from another room" << std::endl;
		exit(255);
	}
	if(lastList.size() == 10 && inputList.size() == 11 && lastList[4] == "-A" && inputList[5] == "-A" && lastList[8] != inputList[9]){
		std::cout << "Invalid, cannot go to one room to another" << std::endl;
		exit(255);
	}
	if (std::equal(lastList.begin() + 2, lastList.end(), inputList.begin() + 3)){
		if (lastList.size() == 8){
			std::cout << "Already in the gallery" << std::endl;
		}else if(lastList.size() == 10){
			std::cout << "Already in the room" << std::endl;

		}
		
		exit(255);
	}
	return false;

}
// executor runs the single line command 
void executor(int argc, char *argv[]){
	std::cout << std::endl;
	std::string filename;
	// std::cout << "Argument length: " << argc<< std::endl;
	if (argc == 9 || argc == 11){
		std::cout << "Argument length check successful!" << std::endl;
		// all the leftover code goes here
		// check if it is 9, 3, or 11
		if(argc == 9) {
			check_command(argc, argv);
			filename = file_validation(argv[8]) + ".txt";
			validate_timestamp(argc, std::stoi(argv[2]), filename);
			token_validation(argv[4]);
			name_validation(argv[7]);
			check_constraints(argv, filename);
			
			std::cout << "It is a single command with no room in it." << std::endl;

		}else if(argc == 11){
			check_command(argc, argv);
			filename = file_validation(argv[10]) + ".txt"; 
			validate_timestamp(argc, std::stoi(argv[2]), filename);
			token_validation(argv[4]);
			name_validation(argv[7]);
			room_validation(std::stoi(argv[9]));
			check_constraints(argv, filename);
			std::cout << "It is a single command with room in it" << std::endl;

		}

		// lets try to insert into the file
			// open the file for appending
		std::ofstream writeFile(filename,std::ios::app);

		while(!writeFile.is_open()){
			std::cout << "Error opening the file for writing" << std::endl;
			exit(255);
		}

		for (int i = 1; i < argc; i++) writeFile << argv[i] << " ";
		writeFile << "\n";
		writeFile.close();
	}else{
		std::cout << "INVALID!" << std::endl;
		std::cout << "Program exited due to invalid arguments. Please provide correct number of arguments." << std::endl;
		std::exit(255);
	}
	

}
// process_single_command process each line from the batch file
void process_single_command(const std::string& command) {
    if (command.empty() || command.find("-B") != std::string::npos) {
        std::cerr << "Invalid command: " << command << std::endl;
        return;
    }

    // Split command into arguments (argc and argv format)
    std::istringstream iss(command);
    std::vector<std::string> args;
    std::string arg;
	args.push_back("logappend");
    while (iss >> arg) {
        args.push_back(arg);
    }

    // Prepare argc and argv for the executor
    int argc = args.size();
    std::vector<char*> argv(argc);

    for (size_t i = 0; i < args.size(); ++i) {
        argv[i] = const_cast<char*>(args[i].c_str());
    }

    // Call the executor function
	std::cout << std::endl;
	for (int i = 0; i < argc; ++i){
		std::cout << argv[i] << " ";
	}
  executor(argc, argv.data());
}


void printHelp() {
    std::cout << "Usage: logread [OPTIONS] <log>\n\n";

    std::cout << "Description:\n";
    std::cout << "logread is a tool for viewing and analyzing log data, specifically for tracking the activities of employees and guests in a gallery. "
              << "It provides functionality to check various states, list rooms, calculate time spent, and more. Use the options below to filter and customize the log output.\n\n";

    std::cout << "Options:\n";
    std::cout << "  -K <token>          Token used to authenticate log access. Alphanumeric.\n";
    std::cout << "  -S                  Print the current state of the log to stdout.\n";
    std::cout << "  -R                  List all rooms entered by an employee or guest. "
              << "If specified, either -E or -G must also be specified.\n";
    std::cout << "  -T                  Shows the total time spent in the gallery by an employee or guest. "
              << "If they are still in the gallery, the time since their arrival is provided. Optional.\n";
    std::cout << "  -I                  Prints the rooms that were occupied by all the specified employees and guests "
              << "at the same time over the complete history of the gallery.\n";
    std::cout << "  -E <name>           Employee name. May be specified multiple times when used with -I.\n";
    std::cout << "  -G <name>           Guest name. May be specified multiple times when used with -I.\n";
    std::cout << "  <log>               The name of the log file used for recording events. Alphanumeric, underscores, and periods.\n\n";

    std::cout << "Examples:\n";
    std::cout << "  logread -K <token> -T (-E <name> | -G <name>) <log>\n";
    std::cout << "  logread -K <token> -I (-E <name> | -G <name>) [(-E <name> | -G <name>) ...] <log>\n\n";

    std::cout << "Note:\n";
    std::cout << "  - When using the -R option, either -E or -G must be specified.\n";
    std::cout << "  - The -T option is optional, and if used, it will provide the time spent in the gallery by the person(s) specified.\n";
}

int main(int argc, char *argv[]) {

	if (argc == 3){
		// run the batch command
		check_command(argc, argv);
		std::string filename = batch_validation(argv[2]) + ".txt";
		std::cout << filename << std::endl;

		std::fstream file(filename);
		if(!file.is_open()){
			std::cout << "Error opening the batch file!" << std::endl;
			exit(255); 
		}
		std::string line;
		while(std::getline(file, line)){
			process_single_command(line);
		}
		file.close();
		

	}else{
		// run the sinle line command
		executor(argc, argv);
	}
	
}

