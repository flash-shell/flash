#include "command_handler.h"
#include <string.h>
#include <stdlib.h>

void handle_commands(char **token, int no_commands) {
    for (int i = 0; i < no_commands; i++) {
            if (strcmp(token[i], "exit") == 0)
                exit(0);
        }
}