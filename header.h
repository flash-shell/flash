/*
 *  Here are the type definitions
 */

typedef struct NODE {
    int id;
    char** command;
    int no_token;
}  Node;

/*
 *  History functions
 */

int get(Node* arr, int id, char** token);
void addNode(Node* arr,int id, int pos, char** token, int no_commands);
void printNodes(Node* arr);
int getEarliest(Node * arr);
int checkNumber(char* string);

/*
 *  Io handler functions
 */

void display_prompt();
void break_to_command(char **token, int *tokenCount, const char *ORIGINAL_PATH, int *count, int *pos, Node* history);
void handle_commands(char **token, int no_commands, const char *ORIGINAL_PATH, int *count, int* pos,  Node* history);