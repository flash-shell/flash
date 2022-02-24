#include "history.h"
#include <stdlib.h>
#include <stdio.h>

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
    Node next = {id, command};
    arr[pos] = next;

    printf("%d, %d\n", id, pos);
}

void printNodes(Node* arr){
    int i = 0;

    printf("History: \n\n");
    
    while(arr[i].id != NULL){
        printf("%d %s\n",arr[i].id, arr[i].command);
        i++;
    }
}