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

    getlogin_r(USERNAME, 511);
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

    // sizeof input is used rather than a hard-coded variable, for good practice.
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

    while(args[*argCount] != NULL) {
        (*argCount)++;
        args[*argCount] = strtok(NULL, delim);
    }

    args[(*argCount)+1] = NULL;

    int quote = '"';
    int quote_index = first_quote_occurence(args, argCount, quote);

    if (quote_index > 0) {
        for (int start = quote_index; start < *argCount; start++) {
            for(int j = (start + 1); j < *argCount; j++) {
                memcpy(word, args[start], strlen(args[start]));
                if(strchr(args[j], quote) != NULL){
                    // sets args[start] to (args[start] + " " + args[j])
                    sprintf(args[start], "%s %s", word, args[j]);
                break;
                }
                sprintf(args[start], "%s %s", word, args[j]);
            }
            break;
        }
        args[quote_index + 1] = '\0';
    }
}

int first_quote_occurence(char **args, int *argCount, int quote) {
    int index = 0;

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
        
        char *cd_args[512];
        int cd_argCount = 0;
        break_to_arg(cd_args, &cd_argCount, token[i]);
        for(int i = 0; i < cd_argCount; i++) {
            if (strcmp(cd_args[i], "cd") == 0)
                chdir(cd_args[i+1]);
        }
            
        pid_t child_pid = fork();
        if (child_pid == -1) {
            printf("Error. Failed to fork.");
        } else if (child_pid > 0) {
            // parent process
            int status;
            /* 
             *  waitpid waits for a child_pid to finish executing
             *  necessary as without it the parent process would
             *  continue executing which could cause issues
             */
            waitpid(child_pid, &status, 0);
        } else {
            // child process
            char *args[512];
            int argCount = 0;
            break_to_arg(args, &argCount, token[i]);

            if (execvp(args[0], args) < 0) {
                fprintf(stderr, "%s: Command not found\n", args[0]);
            }
            fflush(stdout);

            _exit(EXIT_FAILURE);
        }
    }
}