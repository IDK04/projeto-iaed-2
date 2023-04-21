/*
 * Ficheiro:  paragem.c
 * Autor:  Duarte Laia
 * Descrição: Funcionalidades dos comandos relativos a paragens ('p', 'e' e 'a').
*/
#include "proj2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Funcionalidades do comando 'p' */
/* Cria uma nova paragem */
Paragem criarParagem(char *buffer, double latitude, double longitude, BaseDados *bd)
{
    Paragem novaParagem;
    novaParagem.nome = (char*) tenta_malloc(bd, sizeof(char) * (strlen(buffer)+1));
    strcpy(novaParagem.nome, buffer);
    novaParagem.latitude = latitude;
    novaParagem.longitude = longitude;
    novaParagem.numCarreiras = 0;
    novaParagem.idCarreiras = NULL;
    return novaParagem;
}

/* Cria um novo node de paragem */
nodeParagem criarNodeParagem(char *nomeParagem, double latitude, double longitude,
                             BaseDados *bd)
{
    nodeParagem novoNode;
    novoNode = (nodeParagem) tenta_malloc(bd, sizeof(struct nP));
    novoNode->paragem = criarParagem(nomeParagem, latitude, longitude, bd);
    novoNode->proxima = NULL;
    return novoNode;
}

/* Adiciona uma nova paragem */
nodeParagem adicionarParagem(char *nomeParagem, double latitude,
                             double longitude, BaseDados *bd)
{
    nodeParagem nodeAtual;

    if (bd->paragens == NULL){
        return criarNodeParagem(nomeParagem, latitude, longitude, bd);
    }
    
    for (nodeAtual = bd->paragens; nodeAtual->proxima != NULL; 
         nodeAtual = nodeAtual->proxima);
    
    nodeAtual->proxima = criarNodeParagem(nomeParagem, latitude, longitude, bd);

    return bd->paragens;
}

/* Encontra uma paragem */
Paragem* encontraParagem(char *nomeParagem, nodeParagem head){
    nodeParagem atual;
    for (atual = head; atual != NULL; atual = atual->proxima)
    {
        if (strcmp(nomeParagem, atual->paragem.nome)==0)
            return &atual->paragem;
    }
    return NULL;
}

/* Lista todas as paragens */
void listarParagens(nodeParagem head){
    nodeParagem atual;

    for (atual = head; atual != NULL; atual = atual->proxima)
    {
        printf("%s: %16.12f %16.12f %d\n", atual->paragem.nome,
                atual->paragem.latitude, atual->paragem.longitude, 
                atual->paragem.numCarreiras);
    }
}

/* Detalha uma paragem */
void detalharParagem(Paragem *paragem){
    printf("%16.12f %16.12f\n", paragem->latitude, paragem->longitude);
}

/* Funcionalidades do comando 'e' - (Com interação entre carreiras e paragens) */
/* Elimina um node de id de carreira */
nodeIDCarreira popIDCarreira(nodeIDCarreira head){
    nodeIDCarreira proxima = head->proxima;
    free(head);
    return proxima;
}

/* Elimina a paragem de origem de uma ligação */
void encurtaOrigem(Carreira **carreira, nodeLigacao *ligacaoAtual, nodeLigacao *aux)
{
    (*carreira)->custoTotal -= (*ligacaoAtual)->ligacao.custo;
    (*carreira)->duracaoTotal -= (*ligacaoAtual)->ligacao.duracao;
    (*carreira)->numParagens--;
    *aux = *ligacaoAtual;
    *ligacaoAtual = (*ligacaoAtual)->proxima;
    (*carreira)->nodeOrigem = *ligacaoAtual;
    if (*ligacaoAtual != NULL)
    {
        free(*aux);
        (*ligacaoAtual)->anterior = NULL;
    }
}
/* Elimina a paragem de destino de uma ligação */
void encurtaDestino(Carreira **carreira, nodeLigacao *ligacaoAtual, nodeLigacao *aux)
{
    (*carreira)->custoTotal -= (*ligacaoAtual)->ligacao.custo;
    (*carreira)->duracaoTotal -= (*ligacaoAtual)->ligacao.duracao;
    (*carreira)->numParagens--;
    *aux = *ligacaoAtual;
    (*ligacaoAtual) = (*ligacaoAtual)->anterior;
    (*carreira)->nodeDestino = (*ligacaoAtual);
    if (*ligacaoAtual != NULL)
    {
        free(*aux);
        (*ligacaoAtual)->proxima = NULL;
    }
}
/* Colapsa duas ligações */
void colapsaLigacoes(Carreira **carreira, nodeLigacao *ligacaoAtual, nodeLigacao *aux)
{
    (*aux) = (*ligacaoAtual)->proxima;
    (*ligacaoAtual)->proxima = (*aux)->proxima;
    if ((*aux)->proxima == NULL)
        (*carreira)->nodeDestino = (*ligacaoAtual);
    else
        (*aux)->proxima->anterior = (*ligacaoAtual);
    (*ligacaoAtual)->ligacao.destino = (*aux)->ligacao.destino;
    (*ligacaoAtual)->ligacao.custo += (*aux)->ligacao.custo;
    (*ligacaoAtual)->ligacao.duracao += (*aux)->ligacao.duracao;
    (*carreira)->numParagens--;
    free(*aux);
}

