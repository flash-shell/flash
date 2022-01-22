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
        } 
        else if (child_pid > 0) {
        int status;
        waitpid(child_pid, &status, 0);
        }
        else {
            // child process
            char *arg[512];
            int argCount = 0;
            break_to_arg(arg, &argCount, token[i]);

            execvp(arg[0], arg);
            _exit(EXIT_FAILURE);   // exec never returns
        }
    }
}