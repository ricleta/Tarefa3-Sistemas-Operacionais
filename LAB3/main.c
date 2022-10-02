#include <stdlib.h>
#include <stdio.h>
#define SIZE_PRIOR 7

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
    if(cl->current==NULL){
        cl->current=cl->head;
        return getNodeID(cl->current);
    }
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

//TODO export
int isCircularEmpty(CircularList * cl){
    return cl->head!=NULL;
}
//TODO export
void printCircular(CircularList * cl){
    if(cl->head==NULL){
        printf("\n");
        return;
    }
    for(Node * n = cl->head ; n!=NULL ;n = n->next)
        printf("%d ",n->pid);
    printf("\n");
}

//TODO export
typedef struct __PriorList{
    CircularList * priorList[SIZE_PRIOR]; 
}PriorList;

//TODO export
PriorList * newPriorList(){
    PriorList * list = malloc(sizeof(PriorList));
    for(int i = 0;i<SIZE_PRIOR;i++){
        list->priorList[i] = newCircularList();
    }
    return list;
}
//TODO export
int isPriorEmpty(PriorList * l,int priority){
    return isCircularEmpty(l->priorList[priority]);
}
//TODO export
void freePrior(PriorList * l){
    for(int i = 0; i < SIZE_PRIOR ; i++ ){
        if(!isPriorEmpty(l,i)){
            freeCircular(l->priorList[i]);
        }else{
            free(l->priorList[i]);
        }
    }
}
//TODO export
int getNext(PriorList * l,int priority){
    return nextNode(l->priorList[priority]);
}
//TODO export
PriorList * addId(PriorList * l, int priority, int id){
    addNode(l->priorList[priority],id);
    return l;
}
//TODO export
void printPrior(PriorList * pl){
    for(int i = 0; i<SIZE_PRIOR ;i++){
        printf("%d- ",i);
        printCircular(pl->priorList[i]);
    }
}

int main(void){
    PriorList * pl = newPriorList();
    
    addId(pl,1,10);
    addId(pl,1,11);
    addId(pl,1,12);
    addId(pl,2,13);
    addId(pl,2,14);
    addId(pl,2,15);
    addId(pl,4,16);
    addId(pl,4,17);
    addId(pl,6,18);
    addId(pl,6,19);
    addId(pl,6,20);
    
    printPrior(pl);

    for(int i = 0 ;i<10;i++){
        printf("%d\n",getNext(pl,1));
    }
    printf("\n");
    
    freePrior(pl);
    return 0;
}