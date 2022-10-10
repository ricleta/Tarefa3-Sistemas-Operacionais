#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define SIZE 100 
#define UT 1

typedef struct _node
{
	char nome[SIZE];	
	int prioridade;		
	int t_ini;		
	int t_total;		
	int pid;		
	int t_executado;		
	int t_round_robin;		
	struct _node *prox;	
} Node;

Node *lista_proc = NULL;	
Node *lista_exec = NULL;
Node *lista_pronto = NULL;	

int contUT = 0;	

void print_prontos(FILE * f,Node * l)
{
  Node * lista = l;
    
	if(!lista)
	{
    fprintf(f, "fila de prontos vazia\n");
    return;
  }
    
	fprintf(f,"fila de prontos: ");
    
	while(lista)
	{
    fprintf(f,"%s %c",lista->nome, lista->prox == NULL ? ' ': ',');
    lista = lista->prox;  
  }
	
	fprintf(f, "\n");
    
	return;
}

int cria_proc()
{
	int pid;

	if((pid = fork()) < 0) 
	{
		puts("Erro ao criar filho\n");
		exit(1);
	}
	
  if (pid == 0)
	{
		pid = getppid();	
		while(1);
	}

	return pid;	
}

Node* insere_proc(char *nome, int prioridade, int t_ini, int t_total)
{
	Node* novo = (Node*) malloc(sizeof(Node)); 
	Node* atual = lista_proc;	
	Node* anterior = NULL;
	
  if(novo == NULL)	
	{
		puts("Erro ao criar novo Processo\n");
		exit(1);
	}
	
  strcpy(novo->nome, nome);	
	novo->prioridade = prioridade;	
	novo->t_ini = t_ini;		
	novo->t_total = t_total;		
	novo->t_executado = 0;		
	novo->t_round_robin = 0;		
	novo->prox = NULL;		
	
	if(lista_proc == NULL)	
	{
		return novo;	
	}
	
  while(atual != NULL && novo->t_ini > atual->t_ini)	
	{
		anterior = atual;	
		atual = atual->prox;
	}	
	
  novo->prox = atual; 
	
  if(anterior == NULL)	
	{
		return novo;	
	}
	
  anterior->prox = novo;	
	return lista_proc;	
}

Node* insere_pronto(Node *processo) 
{
	Node* novo = (Node*) malloc(sizeof(Node));	
	Node* atual = lista_pronto;	
	Node* anterior = NULL;	
	
	FILE *arq_saida = fopen("saida.txt", "a");	

	if(novo == NULL)	
	{
		puts("Erro ao criar novo processo\n");
		exit(1);
	}
	
  strcpy(novo->nome, processo->nome);	
	novo->prioridade = processo->prioridade;	
	novo->t_ini = processo->t_ini;		
	novo->t_total = processo->t_total;		

	if(processo->pid == 0)	
	{
		novo->pid = cria_proc();		
		kill(novo->pid, SIGSTOP);	
	}
	else			
	{
		novo->pid = processo->pid;		
		kill(novo->pid, SIGSTOP);	
	}
	
	novo->t_executado = processo->t_executado;	
	novo->t_round_robin = 0;			
	novo->prox = NULL;			
	
	if(lista_pronto == NULL)	
	{
		fprintf(arq_saida, "Tempo %d:\t %s está pronto\n", contUT, novo->nome);
		fclose(arq_saida);
		return novo;
	}
	
  while(atual != NULL && novo->prioridade > atual->prioridade)	
	{
		anterior = atual;	
		atual = atual->prox;
	}
	
  novo->prox = atual;	
	
  if(anterior == NULL)	
	{
		fprintf(arq_saida, "Tempo %d:\t %s está pronto\n", contUT, novo->nome);
		fclose(arq_saida);
		return novo;	
	}
	
  anterior->prox = novo;	
  
  fprintf(arq_saida, "Tempo %d:\t %s está pronto\n", contUT, novo->nome);
	print_prontos(arq_saida,lista_pronto);
  
    fclose(arq_saida);
	return lista_pronto;	
}

Node* insere_exec()
{
	Node* novo = (Node*) malloc(sizeof(Node));	
	Node* atual = lista_exec;	
	Node* anterior = NULL;	
	Node* aux;

	if(novo == NULL)	
	{
		puts("Erro ao criar novo processo\n");
		exit(1);
	}
	
  strcpy(novo->nome, lista_pronto->nome);		
	novo->prioridade = lista_pronto->prioridade;	
	novo->t_ini = lista_pronto->t_ini;		
	novo->t_total = lista_pronto->t_total;		
	novo->pid = lista_pronto->pid;			
	novo->t_executado = lista_pronto->t_executado;	
	novo->t_round_robin = lista_pronto->t_round_robin;	
	novo->prox = NULL;				
	
  if(lista_exec == NULL)	
	{
		aux = lista_pronto;			
		lista_pronto = lista_pronto->prox;	
        // free(aux);
        return novo;				
	}
	
  if(atual != NULL && novo->prioridade == atual->prioridade)	
	{
		while(atual != NULL && (novo->t_ini < atual->t_ini))	
		{
			anterior = atual;	
			atual = atual->prox;
		}
		
    novo->prox = atual;	
		
        if(anterior == NULL)	
		{
			lista_exec = novo;	
		}
		else
		{
			anterior->prox = novo;	
		}
		
		aux = lista_pronto;			
		lista_pronto = lista_pronto->prox;	
		
        // free(aux);				
	}
	
  if(atual != NULL && novo->prioridade < atual->prioridade)	
	{
		aux = lista_pronto;			
		lista_pronto = lista_pronto->prox;	
		
    // free(aux);				
    while(atual != NULL)	
		{
			lista_pronto = insere_pronto(atual);	
			aux = atual;		
			atual = atual->prox;	
			// free(aux);		
		}
		
    lista_exec = novo;	
	}

	return lista_exec;	
}

