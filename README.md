# A Simple Linux Shell

This is a C program that runs a simple shell. While running, the program prompts the user with **linux(ea152)|>** where the user can either write a linux command, or a custom command (see below). These commands run as a subprocess in the program. The only command that runs in the background is S which opens firefox and prompts the user for another command at the same time. Note, commands are case sensitive and are defaulted to take capital letters only. After the command runs, the program prompts the user for another command. Also the program has error handling where if the user writes a command that can't run, the program specifies so.

# Compiling and Running

Included, there's a makefile. Therefore, to run the code, the user can either run
> make run

Otherwise, the user can run the code manually:

> gcc myshell.c -o myshell

> ./myshell


Also note that the user can limit the number of threads that the program can use by using the command:
> ulimit –u \<number\>

where \<number\> can be replaced with the number of threads allowed to run

# Explination

To use the shell, simply run the program (maybe using *make run*), then the user will be prompted to enter a command. The user can enter any legal linux command or any one of the custom command shortcuts (see below). After the command has executed, the user is prompted to enter the next command. This goes on endlessly until the program is interrupted (with control + C) or the user enters "Q" to quit the shell, in which case, the user goes back to the main shell from before the program ran.

# Custom Command Shortcuts

Of course, any linux command works, however these specific commands are also accepted:


- H - help; display the user manual
- C - copy file1 file2 (takes two arguments -- filenames)
- D - delete a file (takes one argument -- filename)
- M - create the a text file by launching a text editor (takes one argument -- filename)
- P - display the contents of the named file on screen (takes one argument -- filename)
- S - launch firefox
- W - clear the console
- X - execute a program (takes any number of arguments needed to launch program)
- E - display a comment on screen (takes any number of arguments -- the comment that will be echoed)
- L - list the contents of the current directory
- Q - quit the shell

To execute a command, type the command letter followed by the arguments
For example, to copy a file, type
> C file1 file2

Another example, to execute a program, type
> X programName

### Sample (funny) use of X command
> X make run

or
> X ./myshell

this will run the shell program in the shell. You can keep doing this endlessly until your system runs out of threads in which case you would have a shell in a shell in a shell.... etc.

### Additional Notes
- Commands are case-sensitive.
- File paths should be provided relative to the current directory
- Other linux commands are also accepted, for example you may type in \"ls -1\" or \"man\"
- No outside resources were used to complete this project


