#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int main(void) {
    bool cont = true;
    while (cont == true) {
        const char delims[] = " \t|><&;";
        char *tmp[256];
        int i = 0;

        char USERNAME[512];
        USERNAME[511] = '\0';
        char HOSTNAME[512];
        HOSTNAME[511] = '\0';
        char input[512];
        char *p;

        getlogin_r(USERNAME, 511);
        gethostname(HOSTNAME, 511);

        printf("%s@%s$ ", USERNAME, HOSTNAME);

        /**
            sizeof input is used rather than a hard-coded variable, for good practice.
        */

        if(fgets(input, sizeof input, stdin)) {
            tmp[i] = strtok(input, delims);

            while(tmp[i] != NULL) {
                /**
                    This if statement here checks to see if the last character in the buffer "tmp[i]"
                    is the newline character, and if it is it then removes the character, allowing
                    for comparisons.
                */

                if ((p = strchr(tmp[i], '\n')) != NULL)
                    *p = '\0';

                /**
                    This last if statement, compares the value of tmp[i] to the word "exit", hence checks
                    whether the user has typed "exit", if so, cont will be set to false and the shell
                    will cease to run.
                */

                if (strcmp(tmp[i], "exit") == 0)
                    cont = false;

                i++;
                tmp[i] = strtok(NULL, delims); // Here, "NULL" is passed as the first arg instead of the input, as strtok() has an internal state (static pointer) which remembers the last input.
            }
        }
    }
    return 0;
}