#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include "header.h"

int main(void) {
    const char *ORIGINAL_PATH = getenv("PATH");
    chdir(getenv("HOME"));
    loadAlias();

    int count = 1;
    int pos = 0;
    Node *history = malloc(sizeof(Node) * 20);
    loadHistory(history, &count, &pos);

    while (true) {
        char *token[512];
        char *tokenCopy[512];
        int tokenCount = 0;

        display_prompt();
        break_to_command(token, tokenCopy, &tokenCount, ORIGINAL_PATH, &count, &pos, history);
        handle_commands(token, tokenCopy, tokenCount, ORIGINAL_PATH, &count, &pos, history);

        /**
         * memset() is used on both 'token' and 'tokenCopy' which essentially
         * frees the memory assigned to the arrays after commands are handled from
         * user input. Without memory being freed, the aliases do not work correctly.
         */ 

        memset(&token, 0, sizeof token);
        memset(&tokenCopy, 0, sizeof token);
        
    }
    return 0;
}

void display_prompt() {
    char USERNAME[512];
    char HOSTNAME[512];

    // Initialised cwd as "cwd_default" as compiler warns when an unitialised variable used in getcwd()
    char cwd[512] = "cwd_default";  
    getcwd(cwd, sizeof(cwd));
    
    USERNAME[511] = '\0';
    HOSTNAME[511] = '\0';

    /**
     * getlogin_r() has faults on WSL, in which a random set of character is displayed rather than the username,
     * this is due to the nature of the shell, hence if it's not equal to 0 then it's set to "default".
     */
    
    getlogin_r(USERNAME, 511);
    if (getlogin_r(USERNAME, sizeof(USERNAME)) != 0) {
        strncpy(USERNAME, "default", sizeof(USERNAME));
    }

    gethostname(HOSTNAME, 511);

    printf("\033[0;32m");
    printf("%s", USERNAME);

    printf("\033[0;36m");
    printf("@");

    printf("\033[0;32m");
    printf("%s:", HOSTNAME);

    printf("\033[0;34m");
    printf("%s", cwd);
        
    printf("\033[0m");
    printf(" $ ");
}

void break_to_command(char **token, char **tempNewToken, int *tokenCount, const char *ORIGINAL_PATH, int *count, int *pos, Node* history) {
    char input[512];    

    /*
     *  Initially, fgets reads a line from the stream, with sizeof input used
     *  rather than a hard-coded variable, for good practice. It keeps reading
     *  until it hits NULL, which mainly serves as a purpose of not allowing
     *  <ctrl + d> to crash the shell, as that throws an EOF.
     */

    if (fgets(input, sizeof input, stdin) != NULL) {
        tokenizing_process(token, tempNewToken, tokenCount, count, pos, history, input);
    } else {
        printf("\n");
        setenv("PWD", ORIGINAL_PATH, 1);
        saveAlias();
        saveHistory(history);
        exit(0);

    }

    swap_token(token, tempNewToken, tokenCount);
}

void tokenizing_process(char **token, char **tempNewToken, int *tokenCount, int *count, int *pos, Node* history, char *input) {
    const char delims[] = "\t|><&; ";
    char *p;

    if ((p = strchr(input, '\n')) != NULL)
            *p = '\0';

        token[*tokenCount] = strtok(input, delims);

        while (token[*tokenCount] != NULL) {
            (*tokenCount)++;
            /*
             *  Here, "NULL" is passed as the first arg instead of the input, as strtok() has an
             *  internal state (static pointer) which remembers the last input.
             */
            token[*tokenCount] = strtok(NULL, delims);
        }

        if (input[0] != '!' && input[0]) {
            addNode(history, *count, *pos, token, *tokenCount);
            *count = *count + 1;      
            *pos = (*pos + 1) % 20;
        }
}

