typedef struct NODE {
    int id;
    char* command;
}  Node;

char* get(Node* arr, int id);
void addNode(Node* arr,int id, char* command, int pos);
void printNodes(Node* arr);

// iohandler.h is here

void display_prompt();
void break_to_command(char **token, int *tokenCount, const char *ORIGINAL_PATH, int *count, int *pos, Node* history);
void handle_commands(char **token, int no_commands, const char *ORIGINAL_PATH, Node* history);