#include "tokenise.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void break_to_arg(char **arg, int *argCount, char *input) {
    const char delim[] = " ";
    char displaystring[512];
    arg[*argCount] = strtok(input, delim);

    while(arg[*argCount] != NULL) {
        (*argCount)++;
        arg[*argCount] = strtok(NULL, delim);
    }

    arg[(*argCount)+1] = NULL;

    int quote = '"';
    for(int i = 0; i < *argCount; i++) {
        if(strchr(arg[i], quote) != NULL) {
            int start = i;
            for(int j = (start + 1); j < *argCount; j++) {
                if(strchr(arg[j], quote) != NULL){
                    strcat(arg[start], arg[j]);
                    printf("%s\n", arg[start]);
                    break;
                }
                strcat(arg[start], arg[j]);
            }
            printf("%s\n", arg[start]);
        }
        break;
    }
}