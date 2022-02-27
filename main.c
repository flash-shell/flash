#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "io_handler.h"

int main(void) {
    const char *ORIGINAL_PATH = getenv("PATH");

    chdir(getenv("HOME"));

    while (true) {
        char *token[512];
        char *tempNewToken[512];
        int tokenCount = 0;

        display_prompt();
        break_to_command(token, &tokenCount, ORIGINAL_PATH);
        handle_commands(token, tempNewToken, &tokenCount, tokenCount, ORIGINAL_PATH);

        /**
         * memset() is used on both 'token' and 'tempNewToken' which essentially
         * frees the memory assigned to the arrays after commands are handles from
         * user input. Without memory being freed, the aliases do not work correctly.
         */ 

        memset(&token, 0, sizeof token);
        memset(&tempNewToken, 0, sizeof token);
    }
    return 0;
}