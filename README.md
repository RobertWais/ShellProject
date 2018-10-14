# CS441/541 Project 3

## Author(s):

Robert Wais, Andrew Smith


## Date:

2018-10-14


## Description:

This program is a reproduction of the shell. It has five built-in commands:
wait, fg, history, jobs, and exit. Anything else put into the command line is
treated as an executable. File redirection is implemented with '<' and '>'.


## How to build the software

Run make.


## How to use the software

Start the program with ./mysh for interactive mode. Then, the program can be
used just as any other shell. The wait command waits until all jobs are
complete. The fg command brings the specified job to the foreground or the most
recent if none was specified. History lists all the jobs run. Jobs lists
currently running jobs, and exit waits for all current jobs to finish before
exiting the program. Batch mode is also available if one or more files are
indicated along the initial ./mysh command.


## How the software was tested

Tested variety of commands and executables against the behavior of a MacBook's
Terminal. 


## Known bugs and problem areas

None