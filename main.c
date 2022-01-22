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
#include "command_handler.h"

int main(void) {
    while (true) {
        char *token[256];
        int tokenCount = 0;

        display_prompt();
        break_to_command(token, &tokenCount);
        handle_commands(token, tokenCount);
    }
    return 0;
}
