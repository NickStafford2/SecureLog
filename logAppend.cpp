#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

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


int main(int argc, char *argv[]) {
    
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
	if (argc == 3 or argc == 9 or argc == 11){
		std::cout << "successful" << std::endl;

		// all the leftover code goes here










	
	}else{
		std::cout << "INVALID!" << std::endl;
		std::cout << "Program exited with incorrect arguments! Please provide correct number of arguments!" << std::endl;
		std::exit(255);
	}
	
	// for (int i = 0; i < argc; i++){
	// 	std::cout << argv[i] << std::endl;

	// }


}

