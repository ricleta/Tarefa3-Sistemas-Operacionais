#include <stdlib.h>
#include "CirularList.h"
#define SIZE_PRIOR 7


typedef struct __PriorList{
    CircularList * priorList[SIZE_PRIOR]; 
}PriorList;


PriorList * newPriorList(){
    PriorList * list = malloc(sizeof(PriorList));
    return list;
}


int isPriorEmpty(PriorList * l,int priority){
    return isCircularEmpty(l->priorList[priority]);
}

void freePrior(PriorList * l){
    for(int i = 0; i < SIZE_PRIOR ; i++ ){
        if(!isPriorEmpty(l,i)){
            freeCircular(l->priorList[i]);
        }
    }
}

int getNext(PriorList * l,int priority){
    return nextNode(l->priorList[priority]);
}

PriorList * addId(PriorList * l, int priority, int id){
    addNode(l->priorList[priority],id);
    return l;
}