/* Elimina a referência (por id) desta carreira da paragem de origem */
void reparaParagemOrigem(Carreira **carreira, nodeLigacao *aux)
{
    if ((*carreira)->nodeOrigem->ligacao.destino != (*carreira)->nodeOrigem->ligacao.origem)
        removeCarreiraDeParagem((*carreira)->nodeOrigem->ligacao.origem, (*carreira)->id);
    free(*aux);
    (*carreira)->nodeOrigem = NULL;
    (*carreira)->numParagens = 0;
}

/* Elimina a referência (por id) desta carreira da paragem de destino */
void reparaParagemDestino(Carreira **carreira, nodeLigacao *aux)
{
    if ((*carreira)->nodeDestino->ligacao.destino != (*carreira)->nodeDestino->ligacao.origem)
        removeCarreiraDeParagem((*carreira)->nodeDestino->ligacao.destino, (*carreira)->id);
    free(*aux);
    (*carreira)->nodeDestino = NULL;
    (*carreira)->numParagens = 0;
}

/* Remove uma paragem de uma carreira */
void removeParagemDeCarreira(Paragem *paragem, Carreira *carreira){
    nodeLigacao ligacaoAtual = carreira->nodeOrigem, aux=NULL;
    while (ligacaoAtual){
        if (paragem == ligacaoAtual->ligacao.origem)
        {
            /* Caso 1 - encurtar na origem*/
            encurtaOrigem(&carreira, &ligacaoAtual, &aux);
        }
        else if (paragem == ligacaoAtual->ligacao.destino)
        {   
            if (ligacaoAtual->proxima == NULL)
            {
                /* Caso 2 - encurtar no destino */
                encurtaDestino(&carreira, &ligacaoAtual, &aux);
                break;
            }
            else
            {
                /* Caso 3 - colapsar duas ligações */
                colapsaLigacoes(&carreira, &ligacaoAtual, &aux);
            }
        }
        else
            if (ligacaoAtual != NULL)
                ligacaoAtual = ligacaoAtual->proxima;
    }

    if (carreira->nodeOrigem == NULL)
        reparaParagemDestino(&carreira, &aux);
    else if (carreira->nodeDestino == NULL)
        reparaParagemOrigem(&carreira, &aux);
}

/* Remove a linked list de ids de carreiras associadas a uma paragem */
void removeListaIDsCarreira(Paragem *paragem, BaseDados **bd){
    nodeIDCarreira IDatual;
    if (paragem->idCarreiras == NULL)
        return;
    
    for (IDatual = paragem->idCarreiras; IDatual != NULL;)
    {
        removeParagemDeCarreira(paragem,
                            encontraCarreira(IDatual->id,NULL,(**bd).carreiras));
        IDatual = popIDCarreira(IDatual);
    }
    
}

/* Remove uma paragem */
void removeParagem(BaseDados **bd, char *nomeParagem){
    nodeParagem nodeRemover, nodeAnterior=NULL;

    if ((**bd).paragens == NULL)
    {
        printf("%s: no such stop.\n", nomeParagem);
        return;
    }

    for (nodeRemover = (**bd).paragens; nodeRemover != NULL;
         nodeAnterior = nodeRemover, nodeRemover = nodeRemover->proxima)
    {
        if (strcmp(nomeParagem, nodeRemover->paragem.nome)==0)
            break; 
    }

    if ((nodeRemover == NULL) || ((**bd).paragens == NULL))
    {
        printf("%s: no such stop.\n", nomeParagem);
        return;
    }
    
    if (nodeAnterior == NULL)
        (**bd).paragens = nodeRemover->proxima;
    else
        nodeAnterior->proxima = nodeRemover->proxima;
    
    removeListaIDsCarreira(&nodeRemover->paragem, bd);
    free(nodeRemover->paragem.nome);
    free(nodeRemover);
}

/* Funcionalidades do comando 'a' */

/* Como no comando 'a' são primeiramente eliminadas as carreiras e as suas
   dependências nas paragens, é seguro remover apenas os nodes das paragens
   e os nomes de cada paragem*/

/* Remove o node de uma paragem */
nodeParagem popNodeParagem(nodeParagem head){
    nodeParagem aux = head->proxima;
    free(head);
    return aux;
}

/* Remove todas as paragens */
void removeTodasParagens(BaseDados **bd){
    nodeParagem head = (**bd).paragens;

    while (head)
    {
        removeListaIDsCarreira(&head->paragem, bd);
        free(head->paragem.nome);
        head = popNodeParagem(head);
    }

    (**bd).paragens = NULL;
}