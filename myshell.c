// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:________Elia Albaba______________________ Date:__2/22/24__

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_ARGS 64
#define MAX_ARG_LEN 16
#define MAX_LINE_LEN 80
#define WHITESPACE " ,\t\n"

struct command_t
{
    char *name;
    int argc;
    char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
struct command_t translateCommand(struct command_t *cmd);

int main(int argc, char *argv[])
{
    int pid;
    int status;
    char cmdLine[MAX_LINE_LEN];
    struct command_t command;
    char *exitCommand = "Q";

    bool exitFlag = true;

    while (exitFlag)
    {
        bool runInBackground = false;

        printPrompt();
        /* Read the command line and parse it */
        readCommand(cmdLine);
        parseCommand(cmdLine, &command);
        command.argv[command.argc] = NULL;
        if (command.name && exitCommand) {
            // if the command is Q then exit the shell
            if (strcmp(command.name, exitCommand) == 0) {
                exitFlag = false;
                break;
            } else if(strcmp(command.name, "s") == 0) {
                runInBackground = true;
            }

        }

        command = translateCommand(&command);

        /* Create a child process to execute the command */
        if ((pid = fork()) == 0)
        {
            /* Child executing command */
            // Check if the command gives an error.
            // Implementation detail: The only way command.name is NULL is if the command is 'h' in which case, no need to execute
            if (command.name != NULL && execvp(command.name, command.argv) == -1 )
            {
                printf("There was an error with the command you entered: %s\n", command.name);
                exit(1);
            }
            exit(0);
        }
        if (!runInBackground) {
            /* Wait for the child to terminate */
            waitpid(pid, &status, 0);
        }
    }

    /* Shell termination */
    printf("\n\n shell: Terminating successfully\n");
    return 0;
}

/**
 * Translates a custom shell command shortcut into a corresponding Linux command.
 *
 * This function takes a command_t struct as input, which represents a custom shell command,
 * and translates it into a corresponding Linux command. The function modifies the name and
 * arguments of the command to match the corresponding Linux command based on predefined set
 * of custom commands that can be listed if you run H command.
 *
 * If the custom command is not recognized, the function returns the original command
 * without any modifications which will run as a Linux command.
 *
 * @param cmd A pointer to a command_t struct representing the custom command or Linux command.
 * @return A command_t struct representing the translated Linux command.
 */
struct command_t translateCommand(struct command_t *cmd) {
    struct command_t translatedCommand = *cmd;
    if (strcmp(cmd->name, "C") == 0) {
        // copy file1 file2
        translatedCommand.name = "cp";
    } else if (strcmp(cmd->name, "D") == 0) {
        // Delete the named file
        translatedCommand.name = "rm";
    } else if (strcmp(cmd->name, "H") == 0) {
        // Help; display the user manual, described below
        translatedCommand.name = NULL;
        printf("Welcome to the Shell Manual. This manual provides guidance on using the custom shell with various internal commands.\n");
        printf("This is a basic shell that can execute the following commands:\n");
        printf("H - help; display the user manual\n");
        printf("C - copy file1 file2\n");
        printf("D - delete a file (takes one argument -- filename)\n");
        printf("M - create the a text file by launching a text editor (takes one argument -- filename)\n");
        printf("P - display the contents of the named file on screen (takes one argument -- filename)\n");
        printf("S - launch firefox\n");
        printf("W - clear the console\n");
        printf("X - execute a program (takes one argument -- program filename)\n");
        printf("E - display a comment on screen followed by a new line (takes any number of arguments -- the comment that will be echoed)\n");
        printf("L - list the contents of the current directory\n");
        printf("Q - quit the shell\n");
        printf("To execute a command, type the command letter followed by the arguments\n");
        printf("For example, to copy a file, type 'C file1 file2'\n");
        printf("Another example, to execute a program, type 'X programName'\n");
        printf("Additional Notes\n");
        printf("Commands are case-sensitive.\n");
        printf("File paths should be provided relative to the current directory\n");
        printf("Other linux commands are also accepted, for example you may type in \"ls -1\" or \"man\" \n");
    } else if (strcmp(cmd->name, "M") == 0) {
        // Make; create the named text file by launching a text editor
        translatedCommand.name = "nano";
    } else if (strcmp(cmd->name, "P") == 0) {
        // Print; display the contents of the named file on screen.
        translatedCommand.name = "more";
    } else if (strcmp(cmd->name, "S") == 0) {
        // Surf the web by launching a browser as a background process
        translatedCommand.name = "firefox";
    } else if (strcmp(cmd->name, "W") == 0) {
        // Wipe; clear the screen.
        translatedCommand.name = "clear";
    } else if (strcmp(cmd->name, "X") == 0) {
        // Execute the named program.
        translatedCommand.name = cmd->argv[1];
        translatedCommand.argv[0] = cmd->argv[1];
        translatedCommand.argv[1] = cmd->argv[2];
    } else if (strcmp(cmd->name, "E") == 0) {
        // Echo; display comment on screen followed by a new line (multiple
        // spaces/tabs may be reduced to a single space); if no argument simply
        // issue a new prompt
        translatedCommand.name = "echo";
    } else if (strcmp(cmd->name, "L") == 0) {
        // List the contents of the current directory; see below
        printf("\n");
        int pid;
        if(pid = fork() == 0) {
            translatedCommand.name = "pwd";
            translatedCommand.argv[1] = NULL;
            execvp(translatedCommand.name, translatedCommand.argv);
            exit(0);
        }
        waitpid(pid, NULL, 0);
        printf("\n");
        translatedCommand.name = "ls";
        translatedCommand.argv[1] = "-l";
        translatedCommand.argv[2] = NULL;
    } else {
        return *cmd;
    }
    return translatedCommand;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd)
{
    int argc;
    char **clPtr;
    /* Initialization */
    clPtr = &cLine; /* cLine is the command line */
    argc = 0;
    cmd->argv[argc] = (char *)malloc(MAX_ARG_LEN);
    /* Fill argv[] */
    while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL)
    {
        cmd->argv[++argc] = (char *)malloc(MAX_ARG_LEN);
    }

    /* Set the command name and argc */
    cmd->argc = argc - 1;
    cmd->name = (char *)malloc(sizeof(cmd->argv[0]));
    strcpy(cmd->name, cmd->argv[0]);
    return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt()
{
    /* Build the prompt string to have the machine name,
     * current directory, or other desired information
     */
    char *promptString = "linux(ea152)|>"; /* EDIT THIS LINE */
    printf("%s ", promptString);
}

void readCommand(char *buffer)
{
    /* This code uses any set of I/O functions, such as those in
     * the stdio library to read the entire command line into
     * the buffer. This implementation is greatly simplified,
     * but it does the job.
     */
    fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */