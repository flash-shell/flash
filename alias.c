#include "alias.h"
#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

struct alias_struct {
    char alias[256];
    char command[256];
    UT_hash_handle hh;
};

struct alias_struct *aliases = NULL;

void create_alias(char **token) {
    bool exists;
    char *alias_val;

    /**
     * Both temporary arrays are set to the size of tokens,
     * otherwise the strings become malformed due to memory issues.
     */ 

    char tempArray[sizeof token[1]];
    char tempArray2[sizeof token[2]];

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
    strcpy(tempArray2, token[2]);

    alias_val = tempArray;
    exists = alias_exists(exists, alias_val);

    /**
     * Checkes whether the given alias key exists,
     * if it does it throws an error to the user, else
     * it binds it.
     */

    if (exists == false) {
        bind_alias(tempArray, tempArray2);
    } else {
        printf("The alias already exists. Please choose a different name!\n");
    }
}

void swap_token(char **token) {
    struct alias_struct *a;
    char *alias_val2;
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
        strcpy(token[0], a->command);
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

void bind_alias(char temp[256], char temp2[256]) {
    struct alias_struct *a;

    a = (struct alias_struct*)malloc(sizeof *a);
    strcpy((*a).alias, temp);
    HASH_ADD_STR(aliases, alias, a);

    strcpy(a->command, temp2);
}

struct alias_struct *find_alias(char *alias_val) {
    struct alias_struct *a;

    HASH_FIND_STR(aliases, alias_val, a);

    return a;
}

void show_aliases() {
    struct alias_struct *a;

    for (a = aliases; a != NULL; a = (struct alias_struct*) (a->hh.next)) {
        printf("Alias: %s: Command: %s\n", a->alias, a->command);
    }
}