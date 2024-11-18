#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include  <fstream>
#include <cassert>
#include <cstring> 
#include <list>
#include <sstream> 
#include <vector>


using namespace std;


// void printHelp() {
//     cout << 
// 	"\t-T timestamp Time the event is recorded. This timestamp is formatted as the number of seconds since the gallery opened and is a non-negative integer (ranging from 1 to 1073741823 inclusively). Time should always increase, invoking logappend with an event at a time that is prior to the most recent event already recorded is an error.\n\n"

// 	"\t-K token Token used to authenticate the log. This token consists of an arbitrary-sized string of alphanumeric (a-z, A-Z, and 0-9) characters. The user supplies this token via the command line when creating a new log. Once a log is created with a specific token, any subsequent appends to that log must use the same token. You may assume the token contains sufficient entropy that it cannot be guessed in a reasonable amount of time.\n\n"

// 	"\t-E employee-name Name of employee. Names are alphabetic characters (a-z, A-Z) in upper and lower case. Names may not contain spaces. Names are case sensitive. Employees and guests can have the same name. The names can be arbitrarily long. More concretely, if you make an assumption about lengths, and an attacker can successfully land a correctness, or even confidentiality or integrity attack on the contest server by using a longer name, this will be considered a valid attack.\n\n"

// 	"\t-G guest-name Name of guest. Names are alphabetic characters (a-z, A-Z) in upper and lower case. Names may not contain spaces. Names are case sensitive. Employees and guests can have the same name. The names can be arbitrarily long. More concretely, if you make an assumption about lengths, and an attacker can successfully land a correctness, or even confidentiality or integrity attack on the contest server by using a longer name, this will be considered a valid attack.\n\n"

// 	"\t-A Specify that the current event is an arrival; can be used with -E, -G, and -R. This option can be used to signify the arrival of an employee or guest to the gallery, or, to a specific room with -R. If -R is not provided, -A indicates an arrival to the gallery as a whole. No employee or guest should enter a room without first entering the gallery. No employee or guest should enter a room without having left a previous room. Violation of either of these conditions implies inconsistency with the current log state and should result in logappend exiting with an error condition.\n\n"

// 	"\t-L Specify that the current event is a departure, can be used with -E, -G, and -R.This option can be used to signify the departure of an employee or guest from the gallery, or, from a specific room with -R. If -R is not provided, -L indicates a deparature from the gallery as a whole. No employee or guest should leave the gallery without first leaving the last room they entered. No employee or guest should leave a room without entering it. Violation of either of these conditions implies inconsistency with the current log state and should result in logappend exiting with an error condition.\n\n"

// 	"\t-R room-id Specifies the room ID for an event. Room IDs are non-negative integer characters with no spaces (ranging from 0 to 1073741823 inclusively). Leading zeros in room IDs should be dropped, such that 003, 03, and 3 are all equivalent room IDs. A gallery is composed of multiple rooms. A complete list of the rooms of the gallery is not available and rooms will only be described when an employee or guest enters or leaves one. A room cannot be left by an employee or guest unless that employee or guest has previously entered that room. An employee or guest may only occupy one room at a time. If a room ID is not specified, the event is for the entire art gallery.\n\n"

// 	"\tlog The name of the file containing the event log. The log’s filename may be specified with a string of alphanumeric characters (including underscores and periods). If the log does not exist, logappend should create it. logappend should add data to the log, preserving the history of the log such that queries from logread can be answered. If the log file cannot be created due to an invalid name, or any other error, logappend should print “invalid” and return 255.\n\n"

// 	"\t-B file Specifies a batch file of commands. file contains one or more command lines, not including the logappend command itself (just its options); the lines are separated by \n (newlines), but the final newline in a file is optional. These commands should be processed by logappend individually, in order. This allows logappend to add data to the file without forking or re-invoking. Of course, option -B cannot itself appear in one of these command lines. Commands specified in a batch file include the log name. If a single line in a batch file is invalid, print the appropriate error message for that line and continue processing the rest of the batch file. Here is an example (the last one).\n\n";
// }

// class DataLine {

//     private:
//         string id;
// 		string timestamp;
    
//     public: 
//     string toLogFormat() {
// 	string output = string("") + 
// 	    id + "\t" +
// 	    timestamp + "\t" + 
// 	    "\n";
// 	return output;
//     }

//     void setId(int n) {
// 	id = to_string(n);
//     }

//     void setTimestamp(string n) {
// 	long int nLong = stol(n);
// 	timestamp = to_string(nLong);
//     }
// };

void check_command(int argc, char *argv[]){
	if (argc == 3){
		assert(
			std::strcmp(argv[1], "-B") == 0	
		);
		std::cout << "Success" << std::endl;
	}
	if (argc == 9){
		assert(
			std::strcmp(argv[1], "-T") == 0 &&
			std::strcmp(argv[3], "-K") == 0 &&
			(std::strcmp(argv[5], "-A") == 0 || std::strcmp(argv[5], "-L") == 0) &&
			(std::strcmp(argv[6], "-E") == 0 || std::strcmp(argv[6], "-G") == 0)	
				);
		std::cout << "Success8"<< std::endl;
	}
	if (argc == 11){
		assert(
				std::strcmp(argv[1], "-T") == 0 &&
				std::strcmp(argv[3], "-K") == 0 &&
				(std::strcmp(argv[5], "-A") == 0 || std::strcmp(argv[5], "-L") == 0) &&
				(std::strcmp(argv[6], "-E") == 0 || std::strcmp(argv[6], "-G") == 0) &&
				std::strcmp(argv[8], "-R") == 0
    		);
		std::cout << "Success!!"<< std::endl;

	}

}

