# SecureLog

- Nicholas Stafford
- Abhishek

# Program requirements

<a href="https://course.ece.cmu.edu/~ece732/s24/homework/bibifi/overview.html">https://course.ece.cmu.edu/~ece732/s24/homework/bibifi/overview.html</a>


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

# Running the Program
