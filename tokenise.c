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
                if(strchr(args[j], quote) != NULL){
                    sprintf(args[start], "%s %s", args[start], args[j]);
                    break;
                }
                sprintf(args[start], "%s %s", args[start], args[j]);
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