#include <stdio.h>
#include "auxiliares.h"

int main(void) 
{
  int tam = 0;
  int i = 0;
  Timer comeco, fim;
  int status;
  int *vetA, *vetB, *vetC;

  printf("Tamanho dos vetores: \n");
  scanf("%d", &tam);
  
  vetA = (int *) malloc(sizeof(int) * tam);
  vetB = (int *) malloc(sizeof(int) * tam);
  vetC =  (int *) malloc(sizeof(int) * tam);
  
  preenche_array(1, tam, vetA);
  preenche_array(2, tam, vetB);

  gettimeofday(&comeco, NULL); // incio

  for (i = 0; i < tam; i++)
  {
    vetC[i] = vetA[i] + vetB[i];
  }

  gettimeofday(&fim, NULL);
  
  printf("\nTempo : %f ms\n", timediff(comeco, fim)); // fim

  free(vetA);
  free(vetB);
  free(vetC);
  
  return 0;
}