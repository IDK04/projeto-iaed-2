/*
 * Ficheiro:  carreira.c
 * Autor:  Duarte Laia
 * Descrição: Funcionalidades dos comandos relativos a carreiras ('c', 'r' e 'a').
*/

#include "proj2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Funcionalidades do comando 'c' */
/* Cria uma nova carreira */
Carreira criarCarreira(char *buffer, BaseDados *bd)
{
    Carreira novaCarreira;
    novaCarreira.nome = (char *)tenta_malloc(bd, sizeof(char) * (strlen(buffer) + 1));
    strcpy(novaCarreira.nome, buffer);
    novaCarreira.custoTotal = 0.0;
    novaCarreira.id = bd->idCarreira;
    novaCarreira.duracaoTotal = 0.0;
    novaCarreira.numParagens = 0;
    novaCarreira.nodeOrigem = NULL;
    novaCarreira.nodeDestino = NULL;
    return novaCarreira;
}

/* Cria um node de uma linked list de carreiras */
nodeCarreira criarNodeCarreira(char *nomeCarreira, BaseDados *bd)
{
    nodeCarreira novoNode;
    novoNode = (nodeCarreira)tenta_malloc(bd, sizeof(struct nC));
    novoNode->carreira = criarCarreira(nomeCarreira, bd);
    novoNode->proxima = NULL;
    return novoNode;
}

/* Adiciona uma carreira no fim da linked list de carreiras */
nodeCarreira adicionarCarreira(char *nomeCarreira, BaseDados *bd)
{
    nodeCarreira nodeAtual;

    if (bd->carreiras == NULL)
    {
        return criarNodeCarreira(nomeCarreira, bd);
    }

    for (nodeAtual = bd->carreiras; nodeAtual->proxima != NULL;
         nodeAtual = nodeAtual->proxima)
        ;

    nodeAtual->proxima = criarNodeCarreira(nomeCarreira, bd);

    return bd->carreiras;
}

/* 
 * Encontra uma carreira na linked list de carreiras 
 * Um dos parametros id ou nome deve ser omitido (com NAO_EXISTE ou NULL),
 * respetivamente.
*/
Carreira *encontraCarreira(int id, char *nome, nodeCarreira head)
{
    nodeCarreira atual;

    if (id != NAO_EXISTE)
    {
        for (atual = head; atual != NULL; atual = atual->proxima)
        {
            if (atual->carreira.id == id)
                return &(atual->carreira);
        }
    }
    else
    {
        for (atual = head; atual != NULL; atual = atual->proxima)
        {
            if (strcmp(nome, atual->carreira.nome) == 0)
            {
                return &(atual->carreira);
            }
        }
    }

    return NULL;
}

/* Lista as carreiras por ordem de criação */
void listarCarreiras(nodeCarreira head)
{
    nodeCarreira atual;

    for (atual = head; atual != NULL; atual = atual->proxima)
    {
        printf("%s ", atual->carreira.nome);
        if (atual->carreira.nodeOrigem != NULL)
        {
            printf("%s %s ", atual->carreira.nodeOrigem->ligacao.origem->nome,
                   atual->carreira.nodeDestino->ligacao.destino->nome);
        }
        printf("%d %.2f %.2f\n", atual->carreira.numParagens,
               atual->carreira.custoTotal, atual->carreira.duracaoTotal);
    }
}

/* Lista as paragens associadas a uma carreira */
void listarParagensCarreira(Carreira *carreira, int inverso)
{
    nodeLigacao atual;

    if (!inverso)
    {
        printf("%s,", carreira->nodeOrigem->ligacao.origem->nome);
        for (atual = carreira->nodeOrigem->proxima; atual != NULL; atual = atual->proxima)
        {
            printf(" %s,", atual->ligacao.origem->nome);
        }
        printf(" %s\n", carreira->nodeDestino->ligacao.destino->nome);
    }
    else
    {
        printf("%s,", carreira->nodeDestino->ligacao.destino->nome);
        for (atual = carreira->nodeDestino->anterior; atual != NULL; atual = atual->anterior)
        {
            printf(" %s,", atual->ligacao.destino->nome);
        }
        printf(" %s\n", carreira->nodeOrigem->ligacao.origem->nome);
    }
}

/* Funcionalidades do comando 'r' - (Com interação entre carreiras e paragens) */
/* Remove a instância de uma carreira numa paragem */
void removeCarreiraDeParagem(Paragem *paragem, int id)
{
    nodeIDCarreira nodeRemover, nodeAnterior;
    if (paragem->idCarreiras == NULL)
        return;

    for (nodeAnterior = NULL, nodeRemover = paragem->idCarreiras; nodeRemover != NULL;
         nodeAnterior = nodeRemover, nodeRemover = nodeRemover->proxima)
    {
        if (nodeRemover->id == id)
            break;
    }

    if (nodeRemover == NULL)
        return;

    if (nodeAnterior == NULL)
        paragem->idCarreiras = nodeRemover->proxima;
    else
        nodeAnterior->proxima = nodeRemover->proxima;

    paragem->numCarreiras--;

    free(nodeRemover);
}

/* Remove um node de uma ligação*/
nodeLigacao popNodeLigacao(nodeLigacao head)
{
    nodeLigacao proxima = head->proxima;
    free(head);
    return proxima;
}

/* Remove as ligações de uma carreira */
void removeLigacoesCarreira(Carreira *carreira)
{
    nodeLigacao head = carreira->nodeOrigem;
    while (head)
    {
        removeCarreiraDeParagem(head->ligacao.origem, carreira->id);
        removeCarreiraDeParagem(head->ligacao.destino, carreira->id);
        head = popNodeLigacao(head);
    }
}
/* Remove uma carreira */
void removeCarreira(BaseDados **bd, char *nomeCarreira)
{
    nodeCarreira nodeRemover, nodeAnterior;
    if ((**bd).carreiras == NULL)
    {
        printf("%s: no such line.\n", nomeCarreira);
        return;
    }
    /* encontrar o node a remover */
    for (nodeAnterior = NULL, nodeRemover = (**bd).carreiras; nodeRemover != NULL;
         nodeAnterior = nodeRemover, nodeRemover = nodeRemover->proxima)
    {
        if (strcmp(nomeCarreira, nodeRemover->carreira.nome) == 0)
            break;
    }
    if (nodeRemover == NULL)
    {
        printf("%s: no such line.\n", nomeCarreira);
        return;
    }

    if (nodeAnterior == NULL)
        (**bd).carreiras = nodeRemover->proxima;
    else
        nodeAnterior->proxima = nodeRemover->proxima;

    /* free das ligacoes e do indice das paragens */
    removeLigacoesCarreira(&nodeRemover->carreira);
    free(nodeRemover->carreira.nome);
    free(nodeRemover);
}

/* Funcionalidades do comando 'a' */
/* Remove o node de uma carreira */
nodeCarreira popNodeCarreira(nodeCarreira head)
{
    nodeCarreira aux = head->proxima;
    free(head);
    return aux;
}

/* Remove todas as carreiras */
void removeTodasCarreiras(BaseDados **bd)
{
    nodeCarreira head = (**bd).carreiras;

    while (head)
    {
        removeLigacoesCarreira(&head->carreira);
        free(head->carreira.nome);
        head = popNodeCarreira(head);
    }

    (**bd).carreiras = NULL;
}