#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int get(Node* arr, int id, char** token) {
    int i = 0;

    while (arr[i].command != NULL) {
        if (arr[i].id == id) {
            for (int j = 0; j < arr[i].no_token; j++) {
                if (token[j] != NULL) {
                    strcpy(token[j], arr[i].command[j]);
                } else {
                    token[j] = arr[i].command[j];
                }
            }

            int tokenCount = arr[i].no_token;
            char *tempNewToken[512];
            recheck_aliases(token, tempNewToken, tokenCount);
            return 1;
        }
        i++;
    }

    printf("Error. ID %d is not found in history\n", id);
    return 0;
}

void addNode(Node* arr, int id, int pos, char **token, int no_token) {
    char **command = malloc(sizeof(char*) * no_token);
    for (int i = 0; i < no_token; i++) {
        command[i] = malloc(sizeof(char) * strlen(token[i]));
        strcpy(command[i], token[i]);
    }
    
    Node next = {id, command, no_token};
    arr[pos] = next;
}

void printNodes(Node* arr) {
    int count = 0;
    int index = getEarliest(arr);
    printf("History:\n\n");
    
    while (arr[index].command != NULL && count < 20) {
        printf("%d\t", arr[index].id);
        for (int j = 0; j < arr[index].no_token; j++) {
            printf("%s ", arr[index].command[j]);
        }
        printf("\n");
        count++;
        index = (index + 1) % 20;
    }
}

int getEarliest(Node* arr) {
    int i = 1;
    int earliest = arr[0].id;
    int index = 0;

    while (arr[i].command != NULL && i < 20) {
        if (arr[i].id < earliest) {
            earliest = arr[i].id;
            index = i;
        }
        i++;
    }
    
    return index;
}

void saveHistory(Node* arr) {
    FILE *historyFile;
    char *historyFilePath = getenv("HOME");
    char historyFilePathCopy[256];
    strcpy(historyFilePathCopy, historyFilePath);
    strcat(historyFilePathCopy, "/.hist_list");
    historyFile = fopen(historyFilePathCopy, "w");
    if (historyFile == NULL) {
        return;
    }
    
    int count = 0;
    int index = getEarliest(arr);

    while (arr[index].command != NULL && count < 20) {
        fprintf(historyFile, "%d ", arr[index].id);
        for (int j = 0; j < arr[index].no_token; j++) {
            fprintf(historyFile, "%s ", arr[index].command[j]);
        }
        fprintf(historyFile, "\n");
        count++;
        index = (index + 1) % 20;
    }

    fclose(historyFile);
}

void loadHistory(Node* arr, int *count, int *pos) {
    char input[256];
    int id = 0;
    char command[256];
    FILE *historyFile;
    bool check = true;
    
    historyFile = fopen(".hist_list", "r");
    
    if (historyFile == NULL) {
        return;
    }
    
    const char delims[] = "\t|><&; ";
    while (fgets(input, sizeof input, historyFile)) {
        sscanf(input, "%d %[^\n]", &id, command);
        int tokenCount = 0;
        char *token[512];
        token[tokenCount] = strtok(command, delims);

        while (token[tokenCount] != NULL) {
            tokenCount++;
            token[tokenCount] = strtok(NULL, delims);
        }

        if (check) {
            *count = id;
            check = false;
        }

        addNode(arr, *count, *pos, token, tokenCount);
        *count = *count + 1;
        *pos = (*pos + 1) % 20;
    }

    fclose(historyFile);
}