void trataUT(int sinal)
{
	Node *aux;
	contUT += UT;	

	FILE *arq_saida = fopen("saida.txt", "a"); 
	
	if(lista_exec != NULL)	
	{
		if(lista_exec->prox == NULL)	
		{
			fprintf(arq_saida, "Tempo %d: \texecutando %s -- ", contUT, lista_exec->nome);
			print_prontos(arq_saida,lista_pronto);
			lista_exec->t_executado++;	
			
      if(lista_exec->t_executado == lista_exec->t_total)	
			{
				fprintf(arq_saida, "Tempo %d:\t%s acabou -- ", contUT, lista_exec->nome);
				print_prontos(arq_saida,lista_pronto);
				kill(lista_exec->pid, SIGKILL);	
				aux = lista_exec;	
				lista_exec = NULL;	
				free(aux);		
			}
		}
		else	
		{				
			Node *atual;			
			atual = lista_exec;	
			aux = atual;		
		
			fprintf(arq_saida, "Tempo %d: \texecutando %s -- ", contUT, atual->nome);
			print_prontos(arq_saida,lista_pronto);
			
            atual->t_executado++;	
			atual->t_round_robin++;	
      
            if((atual->t_round_robin)% 3 == 0 && atual->t_round_robin > 0)  
            {
                kill(atual->pid, SIGSTOP);	
                lista_exec = lista_exec->prox;  	
                    
                while(aux->prox != NULL)	
                {
                    aux = aux->prox;
                }
                        
                aux->prox = atual;		
                atual->prox = NULL;		
			}
			
      if(atual->t_executado == atual->t_total)	
			{
				fprintf(arq_saida, "Tempo %d:\t%s acabou -- ", contUT, atual->nome);
				print_prontos(arq_saida,lista_pronto);
				kill(atual->pid, SIGKILL);	
				
        if(atual->prox == NULL) 	
				{
					aux->prox = NULL;	
					free(atual);		
				}
				else	
				{
					lista_exec = lista_exec->prox;	
					free(atual);			
				}
			}	
		}
	}
	
	if(lista_proc)	
	{
		while(lista_proc->t_ini == contUT)	
		{
			Node *aux;
			lista_pronto = insere_pronto(lista_proc);	
			aux = lista_proc;		
			lista_proc = lista_proc->prox;	
			
      free(aux);			
			
      if(lista_proc == NULL)
      {
        break;
      }
		}
	}
	
  if(lista_pronto != NULL)	
	{
		lista_exec = insere_exec();	
		
    while(lista_pronto != NULL && (lista_exec->prioridade == lista_pronto->prioridade))	
		{
			lista_exec = insere_exec();	
		}
		
    kill(lista_exec->pid, SIGCONT);		
	}
	
	fclose(arq_saida);
	alarm(UT);
}


void exec_processos()
{	
	
	while(lista_proc->t_ini == contUT)	
	{
		Node *aux;
		lista_pronto = insere_pronto(lista_proc);	
		aux = lista_proc;			
		lista_proc = lista_proc->prox;		
		free(aux);				
		
		if(lista_proc == NULL) 
		{
			break;
		}		
	}
	
  if(lista_pronto != NULL)	
	{
		lista_exec = insere_exec();	
			
		while(lista_pronto != NULL && (lista_exec->prioridade == lista_pronto->prioridade))	
		{	
			lista_exec = insere_exec();	
		}
			
		kill(lista_exec->pid, SIGCONT);		
	}
	
  alarm(UT);	
	
    while(1)
    {
	    if(lista_proc == NULL && lista_pronto == NULL && lista_exec == NULL)	
	    {
			break;	
	    }
	}
}

int main(void)
{
    FILE *arq_entrada = fopen("entrada.txt", "r"); 
    char nome_proc[SIZE];
    int prioridade = 0; 
    int ini_exec = 0;
    int total_exec = 0;

    FILE *arq_saida = fopen("saida.txt", "w");	
	fclose(arq_saida);
    
    signal(SIGALRM, trataUT);	

    while(fscanf(arq_entrada, "exec %[^,], prioridade=%d, inicio_tempo_execucao=%d, tempo_total_execução=%d\n", nome_proc, &prioridade, &ini_exec, &total_exec) == 4)
    {       
        lista_proc = insere_proc(nome_proc, prioridade, ini_exec, total_exec);	
    }
    
    exec_processos();

    fclose(arq_entrada);
    
    return 0;
}