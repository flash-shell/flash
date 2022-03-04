#include "alias.h"
#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

struct alias_struct *aliases = NULL;

void create_alias(char **token, int no_token) {
    char *aliasValue;
    struct alias_struct *aliasStruct;
    char *slicedToken[512];

    /**
     * slicedToken is a partial token in which it contains all tokens
     * starting from index 2, that is only the value in the key:value pair,
     * for example: "alias testalias pwd", "pwd" would be the only thing
     * in the slicedToken.
     */

    for (int i = 0; i < no_token; i++) {
        if (token[i] != NULL) {
            slicedToken[i] = token[i + 2];
        }
    }

    /**
     * Ensures that there is two commands present after 'alias'
     * otherwise an informative print message is thrown to the
     * user.
     */

    if (token[1] == NULL) {
        printf("Please include an alias command.\n");
        return;
    } else if (token[2] == NULL) {
        printf("Please include command to be mapped to.\n");
        return;
    }

    /**
     * Copies over token[1] into a aliasToken of size token[1] in order
     * to later copy values from a char array to another char array, which token
     * is not by default.
     */ 

    char aliasToken[sizeof token[1]];
    strcpy(aliasToken, token[1]);
    aliasValue = aliasToken;

    /**
     * Checks whether the given alias key exists,
     * if it does then it prints a message to the user notifying them that
     * a key was overwritten. Then it proceeds to find the alias, empty it, and bind
     * the new alias. Otherwise, if it does not exist, it checks whether the 10 alias
     * limit has been reached, if so, it throws an error. If not, it binds the alias.
     */ 

    if (alias_exists(aliasValue)) {
        printf("There was an alias with that name. Your alias was overwritten!\n");
        aliasStruct = find_alias(aliasValue);
        empty_alias(aliasStruct);
        bind_alias(aliasToken, slicedToken, no_token);
    } else {
        if (alias_limit_reached()) {
            printf("There already exist 10 aliases. Please unalias some commands to add more.\n");
        } else {
            bind_alias(aliasToken, slicedToken, no_token);
        }
    }
}

void swap_token(char **token, char **tokenCopy) {
    char *aliasValue;
    struct alias_struct *aliasStruct;
    char commandHolder[512];
    char *p;
    const char delims[] = "\t|><&; ";
    int tokenCount = 0;
    int *tokenPointer = &tokenCount;

    /**
     * A temporary array is declared, and is set to the size of tokens,
     * otherwise the strings become malformed due to memory issues.
     */

    char temporaryToken[sizeof token[0]];
    strcpy(temporaryToken, token[0]);
    aliasValue = temporaryToken;

    /**
     * First, the if statement only runs if alias_exists. alias_exists() returns either a
     * true or false value set by the custom typedef of bool. It starts of by finding the alias
     * as it already knows it exists, which is then copied into a temporary array of size 512, as
     * without it the aliasStruct->command becomes malformed again due to memory issues. Then it tokenizes
     * the value of the key.
     */ 

    if (alias_exists(aliasValue)) {
        aliasStruct = find_alias(aliasValue);
        strcpy(commandHolder, aliasStruct->command);

        // If last character of commandHolder is newline, replace it with end of line to allow for comparisons
        if ((p = strchr(commandHolder, '\n')) != NULL) {
            *p = '\0';
        }

        tokenCopy[*tokenPointer] = strtok(commandHolder, delims);

        while(tokenCopy[*tokenPointer] != NULL) {
            (*tokenPointer)++;

            /*
             *  Here, "NULL" is passed as the first arg instead of the input, as strtok() has an
             *  internal state (static pointer) which remembers the last input.
             */

            tokenCopy[*tokenPointer] = strtok(NULL, delims);
        }

        strcpy(token[0], tokenCopy[0]);

        /**
         * This for loops serves to loop over the amount of tokens that is contained within the current key->value pair, which is defined in
         * the struct. Essentially, it swaps the temporary tokens (which were previously tokenized) into the actual tokens which will be used
         * after the function is finished. If a token does not exist (that is, if the user only types in a single word, for example), then the
         * tokens are simply assigned a new value. This will happen in the case of a single worded alias executing multiple commands/arguments.
         */

        for(int i = 0; i < aliasStruct->no_of_tokens; i++) {
            if (token[i] != NULL) {
                strcpy(token[i], tokenCopy[i]);
            } else {
                token[i] = tokenCopy[i];
            }
        }
    }
}

