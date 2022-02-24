#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "history.h"
#include "io_handler.h"

int main(void) {
    const char *ORIGINAL_PATH = getenv("PATH");

    chdir(getenv("HOME"));

    int count =  1;
    int pos = 0;
    Node *history = malloc(sizeof(Node) * 20);

    while (true) {
        char *token[512];
        int tokenCount = 0;

        display_prompt();
        break_to_command(token, &tokenCount, ORIGINAL_PATH, &count, &pos, history);
        handle_commands(token, tokenCount, ORIGINAL_PATH, history);
    }
    return 0;
}