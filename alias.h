#include "uthash.h"

#define true 1
#define false 0
typedef int bool;

struct alias_struct {
    char alias[256];
    char command[256];
    UT_hash_handle hh;
};

void create_alias(char **token);
bool alias_exists(bool exists, char *alias_val);
void bind_alias(char temp[256], char temp2[256]);
void empty_alias(struct alias_struct *alias);
void swap_token(char **token);
void show_aliases();

struct alias_struct *find_alias(char *alias_val);