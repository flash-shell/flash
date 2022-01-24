#include "command_handler.h"
#include "tokenise.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void handle_commands(char **token, int no_commands) {
    for (int i = 0; i < no_commands; i++) {
        if (strcmp(token[i], "exit") == 0) 
            exit(0);
            
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

            _exit(EXIT_FAILURE);
        }
    }
}