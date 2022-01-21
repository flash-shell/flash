/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: Main executable of flash shell. 
 *
 *        Version:  0.1
 *        Created:  19/01/22 22:58:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Angus Logan
 *   Organization:  Best group in CS210 :sunglasses:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main(void) {
    while (true) {
        char USERNAME[512];
        USERNAME[511] = '\0';
        char HOSTNAME[512];
	HOSTNAME[511] = '\0';
        char input[512];
        char *p;

        getlogin_r(USERNAME, 511);
        gethostname(HOSTNAME, 511);

        printf("%s@%s$ ", USERNAME, HOSTNAME);
        if (fgets(input, 512, stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        // This if statement here checks to see if the last character in the buffer "input"
        // is the newline character, and if it is it then removes the character, allowing
        // for comparisons.
        if ((p = strchr(input, '\n')) != NULL)
            *p = '\0';

        if (strcmp(input, "exit") == 0)
	    exit(0);
    }
    return 0;
}
