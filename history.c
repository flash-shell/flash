#include "history.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void get(Node* arr, int id, char** token){
    int i = 0;

    while(arr[i].id != NULL){
        printf("%d\n", arr[i].id);
        if(arr[i].id == id){ 
            printf("%d\t", arr[i].id);
            for(int j = 0; j < arr[i].no_token; j++){
                strcpy(token[j], arr[i].command[j]);
                printf("%s ", arr[i].command[j]);
            }
            printf("\n");
            return;
        }
        i++;
    }
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
    
    while(arr[i].id != NULL){
        printf("%d\t", arr[i].id);
        for(int j = 0; j < arr[i].no_token; j++){
            printf("%s ", arr[i].command[j]);
        }
        printf("\n");
        i++;
    }
}