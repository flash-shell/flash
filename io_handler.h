void display_prompt();
void break_to_command(char **token, int *tokenCount);
void break_to_arg(char **arg, int *argCount, char *input);
int first_quote_occurence(char **args, int *argCount, int quote);
void handle_helper(char **token, int no_commands);
void handle_commands(char **token, int no_commands);