int get_most_recent_time(int argc, std::string filename){
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
	std::cout << "most recent time is: " << time << std::endl;
	return time;

}

int validate_timestamp(int  argc, int currentTimestamp, std::string filename) {
    int mostRecentTimestamp = get_most_recent_time(argc, filename);
	std::cout << "this line" << std::endl;
    if (currentTimestamp <= mostRecentTimestamp || currentTimestamp < 1 || currentTimestamp > 1073741823) {
        std::cerr << "Invalid: Timestamp" << std::endl;
        exit(255);
    }
	std::cout << "Timestamp succesful"<< std::endl;
    return currentTimestamp;
}

std::string token_validation(const std::string str){
	for (char c: str){
		if(!std::isalnum(c)){
			std::cerr << "Invalid: Token" << std::endl;
			exit(255);
		}
	}
	std::cout << "Token is successfully validated!" << std::endl;
	return str;

}

std::string name_validation(const std::string name){
	for(char n: name){
		if (!std::isalpha(n)){
			std::cerr << "Invalid: Name" << std::endl;
			exit(255);
		}
	}
	std::cout << "Name is successfully validated" << std::endl;
	return name;
}
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
bool check_constraints(char *argv[],std::string filename){
	std::cout << "check_constraint is working" << std::endl;
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
		// std::cout << wordList[3] << wordList.size() << std::endl;
		// std::cout << lastList[3] << lastList.size() << std::endl;
		// for(auto n: wordList){
		// 	std::cout << n << " ";
		// }
		// std::cout << std::endl;
		for(auto n: lastList){
			std::cout << n << " ";
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
	file.close();
	std::cout << inputList.size() << std::endl;

	if (lastList.size() == 8 && lastList[4] == "-L" && inputList.size() == 9 && inputList[5] == "-A"){
		return true;
	}
	if (lastList.size() == 8 && lastList[4] == "-L"){
		std::cout << "Not in the Gallery" << std::endl;
		exit(255);
	}
	if (lastList.size() == 0 && inputList.size() == 9 && inputList[5] == "-A"){
		std::cout << "new incoming" << std::endl;
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
		std::cout << "Invalidd"<< std::endl;
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
		std::cout << "Already in the room" << std::endl;
		exit(255);
	}
	return false;

}
int main(int argc, char *argv[]) {
	std::string filename;
    
    // DataLine line; 
    // line.setId(0); // todo: decide if we want this;
    // // 
    // // Handle input arguments
    // for (int i=1; i< argc; i++) {
	// // printf("Argument %i = %s\n", i, argv[i]);
	// string userInput = argv[i];
	// if (userInput == "--help") {
	//     printHelp();
	// }
	// if (userInput == "-T") {
	//     line.setTimestamp(argv[i+1]); // todo: could error out
    // 	}
    // }
    
    // string s = line.toLogFormat();
    // std::ofstream outfile;
    // outfile.open("log.txt", std::ios_base::app);


    // outfile << s;
    // cout << "LogAppend Successful\n";
    // return 0;


	// checking input
		// accepting if single line of command or a batch
		// rejecting and exiting with an argument telling that the number of inputs is incorrect
	
	std::cout << argc<< std::endl;
	if (argc == 3 || argc == 9 || argc == 11){
		std::cout << "successful" << std::endl;
		// all the leftover code goes here
		// check if it is 9, 3, or 11
		if (argc == 3)
		{
		
			std::cout << "It is a batch file" << std::endl;
			/* code */

		}
		else if(argc == 9) {
			check_command(argc, argv);
			filename = file_validation(argv[8]) + ".txt";
			int time = validate_timestamp(argc, std::stoi(argv[2]), filename);

			std::string token = token_validation(argv[4]);
			std::string name = name_validation(argv[7]);
			check_constraints(argv, filename);
			
			std::cout << "It is a single command with no room in it." << std::endl;


		}else if(argc == 11){
			check_command(argc, argv);
			filename = file_validation(argv[10]) + ".txt"; 
			int time = validate_timestamp(argc, std::stoi(argv[2]), filename);
			std::string token = token_validation(argv[4]);
			std::string name = name_validation(argv[7]);
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
		
		
		// std::cout << "now this line should run" << std::endl;
		// std::fstream file("log.txt");

		// if(!file.is_open()){
		// 	std::cout << "Error opening the file!" << std::endl;
		// 	exit(255); 
		// }

		// std::string line;
		// while(std::getline(file, line)){
		// 	std::cout << line << std::endl;
		// }

		// file.close();

	}else{
		std::cout << "INVALID!" << std::endl;
		std::cout << "Program exited with incorrect arguments! Please provide correct number of arguments!" << std::endl;
		std::exit(255);
	}
	
	// for (int i = 0; i < argc; i++){
	// 	std::cout << argv[i] << std::endl;

	// }


}

