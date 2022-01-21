#include "prompt.h"
#include <unistd.h>
#include <stdio.h>

void display_prompt() {
    char USERNAME[512];
    char HOSTNAME[512];

    USERNAME[511] = '\0';
    HOSTNAME[511] = '\0';

    getlogin_r(USERNAME, 511);
    gethostname(HOSTNAME, 511);

    printf("\033[0;32m");
    printf("%s", USERNAME);

    printf("\033[0;36m");
    printf("@");

    printf("\033[0;32m");
    printf("%s", HOSTNAME);

    printf("\033[0m");
    printf(" $ ");
}