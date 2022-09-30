#include <stdlib.h>
#include "CirularList.h"
#define SIZE_PRIOR 7

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