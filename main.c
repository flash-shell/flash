/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *     Description: Main executable of flash shell. 
 *
 *        Version:  0.1
 *        Created:  19/01/22 22:58:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Authors:  Angus Logan, Kamil Zak, George O'Brien, James McLean, Jude Craig
 *   Organization:  Best group in CS210 :sunglasses:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "prompt.h"
#include "tokenise.h"

int main(void) {
    while (true) {
        char input[512];
        char *p;
        const char delims[] = " \t|><&;";
        char *token[256];
        int tokenCount = 0;

        display_prompt();

        tokenise(*token, tokenCount);

        // // sizeof input is used rather than a hard-coded variable, for good practice.
        // if(fgets(input, sizeof input, stdin) != NULL) {
        //     // If last char in buffer is newline, replace it with end of line to allow for comparisons
        //     if ((p = strchr(input, '\n')) != NULL)
        //         *p = '\0';
        //     token[tokenCount] = strtok(input, delims);

        //     while(token[tokenCount] != NULL) {
        //         tokenCount++;
        //         /*
        //          *  Here, "NULL" is passed as the first arg instead of the input, as strtok() has an
        //          *  internal state (static pointer) which remembers the last input.
        //          */
        //         token[tokenCount] = strtok(NULL, delims); 
        //     }
        // } else {
        //     printf("\n");
        //     exit(0);
        // }

        for (int i = 0; i < tokenCount; i++) {
            if (strcmp(token[i], "exit") == 0)
                exit(0);
        }
    }
    return 0;
}
