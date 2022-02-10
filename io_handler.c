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
    const char delims[] = "\t|><&; ";

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

void handle_commands(char **token, int no_token) {
    for (int i = 0; i < no_token; i++) {
        if (strcmp(token[i], "exit") == 0)
            exit(0);

        if (strcmp(token[i], "cd") == 0)
            chdir(token[i+1]);
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

        if (execvp(token[0], token) < 0) {
            for(int i = 0; i < no_token; i++) {
                if (strcmp(token[0], "cd") == 0) {} else {
                    fprintf(stderr, "%s: command not found\n", token[0]);
                }
            }
        }

        fflush(stdout);

        _exit(EXIT_FAILURE);
    }
}