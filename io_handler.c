#include "io_handler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>

void display_prompt() {
    char USERNAME[512];
    char HOSTNAME[512];

    USERNAME[511] = '\0';
    HOSTNAME[511] = '\0';

    /**
     * getlogin_r() has faults on WSL, in which a random set of character is displayed rather than the username,
     * this is due to the nature of the shell, hence if it's not equal to 0 then it's set to "defualt".
     */

    if (getlogin_r(USERNAME, sizeof(USERNAME)) != 0) {
        strncpy(USERNAME, "default", sizeof(USERNAME));
    }

    gethostname(HOSTNAME, 511);

    printf("\033[0;32m");
    printf("%s", USERNAME);

    printf("\033[0;36m");
    printf("@");

    printf("\033[0;32m");
    printf("%s", HOSTNAME);

    printf("\033[0m");
    printf(" $ ");
}

void break_to_command(char **token, int *tokenCount) {
    char input[512];
    char *p;
    const char delims[] = "\t|><&;";

    /*
     *  Initially, fgets reads a line from the stream, with sizeof input used
     *  rather than a hard-coded variable, for good practice. It keeps reading
     *  until it hits NULL, which mainly serves as a purpose of not allowing
     *  <ctrl + d> to crash the shell, as that throws an EOF.
     */

    if(fgets(input, sizeof input, stdin) != NULL) {
        // If last char in buffer is newline, replace it with end of line to allow for comparisons
        if ((p = strchr(input, '\n')) != NULL)
            *p = '\0';
        token[*tokenCount] = strtok(input, delims);

        while(token[*tokenCount] != NULL) {
            (*tokenCount)++;

            /*
             *  Here, "NULL" is passed as the first arg instead of the input, as strtok() has an
             *  internal state (static pointer) which remembers the last input.
             */

            token[*tokenCount] = strtok(NULL, delims);
        }
    } else {
        printf("\n");
        exit(0);
    }
}

void break_to_arg(char **args, int *argCount, char *input) {
    const char delim[] = " ";
    char word[64]; 
    args[*argCount] = strtok(input, delim);

    /*
     * Here, like above when breaking the commands, "NULL" is passed as the first arg in strtok()
     * for the same purpose, as it has an internal static pointer which remembers the last input.
     */

    while(args[*argCount] != NULL) {
        (*argCount)++;
        args[*argCount] = strtok(NULL, delim);
    }

    args[(*argCount)+1] = NULL;

    int quote = '"';
    int quote_index = first_quote_occurence(args, argCount, quote);

    /*
     * Initially, it checks whether quote_index is greater than 0, if so,
     * that means a quote exists (checked by first_quote_occurence), it
     * loops over args starting at the index where the first quote
     * was detected. 
     */

    if (quote_index > 0) {
        for (int start = quote_index; start < *argCount; start++) {
            for(int j = (start + 1); j < *argCount; j++) {
                memcpy(word, args[start], strlen(args[start])); // Copies args[start] to word, as sprintf() does not like the same var used twice.
                if(strchr(args[j], quote) != NULL){
                    sprintf(args[start], "%s %s", word, args[j]); // A space is added in between word & args[j] and set to args[start].
                break;
                }
                sprintf(args[start], "%s %s", word, args[j]); // A space is added in between word & args[j] and set to args[start].
            }
            break;
        }
        args[quote_index + 1] = '\0'; // Indication of the termination of args so the unusable args are left behind.
    }
}

int first_quote_occurence(char **args, int *argCount, int quote) {
    int index = 0;

    /*
     * Loops through all the arguments, a search is conducted
     * in every arg, and if a quote character is found
     * then it is returned.
     */

    for (int i = 0; i < *argCount; i++) {
        if(strchr(args[i], quote) != NULL) {
            index = i;
            break;
        }
    }

    return index;
}

void handle_commands(char **token, int no_commands) {
    for (int i = 0; i < no_commands; i++) {
        if (strcmp(token[i], "exit") == 0) 
            exit(0);
            
        pid_t child_pid = fork();
        if (child_pid == -1) {
            printf("Error. Failed to fork.");
        } else if (child_pid > 0) {
            // Parent process
            int status;

            /* 
             *  Waitpid waits for a child_pid to finish executing
             *  necessary as without it the parent process would
             *  continue executing which could cause issues.
             */

            waitpid(child_pid, &status, 0);
        } else {
            // Child process
            char *args[512];
            int argCount = 0;
            break_to_arg(args, &argCount, token[i]);

            /*
            *  When execvp() is not successful, a -1 is returned, therefore,
            *  if a command is not found an error is displayed to the terminal with the arg name.
            */

            if (execvp(args[0], args) < 0) {
                fprintf(stderr, "%s: Command not found\n", args[0]);
            }

            fflush(stdout); // Output stream is flushed so terminal can continue displaying statements.
            _exit(EXIT_FAILURE);
        }
    }
}