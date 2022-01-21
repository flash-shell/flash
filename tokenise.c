#include "tokenise.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void tokenise(char **token, int *tokenCount) {
    char input[512];
    char *p;
    const char delims[] = " \t|><&;";

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