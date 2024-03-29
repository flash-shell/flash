#include "uthash.h"
#include <stdbool.h>

/*
 *  Type definitions
 */

typedef struct NODE {
    int id;
    char** command;
    int no_token;
}  Node;

struct alias_struct *find_alias(char *alias_val);

#define true 1
#define false 0

struct alias_struct {
    char alias[512];
    char command[512];
    int no_of_tokens;
    UT_hash_handle hh;
};

/*
 * IO Handler functions
 */

void display_prompt();
void break_to_command(char **token, char **tempNewToken, int *tokenCount, const char *ORIGINAL_PATH, int *count, int *pos, Node* history);
void tokenizing_process(char **token, char **tempNewToken, int *tokenCount, int *count, int *pos, Node* history, char *input);
void handle_commands(char **token, char **tempNewToken, int no_commands, const char *ORIGINAL_PATH, int *count, int* pos,  Node* history);
void recheck_aliases(char **token, char **tempNewToken, int no_token);

/*
 *  History functions
 */

int get(Node* arr, int id, char** token);
void addNode(Node* arr,int id, int pos, char** token, int no_commands);
void printNodes(Node* arr);
int getEarliest(Node * arr);
int check_number(char* string);
void saveHistory(Node* arr);
void loadHistory(Node* arr, int *count, int *pos);

/*
 * Aliasing functions
 */

void create_alias(char **token, int no_token);
bool alias_exists(char *alias_val);
void bind_alias(char aliasToken[512], char *slicedToken[512], int no_token);
void empty_alias(struct alias_struct *alias);
void swap_token(char **token, char **tokenCopy, int *no_of_tokens);
void unalias(char **token);
bool alias_limit_reached();
void show_aliases();
void saveAlias();
void loadAlias();