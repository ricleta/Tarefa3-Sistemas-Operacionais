#include <stdlib.h>

typedef struct __Node{
    int pid;
    struct __Node * next;
}Node;

//TODO export
typedef struct __CircularList{
    Node * head;
    Node * last;
    Node * current;
}CircularList;

/*
int throw(const char * str){
    printf("Error %s\n",str);
    exit(-1);
    return 0;
}
*/

Node * newNode(int pid){
    Node * n = malloc(sizeof(Node));
    n->pid = pid;
    n->next = NULL;
}


//TODO export
CircularList * newCircularList(){
    CircularList * list = malloc(sizeof(CircularList));
    list->head = NULL;
    list->last = NULL;
    list->current = NULL;
    return list;
}


//TODO export
CircularList * addNode(CircularList * cl, int pid){
    Node * node = newNode(pid);
    if(cl->head==NULL){
        cl->head = node;
        cl->last = node;
    }else{
        cl->last->next = node;
        cl->last = node;
    }
    return cl;
}

int getNodeID(Node * n){
    return n->pid;
}

//TODO export
int nextNode(CircularList * cl){
    Node * next = cl->current->next;
    if(cl->head!=NULL){
        if(next==NULL)
            cl->current = cl->head;    
        else
            cl->current = next;

        return getNodeID(cl->current);
    }
    return -1;
}

void freeNode(Node * n){
    if(n==NULL)
        return;
    freeNode(n->next);
    free(n);
}

//TODO export
void freeCircular(CircularList * cl){
    freeNode(cl->head);
    free(cl);
}

int isCircularEmpty(CircularList * cl){
    return cl->head!=NULL;
}