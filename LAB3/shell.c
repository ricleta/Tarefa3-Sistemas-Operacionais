#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define SIZE_PRIOR 7

void shell(FILE *entrada, FILE * saida);
int interpreta_cmd(char *buffer);

int main(void)
{
    FILE * entrada = fopen("entrada.txt", "w");
    FILE * saida = fopen("saida.txt", "w");
    
    shell(entrada, saida);

    return 0;
}

int interpreta_cmd(char *buffer)
{
    char tmp[100];
    int i = 0;

    while(buffer[i] != ' ' && buffer[i] != '=')
    {
        tmp[i] = buffer[i];
        i++;
        tmp[i] = '\0';
    }

    if (strcmp(tmp, "exit") == 0)
        return 0;

    if (strcmp(tmp, "help") == 0)
        return 1;

    if (strcmp(tmp, "exec") == 0)
        return 2;
    
    if (strcmp(tmp, "inicio_tempo_execucao") == 0)
        return 3;

    return -1;
}

void shell(FILE *entrada, FILE * saida)
{
    char nome_prog[100];
    char buffer[1000];
    int prioridade = 0;
    int ini_exec = 0;
    int total_exec = 0;

    while (1)
    {
        printf("\n######\nDigite 'help' para ver comandos disponiveis.\n######\n");
        scanf(" %[^\n]", buffer);

        int cmd = interpreta_cmd(buffer);

        if(cmd == 0)
        {
            break;
        }
        if(cmd== 1)
        {
            printf("\n--Para executar um programa digite: exec <nome_programa>, prioridade=<numero inteiro, de 1 a 7>\n");
            printf("\n-- Para fechar a shell digite: 'exit'\n");
        }
        if(cmd== 2)
        {
            printf("Executando\n");
        }
        if(cmd== 3)
        {
            printf("Tempo de exec=\n");
        }
    }
}