# SecureLog
- Nicholas Stafford
- Abhishek Pandit

Private and secure CLI logging. 

# Introduction
## Purpose
Provide access to a secure log file which is used to document the state of an art gallery. Describes the guests and employees who have entered and left; as well as people whom are in the rooms.

## Intended use
Users are gallary employees and guests. 

Two programs are used to access the log via command line. 
- logread - Reads from the log and returns information regarding the art gallery’s status. Accepts query information and displays response
- logappend - Appends new information to the file. 

## Scope
Application should be secure and simple. Accessed via command line with strict requirements. Only necessary features are implemented to limit vulnerabilities. 

# Functional Requirements
## logappend
Appends data to the specified log file. If the log does not exist, a new log is created. Requires a user specified timestamp and authentication token. User is informed if information provided is invalid in any way. 

Logappend must have permissions to read and write to files in the directory. Logappend has not state and strictly writes to the logs.

Logappend has two possible inputs. Information from the cli, as well as information from files specified from command line arguments. Both data sources are written to specifiedlog fies. These provide significant security risks.


## logread
Queries the state of the gallery. Ptints out which employees and guests are in the gallery and rooms. Arguments may be used to query specific information. It is worth noting that secure programming techniques must be used to ensure safe execution.  

Logread must have read permissions. 

Employeess and guests are expected to enter the gallery first. Once in the gallery, they may enter various rooms. The person is considered 'in the gallery' from the time they enter until the time they leave. 


# External Interface Requirements

## CLI
### logappend
~~~
logappend -T <timestamp> -K <token> (-E <employee-name> | -G <guest-name>) (-A | -L) [-R <room-id>] <log>
logappend -B <file>
~~~

-T timestamp Time of the event. Must be provided as the number of seconds since the gallery opening. 

-K token Token used to authenticate log access. Alphanumeric. 

-E employee-name Name of the employee. Case sensitive. Alphanumeric.

-G guest-name Name of the guest. Alphanumeric.

-A Specifies that the event is an arrival. Used to specify the arrival of a specific employee at the gallary or at a specific room with -R.

-L Specifies that the event is a departure. Used to signify departure of a specific employee from the gallary, or from a specific room if specified with -R.

-R room-id Spedivies the room ID for an event. Non negative integer. 

log The name of the file containing the event log. Alphanumeric, underscores, and periods. 

-B file Specifies a batch file of commands.


### logread
~~~
logread -K <token> -T (-E <name> | -G <name>) <log>
logread -K <token> -I (-E <name> | -G <name>) [(-E <name> | -G <name>) ...] <log>
~~~

-K token Token used to authenticate log access. Alphanumeric. 

-S Print the current state of the log to stdout. 

-R Give a list of all rooms entered by an employee or guest. If specified, either -E or -G must be specified.

-T Gives the total time spent in the gallery by an employee or guest. If the employee or guest is still in the gallery, the time since their arrival is provided. Optional.

- I Prints the rooms that were occupied by all the specified employees and guests at the same time over the complete histor of the gallery.

-E Employee name. May be specified mutltiple times when used with -I. 

-G Guest name. May be specified multiple times when used with -I. 

log The name of the file log used for recording events. Alphanumeric, underscores, and periods. 


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

Every command line code will have an input validation and an authentication with a token. At every point where there will be state change, it must be validated first. Validation requirements: 
- Ensure authentication token matches the one in the file.
- String format check to prevent format attack
- Command line number of inputs check, either single command line code or a batch file 
- Each command line argument will check to ensure only expected characters are provided.
- Limited error information. Return “Inserted” if the log has been successfully inserted and “invalid” or 255, depending on where the error occurred, if there was any problen while validating and authenticating the token. This limits hackers knowlege of how the program handled the input.

### File Access
- Files will be encrypted with some standard encryption algorithm. Without the authentication token, it is impossible to access. 
- Authentication tokens will be hashed and salted. 
- Only read 100Mb of data maximum.

## Capacity and Scale
Limit file size to 100Mb. 
 
## Compatibility
Accessable via most popular command line applications. Uses minimal system resources

## Reliability and Availibility
Not essential requiremetns








