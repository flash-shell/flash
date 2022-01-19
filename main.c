/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: Main executable of flash shell. 
 *
 *        Version:  1.0
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

int main(void) {
    bool cont = true;
    while (cont == true) {
        char USERNAME[] = "user";
        char COMPUTER[] = "comp";
        char input[512];
        printf("%s@%s$ ", USERNAME, COMPUTER);
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) cont = false;
    }
    return 0;
}
