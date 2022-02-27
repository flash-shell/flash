#include "history.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* get(Node* arr, int id){
    int i = 0;
    
    while(arr[i].id != NULL){
        if(arr[i].id == id)
            return arr[i].command;
        i++;
    }
    return "";
}
void addNode(Node* arr, int id, char* command,int pos){
    char *_command = malloc(sizeof(char) * strlen(command));
    strcpy(_command, command);
    Node next = {id, _command};
    arr[pos] = next;
}

void printNodes(Node* arr){
    int i = 0;

    printf("History: \n\n");
    
    while(arr[i].id != NULL){
        printf("%d %s\n",arr[i].id, arr[i].command);
        i++;
    }
}