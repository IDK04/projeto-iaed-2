/*
 * Ficheiro:  helpers.c
 * Autor:  Duarte Laia
 * Descrição: Funções auxiliares ao projeto.
*/
#include "proj2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Verifica se uma string está vazia */
int strVazia(char str[]) { return strlen(str)==0; }

/* Input */
/* Limpa espaços do stdin*/
char limparComando()
{
    char c;
    while ((c = getchar()) == ' ');
    return c;
}

/* Lê uma sequencia de caracteres completa do stdin */
int lerArgumento(char buffer[])
{
    char c;
    int i = 0;

    c = limparComando();

    if (c == '\"')
    {
        while ((c = getchar()) != '\"')
        {
            buffer[i++] = c;
        }
        buffer[i] = '\0';
        return (c = getchar()) != '\n';
    }
    else
    {
        /* funcionamento diferente do anterior para ler o caracter apos os espacos*/
        while ((c != ' ') && (c != '\n'))
        {
            buffer[i++] = c;
            c = getchar();
        }
        buffer[i] = '\0';
        return c != '\n';
    }
}

/* Carreiras */
void leArgumentoCarreira(int* numArgumentos, char* nomeCarreira,
                         char* argumentoOpcional)
{
    int lerArg = VERDADE;

    lerArg = lerArgumento(nomeCarreira);
    *numArgumentos += !strVazia(nomeCarreira);

    if (lerArg)
    {
        lerArg = lerArgumento(argumentoOpcional);
        *numArgumentos += !strVazia(argumentoOpcional);
    }

    if (lerArg)
        limparComando();

}

int verificaArgumentosCarreira(char *argumentoOpcional){
    char inverterComp[] = "inverso";

    if (!((strlen(argumentoOpcional) >= 3) &&
                !(strncmp(argumentoOpcional, inverterComp, strlen(argumentoOpcional)))))
    {
        printf("incorrect sort option.\n");
        return FALSO;
    }
    return VERDADE;
}

/* Paragens */
void leArgumentosParagem(int *numArgumentos, char *nomeParagem,
                         double *latitude, double *longitude){
    int lerArg = VERDADE;

    lerArg = lerArgumento(nomeParagem);
    *numArgumentos += !strVazia(nomeParagem);

    if (lerArg)
    {
        *numArgumentos+=2;
        if (scanf("%lf%lf", latitude, longitude)){;} /* evitar warning */
    }
    if (lerArg)
        limparComando();
}

int verificaArgumentosParagem(int numArgumentos, Paragem *paragem,
                              char *nomeParagem)
{

    if ((paragem == NULL) && (numArgumentos==1))
    {
        printf("%s: no such stop.\n", nomeParagem);
        return FALSO;
    }
    if ((paragem != NULL) && (numArgumentos==3))
    {
        printf("%s: stop already exists.\n", nomeParagem);
        return FALSO;
    }

    return VERDADE;
}

/* Ligações */
void leArgumentoLigacoes(char *nomeCarreira, char *nomeParagemOrigem, 
                         char *nomeParagemDestino, double *custo, 
                         double *duracao){
    lerArgumento(nomeCarreira);
    lerArgumento(nomeParagemOrigem);
    lerArgumento(nomeParagemDestino);
    if (scanf("%lf%lf", custo, duracao)){;} /* evitar warning */

}

int verificaArgumentosLigacoes1(Carreira *carreira, Paragem *origem,
                               Paragem *destino, char *nomeCarreira,
                               char *nomeParagemDestino, char *nomeParagemOrigem,
                               double custo, double duracao)
{
    if (carreira == NULL)
    {
        printf("%s: no such line.\n", nomeCarreira);
    }
    else if (destino == NULL)
    {
        printf("%s: no such stop.\n", nomeParagemDestino);
    }
    else if (origem == NULL)
    {
        printf("%s: no such stop.\n", nomeParagemOrigem);
    }
    else if ((custo < 0) || (duracao < 0))
    {
        printf("negative cost or duration.\n");
    }
    return (carreira!=NULL) && (destino!=NULL) &&
            (origem!=NULL) && (custo >= 0) && 
            (duracao >= 0);
}

int verificaArgumentosLigacoes2(int alteraDestino, int alteraOrigem,
                                int primeiraLigacao){
    if (!primeiraLigacao && !alteraDestino && !alteraOrigem)
    {
        printf("link cannot be associated with bus line.\n");
        return FALSO;
    }
    return VERDADE;
}
/* ----- */

/* BubbleSort */
void troca(int* n1, int* n2){
    int temp;
    temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

void bubbleSort(int vetor[], int tamanho, int (*comparacao)(int,int,nodeCarreira),
                nodeCarreira head)
{
    int i, j, ordenado = VERDADE;

    for (i = 0; i < tamanho-1; i++)
    {
        ordenado = VERDADE;
        for (j = tamanho-1; j > i; j--)
            if ((*comparacao)(vetor[j-1],vetor[j],head))
            {
                troca(&vetor[j], &vetor[j-1]);
                ordenado = FALSO;
            }
        if (ordenado)
            break;
    }
}
/* -------- */

/* Handle do erro de memória */
void* tenta_malloc(BaseDados *bd, size_t tamanho){
    void* ponteiro = malloc(tamanho);

    if (ponteiro != NULL)
        return ponteiro;
    
    printf("No memory.\n");
    chamarRemoverTudo(bd);
    exit(0);
    return NULL;
}
/* ------------------------- */