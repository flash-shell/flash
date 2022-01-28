#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "io_handler.h"

int main(void) {
    const char *ORIGINAL_PATH = getenv("PATH");

    while (true) {
        char *token[512];
        int tokenCount = 0;

        display_prompt();
        break_to_command(token, &tokenCount, ORIGINAL_PATH);
        handle_commands(token, tokenCount, ORIGINAL_PATH);
    }
    return 0;
}
