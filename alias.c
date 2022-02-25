#include "alias.h"
#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

struct alias_struct *aliases = NULL;

void create_alias(char **token, int no_token) {
    bool exists;
    char *alias_val;
    struct alias_struct *astruct;
    char *slicedToken[512];

    /**
     * Assign memcpy to slicedToken to move all the tokens
     * to slicedToken, somewhat similar in style to Python slicing
     * as it starts from index 1 of the original token char array.
     */ 

    for (int i = 0; i < no_token; i++) {
        if (token[i] != NULL) {
            slicedToken[i] = token[i + 2];
        }
    }

    /**
     * Both temporary arrays are set to the size of tokens,
     * otherwise the strings become malformed due to memory issues.
     */ 

    char tempArray[sizeof token[1]];

    /**
     *  Ensures that there is two commands present after 'alias'
     *  otherwise a print message is thrown.
     */

    if (token[1] == NULL) {
        printf("Please include an alias command.\n");
        return;
    } else if (token[2] == NULL) {
        printf("Please include command to be mapped to.\n");
        return;
    }

    /**
     * Copies over token[1] and token [2] respectively,
     * to other variables in order to later copy values
     * from a char array to another char array, which token
     * is not by default.
     */

    strcpy(tempArray, token[1]);

    alias_val = tempArray;
    exists = alias_exists(exists, alias_val);

    /**
     * Checkes whether the given alias key exists,
     * if it does it throws an error to the user, else
     * it binds it.
     */

    if (exists == false) {
        exists = count_users(exists);
        if (exists == false) {
            bind_alias(tempArray, slicedToken, no_token);
        } else {
            printf("There already exist 10 aliases. Please unalias some commands to add more.\n");
        }
    } else {
        printf("There was an alias with that name. Your alias was overwritten!\n");
        astruct = find_alias(alias_val);
        empty_alias(astruct);
        bind_alias(tempArray, slicedToken, no_token);
    }
}

void swap_token(char **token, int no_token) {
    struct alias_struct *a;
    char *alias_val2;
    char *commandPointer;
    char tempWord[64];
    bool tempExists;

    /**
     * Both temporary arrays are set to the size of tokens,
     * otherwise the strings become malformed due to memory issues.
     */

    char tempArray3[sizeof token[0]];
    strcpy(tempArray3, token[0]);

    alias_val2 = tempArray3;
    tempExists = alias_exists(tempExists, alias_val2);

    if (tempExists == true) {
        a = find_alias(alias_val2);
        commandPointer = a->command;

        int length = (int) strlen(commandPointer);

        for (int i = 0; (i < length); i++) {
            if (commandPointer[i] != 32) {
                char d = commandPointer[i];
                char *dPointer = &d;
                strncat(tempWord, dPointer, 1);
                memset(&d, 0, sizeof d);
            } else {
                memset(tempWord, 0, sizeof tempWord);
            }

            printf("%s", tempWord);
            // strcpy(token[i], tempWord);
        }
    }
}

bool alias_exists(bool exists, char *alias_val) {
    struct alias_struct *a;

    HASH_FIND_STR(aliases, alias_val, a);

    if (a == NULL) {
        exists = false;
    } else {exists = true; }

    return exists;
}

void bind_alias(char temp[512], char *slicedToken[512], int no_token) {
    struct alias_struct *a;
    char listOfCommands[512];

    a = (struct alias_struct*)malloc(sizeof *a);
    strcpy((*a).alias, temp);
    HASH_ADD_STR(aliases, alias, a);

    for(int i = 0; i < (no_token - 2); i++) {
        strcat(listOfCommands, slicedToken[i]);
        if (i != (no_token - 3)) {
            strcat(listOfCommands, " ");
        }
    }

    strcpy(a->command, listOfCommands);
    memset(listOfCommands, 0, sizeof listOfCommands);
}

void empty_alias(struct alias_struct *alias) {
    HASH_DEL(aliases, alias);
    free(alias);
}

struct alias_struct *find_alias(char *alias_val) {
    struct alias_struct *a;

    HASH_FIND_STR(aliases, alias_val, a);

    return a;
}

void show_aliases() {
    struct alias_struct *a;

    for (a = aliases; a != NULL; a = (struct alias_struct*) (a->hh.next)) {
        printf("Alias: %s | Command: %s\n", a->alias, a->command);
    }
}

void unalias(char **token) {
    char *alias_val3;
    struct alias_struct *astruct2;

    char tempArray4[sizeof token[1]];
    strcpy(tempArray4, token[1]);

    alias_val3 = tempArray4;
    astruct2 = find_alias(alias_val3);

    if (astruct2 == NULL) {
        printf("This alias name does not exist. Please try again.\n");
    } else {
        empty_alias(astruct2);
    }
}

bool count_users(bool exists) {
    int num_of_aliases;
    num_of_aliases = HASH_COUNT(aliases);

    if (num_of_aliases == 9) {
        exists = true;
    } else {
        exists = false;
    }

    return exists;
}