bool alias_exists(char *alias_val) {

    /**
     * HASH_FIND_STR() looks for the key:value pair, if not found,
     * exists is set to false, otherwise it's set to true and
     * is then returned.
     */ 

    struct alias_struct *aliasStruct;
    bool exists;

    HASH_FIND_STR(aliases, alias_val, aliasStruct);

    if (aliasStruct == NULL) {
        exists = false;
    } else {exists = true; }

    return exists;
}

void bind_alias(char aliasToken[512], char *slicedToken[512], int no_token) {
    struct alias_struct *aliasStruct;
    char listOfCommands[512];
    // Sets the memory to 0 as sometimes on Linux systems it holds random data on declaration.
    memset(listOfCommands, 0, sizeof listOfCommands); 

    /**
     * First, a is declared as a pointer to alias_struct, in which then
     * the value from temp is copied into it (which holds the token of index 1, the alias name).
     * It then adds the string to the hashmap via HASH_ADD_STR().
     */ 

    aliasStruct = (struct alias_struct*)malloc(sizeof *aliasStruct);
    strcpy((*aliasStruct).alias, aliasToken);
    HASH_ADD_STR(aliases, alias, aliasStruct);

    /**
     * Loops over (no_token - 2) times, since it serves to append the value to the key,
     * and the values start at token[2], for example: "alias testalias pwd", in which
     * token[2] would be the start of the values for the alias.
     */ 

    memset(&aliasStruct->no_of_tokens, 0, sizeof aliasStruct->no_of_tokens);


    for(int i = 0; i < (no_token - 2); i++) {
        strcat(listOfCommands, slicedToken[i]); // The current iteration index of slicedToken is concatenated into listOfCommands.
        aliasStruct->no_of_tokens++; // no_of_tokens is incremented, to later be used to swap tokens correctly.

        if (i != (no_token - 3)) {
            strcat(listOfCommands, " "); // a space is appended in between tokens.
        }
    }

    /**
     * The list of commands is the copied into a->command 
     * (which holds the value of the key:value pair),
     */ 

    strcpy(aliasStruct->command, listOfCommands);
}

void empty_alias(struct alias_struct *alias) {

    /**
     * HashMap pair is deleted, and memory is freed.
     */

    HASH_DEL(aliases, alias);
    free(alias);
}

struct alias_struct *find_alias(char *alias_val) {
    struct alias_struct *aliasStruct;

    /**
     * HASH_FIND_STR() finds the key:value struct, 
     * assigns it to "a" and returns it.
     */

    HASH_FIND_STR(aliases, alias_val, aliasStruct);

    return aliasStruct;
}

void show_aliases() {
    struct alias_struct *a;

    /**
     * Loops over all aliases and prints them out to the user,
     * in an aesthetic way.
     */ 

    if ((a=aliases) == NULL){
        printf("No aliases exist yet!\n");
    }

    for (a = aliases; a != NULL; a = (struct alias_struct*) (a->hh.next)) {
        printf("Alias: %s | Command: %s\n", a->alias, a->command);
    }
}

void unalias(char **token) {

    /**
     * Currently very inefficient (will be fixed!), a temporaryArray is created
     * with the size of a singular token, which then the contens of token[1] are
     * copied into the temp array. Then, a char pointer is assigned to point to the temp
     * array which is then passed into find_alias(). This is because find_alias() has to take
     * a char pointer at the moment.
     */ 

    char *aliasValue;
    struct alias_struct *aliasStruct;

    char aliasKey[128];

    strcpy(aliasKey, token[1]);

    aliasValue = aliasKey;
    aliasStruct = find_alias(aliasValue);

    /**
     * find_alias() returns a pointer to a struct, if it's NULL it means
     * it does not exist, hence a message is thrown to the user
     * of the shell, otherwise, the alias is deleted via empty_alias().
     */ 

    if (aliasStruct == NULL) {
        printf("This alias name does not exist. Please try again.\n");
    } else {
        empty_alias(aliasStruct);
    }
}

bool alias_limit_reached() {
    int num_of_aliases;
    num_of_aliases = HASH_COUNT(aliases); // HASH_COUNT returns the number of key:value pairs in the HashMap.
    bool limitReached;

    /**
     * Checks if the num of aliases is equal to 9 (indexing starts at 0),
     * if yes (that means there is 10 aliases), then limitReached is set
     * to true.
     */

    if (num_of_aliases == 10) {
        limitReached = true;
    } else {
        limitReached = false;
    }

    return limitReached;
}
