# SecureLog
- Nicholas Stafford
- Abhishek Pandit

Private and secure CLI logging. 

# Introduction
## Purpose
Provide access to a secure log file which is used to document the state of an art gallery. Describes the guests and employees who have entered and left; as well as people whom are in the rooms.

## Intended use
Two programs are used to access the log via command line. 
logread - Reads from the log and returns information regarding the art gallery’s status. Accepts query information and displays response
logappend - Appends new information to the file. 

## Scope
Application should be secure and simple. Accessed via command line with strict requirements. Only necessary features are implemented to limit vulnerabilities. 

# Functional Requirements
## logappend
Logappend, takes input from the command line and stores the log if the users credentials are authenticated. The structure for logappend is divided into two parts. 

First, we can provide command line input for each employee or guest log. Second, we can directly use a batch file to send multiple log command for multiple employees or guests at once. 
Visually, it can be represented like this:

Fig: The top is for executing the single command line and the bottom is for executing the batch file. 



## logread
todo


# External Interface Requirements

## Dependencies
Some sort of terminal to access the cli.
In order to compile the following must be installed and accessible via the PATH.
- C++
- Make
- Gcc

## User Permissions
Users must have system permissions to read, write, and execute. 





# Non-Functional Requirements

## Security Requirements
SecureLog is only accessable to users with the proper credentails. 

Every command line code will have an input validation and an authentication with a token. Also, at every point there will be state change validation: state change is accepted or not. In the batch file too, there will be same but will have multiple log commands. There will also be format string check to check and prevent from the format string attack. 

The priority will be:
1. Command line number of inputs check, either single command line code or a batch file 
2. Each of the command line will string or alphanumeric characters will be validated for the format. 
3. Employee or Guest will be validated while authenticating using the token.
4. Return “Inserted” if the log has been successfully inserted and “invalid” if there was any problen while validating and authenticating the token.

## Capacity and Scale
File size ???
 
## Compatibility
Accessable via most popular command line applications. Uses minimal system resources

## Reliability and Availibility
Not essential requiremetns








