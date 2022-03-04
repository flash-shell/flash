#include "header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int get(Node* arr, int id, char** token){
    int i = 0;

    while(arr[i].command != NULL){
        if(arr[i].id == id){
            for(int j = 0; j < arr[i].no_token; j++){
                if (token[j] != NULL) {
                    strcpy(token[j], arr[i].command[j]);
                } else {
                    token[j] = arr[i].command[j];
                }
            }
            return 1;
        }
        i++;
    }
    printf("id %d not found\n", id);
    return 0;
}

void addNode(Node* arr, int id, int pos, char **token, int no_token){
    char **command = malloc(sizeof(char*) * no_token);
    for(int i = 0; i < no_token; i++){
        command[i] = malloc(sizeof(char) * strlen(token[i]));
        strcpy(command[i], token[i]);
    }
    Node next = {id, command, no_token};
    arr[pos] = next;
}

void printNodes(Node* arr){
    int i = 0;

    printf("History:\n\n");
    
    while(arr[i].command != NULL && i < 20){
        printf("%d\t", arr[i].id);
        for(int j = 0; j < arr[i].no_token; j++){
            printf("%s ", arr[i].command[j]);
        }
        printf("\n");
        i++;
    }
}