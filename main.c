#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "io_handler.h"

int main(void) {
    while (true) {
        char *token[512];
        int tokenCount = 0;

        display_prompt();
        break_to_command(token, &tokenCount);
        handle_commands(token, tokenCount);
    }
    return 0;
}
