#include "uthash.h"

#define true 1
#define false 0
typedef int bool;

struct alias_struct {
    char alias[512];
    char command[512];
    UT_hash_handle hh;
};

void create_alias(char **token, int no_token);
bool alias_exists(bool exists, char *alias_val);
void bind_alias(char temp[512], char *slicedToken[512], int no_token);
void empty_alias(struct alias_struct *alias);
void swap_token(char **token);
void unalias(char **token);
bool count_users(bool exists);
void show_aliases();

struct alias_struct *find_alias(char *alias_val);