#include <stdio.h>
#include "auxiliares.h"

int main(void)
{
  int tam = 0;
  int num_filhos = 8;
  int tam_p = tam / num_filhos;
  int i = 1;
  int j = 0;
  int status = 0;
  int pid, segmento;
  int *vetA, *vetB, *vetC;
  Timer comeco, fim;

  printf("Tamanho dos vetores: \n");
  scanf("%d", &tam);
  
  vetA = (int *)malloc(sizeof(int) * tam);
  vetB = (int *)malloc(sizeof(int) * tam);
  
  preenche_array(1, tam, vetA);
  preenche_array(2, tam, vetB);

  segmento = shmget(IPC_PRIVATE, sizeof(int) * tam, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  vetC = (int *)shmat(segmento, 0, 0);


  gettimeofday(&comeco, NULL); // incio

  for (i = 1; i <= num_filhos; i++)
  {
    pid = fork();
    
    if(pid == 0)
    {
      for (j = tam_p * (i - 1); j < i * tam_p; j++)
      {
        vetC[j] = vetA[j] + vetB[j];
      }

      exit(0);
    }
  }

  for (int k = 0; k < num_filhos; k++)
  {
    waitpid(-1, &status, 0);
  }

  gettimeofday(&fim, NULL);

  printf("\nTempo : %f ms\n", timediff(comeco, fim)); // fim

  free(vetA);
  free(vetB);
    
  shmdt(vetC);
  // libera a memória compartilhada
  shmctl(segmento, IPC_RMID, 0);

  return 0;
}