void handle_commands(char **token, char **tempNewToken, int no_token, const char *ORIGINAL_PATH, int *count, int* pos, Node* history) {
    // check tokens for commands execvp()won't recognise
    recheck_aliases(token, tempNewToken, no_token);

    int counter = 0;
        
    for (int i = 0; i < no_token; i++) {
        if (counter>10) {
            printf("System error. Infinite history invocations\n");
            exit(0);
        }
        if (token[i][0] == '!') {
            if (token[i+1] != NULL) {
                printf("Error. History invocation has too many arguements\n");
                return;
            } if (strlen(token[i]) == 1) {
                printf("Error. Invalid history invocation argument. Use !<no> or !-<no> or !!\n");
                return;
            }

            if (strcmp(token[i], "!!") == 0) {
                if (get(history, *count - 1, token) == 0) {
                    return;
                } else {
                    i--;
                }
            } else if (token[i][1] == '-') {
                if (check_number(&token[i][2]) == 1) {
                    int id = *count - atoi(&token[i][2]) ;
                    if (get(history, id, token) == 0) {
                        return;
                    } else {
                        i--;
                    }
                } else {
                    printf("Error. Invalid history invocation argument. Use !<no> or !-<no> or !!\n");
                    return;
                }
            } else if (check_number(&token[i][1]) == 1) {
                int id = atoi(&token[i][1]);
                if (get(history, id, token) == 0) {
                    return;
                } else {
                    i--;
                }
            } else {
                printf("Error. Invalid history invocation argument. Use !<no> or !-<no> or !!\n");
                return;
            }
            counter ++;
        }

        recheck_aliases(token, tempNewToken, no_token);

        if (strcmp(token[i], "exit") == 0) {
            setenv("PWD", ORIGINAL_PATH, 1);
            exit(0);
        }

        if (strcmp(token[i], "cd") == 0) {
           if (token[i+1] == NULL) {
                chdir(getenv("HOME"));
            } else {

                /*
                * Here we check to see if the directory exists
                * by using F_OK and Access which return 0 if the 
                * directory exists 
                */
               
                if (token[i+2] != NULL)
                    printf("Error. \"cd\" requires exactly one argument.\n");
                else if (access(token[i+1], F_OK) == 0) {
                    chdir(token[i+1]);
                    if (errno != 0) {
                        perror(token[i+1]);
                    }
                } else {
                    perror(token[i+1]);
                    printf("Please select an existing file or directory.\n");
                }
            }
            return;
        }

        if (strcmp(token[i], "alias") == 0) {
            if (token[i+1] == NULL) {
                show_aliases();
            } else {
                create_alias(token, no_token);
            }
            return;
        }

        if (strcmp(token[i], "unalias") == 0) {
            if (token[i+1] != NULL) {
                unalias(token);
            } else {
                printf("Please provide the alias name to remove.\n");
            }
            return;
        }

        if (strcmp(token[i], "getpath") == 0) {
            if (token[i+1] != NULL) {
                printf("Error. \"getpath\" does not take arguments.\n");
                return;
            }
            printf("%s\n", getenv("PATH"));
            return;
        }

        if (strcmp(token[i], "setpath") == 0) {
            if (token[i+2] != NULL) {
                printf("Error. \"setpath\" requires exactly one argument.\n");
                return;
            }
            if (token[i+1] != NULL) {
                setenv("PATH", token[i+1], 1);
                return;
            }
            printf("Error. \"setpath\" requires an argument.\n");
            return;
        }

        if (strcmp(token[i], "history") == 0) {
            if (token[i+1] != NULL) {
                printf("Error. History doesn't take arguments\n");
                return;
            }
            printNodes(history);
            return;
        }
    }
        
    pid_t child_pid = fork();
    if (child_pid == -1) {
        printf("Error. Failed to fork.");
    } else if (child_pid > 0) {
        // Parent process
        int status;
        /* 
         *  waitpid() waits for a child_pid to finish executing
         *  necessary as without it the parent process would
         *  continue executing which could cause issues
         */
        waitpid(child_pid, &status, 0);
    } else {
        // Child process

        /*
         *  When execvp() is not successful, a -1 is returned, therefore,
         *  if a command is not found an error is displayed to the terminal with the arg name via perror().
         */

        if (execvp(token[0], token) < 0) {
            perror(token[0]);
        }

        fflush(stdout); // Output stream is flushed so terminal can continue displaying statements.
        _exit(EXIT_FAILURE);
    }
}

/**
 * This function serves to recheck aliases, with regards to aliasing aliases.
 * If a token exists (a user input), while there exists an alias with that token (it gets swapped)
 * then continue to swap it out, and if counter reach > 10 then an infinite alias is found,
 * as there is a max limit of 10 aliases, then the system exits.
 */

void recheck_aliases(char **token, char **tempNewToken, int no_token) {
    int counter = 0;

    if (token[0] != NULL) {
        while (alias_exists(token[0])) {
            if (!(counter > 10 )) {
                swap_token(token, tempNewToken, &no_token);
                counter++;
            } else {
                printf("System error. Infinite alias found.\n");
                exit(0);
            }
        }
    }
}

/**
 * Ensures that the string entered is between 0 and 9, if so,
 * it returns a 0 else 1.
 */ 

int check_number(char* string) {
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] < '0' || string[i] > '9') {
            return 0;
        }
    }
    return 1;
}