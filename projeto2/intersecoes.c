/*
 * Ficheiro:  intersecoes.c
 * Autor:  Duarte Laia
 * Descrição: Funcionalidades do comando relativo a interseções ('i').
*/
#include "proj2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compara o nome de duas carreiras (para ordenação)*/
int comparaCarreira(int c1, int c2, nodeCarreira head){
    return strcmp(encontraCarreira(c1,NULL,head)->nome,
                  encontraCarreira(c2,NULL,head)->nome) > 0;
}

/* Converte uma linked list de nodes de ids de carreira associados a uma paragem
   para um vetor de inteiros */
int* converteListaLigada(nodeParagem nodeParagem, BaseDados *bd){
    nodeIDCarreira atual;
    int i;
    int *idCarreiras = (int*) tenta_malloc(bd, sizeof(int)*nodeParagem->paragem.numCarreiras);
    for (i = 0,atual = nodeParagem->paragem.idCarreiras; atual != NULL; i++,
         atual = atual->proxima)
    {
        idCarreiras[i] = atual->id;
    }
    return idCarreiras;
}

/* Mostra as interseções entre carreiras nas paragens */
void mostraIntercesoes(BaseDados *bd){
    int *idCarreiras, i;
    nodeParagem atual;

    for (atual = bd->paragens; atual != NULL; atual = atual->proxima)
    {
        if (atual->paragem.numCarreiras > 1)
        {
            idCarreiras = converteListaLigada(atual, bd);
            bubbleSort(idCarreiras, atual->paragem.numCarreiras, comparaCarreira,
                       bd->carreiras);
            printf("%s %d:", atual->paragem.nome, atual->paragem.numCarreiras);
            for (i = 0; i < atual->paragem.numCarreiras; i++)
            {
                printf(" %s", encontraCarreira(idCarreiras[i],NULL,
                                               bd->carreiras)->nome);
            }
            putchar('\n');
            free(idCarreiras);
        }
    }
}