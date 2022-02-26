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

        memset(&token, 0, sizeof token);
        memset(&tempNewToken, 0, sizeof token);
    }
    return 0;
}