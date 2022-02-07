void display_prompt();
void break_to_command(char **token, int *tokenCount, const char *ORIGINAL_PATH);
void break_to_arg(char **arg, int *argCount, char *input);
int first_quote_occurence(char **args, int *argCount, int quote);
void handle_commands(char **token, int no_commands, const char *ORIGINAL_PATH);
