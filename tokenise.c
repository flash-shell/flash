#include "tokenise.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

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
    char displaystring[512];
    args[*argCount] = strtok(input, delim);

    while(args[*argCount] != NULL) {
        (*argCount)++;
        args[*argCount] = strtok(NULL, delim);
    }

    args[(*argCount)+1] = NULL;

    int quote = '"';
    int start_at = 0;
    int one_word = 0;

    for (int i = 0; i < *argCount; i++) {
        if(strchr(args[i], quote) != NULL) {
            start_at = i;
            break;
        }
    }

    if (start_at > 0) {
        for (int i = start_at; i < *argCount; i++) {
            if(strchr(args[i], quote) != NULL) {
                int start = i;
                for(int j = (start + 1); j < *argCount; j++) {
                    if(strchr(args[j], quote) != NULL){
                        one_word = 1;
                        sprintf(args[start], "%s %s", args[start], args[j]);
                        break;
                    }
                    sprintf(args[start], "%s %s", args[start], args[j]);
                }
                if (one_word != 1) {
                    // tbd
                } else {
                    //tbd
                }
            }
            break;
        }
        args[start_at + 1] = '\0';
    }
}