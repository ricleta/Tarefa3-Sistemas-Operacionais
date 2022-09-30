#include <stdlib.h>
#include "CirularList.h"
#define SIZE_PRIOR 7


typedef struct __PriorList{
    CircularList * priorList[SIZE_PRIOR]; 
}PriorList;

Node * newNode(int a,Node * prev,Node * next){
    Node * node = malloc(sizeof(Node));
    if(next==NULL){
        node->prev = node;
        node->next = node;
    }else{
        node->prev = prev;
        node->next = next;
    }
    node->id = a;
    return node;
}



PriorList * newList(){
    PriorList * list = malloc(sizeof(List));
    return list;
}
/*


List * addNode(List * l,int priority,int n){
    if(isPriorityEmpty){
        l->priorList[priority] = newNodeStart(n);
    }else{
        l->priorList[priority] = newNode(n);
    }
    return l;
}

*/

int isPriorityEmpty(List * l,int priority){
    return l->priorList[priority]!=NULL;
}


void freeNode(Node * n){

}



void freeList(List * l){
    for(int i = 0; i < SIZE_PRIOR ; i++ ){
        if(!isPriorityEmpty(l,i)){
            freeNode(l->priorList[i]);
        }
    }
}


