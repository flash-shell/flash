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

    char tempArray[256];
    char tempArray2[256];

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

    /**
     * Assign pointers to both temp variables.
     */ 

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

void show_aliases() {}