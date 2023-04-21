/*
 * Ficheiro:  ligacao.c
 * Autor:  Duarte Laia
 * Descrição: Funcionalidades do comando relativo a ligaões ('l').
*/
#include "proj2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Funcionalidades do comando l*/
/* Verifica se uma paragem se encontra na origem de uma carreira */
int comparaOrigemCarreira(Carreira *carreira, Paragem *paragem){
    return (carreira->nodeOrigem != NULL) &&
   (carreira->nodeOrigem->ligacao.origem == paragem);
}

/* Verifica se uma paragem se encontra no destino de uma carreira */
int comparaDestinoCarreira(Carreira *carreira, Paragem *paragem){
    return (carreira->nodeDestino != NULL) && 
    (carreira->nodeDestino->ligacao.destino == paragem);
}

/* Cria uma nova ligação */
Ligacao criaLigacao(Carreira *carreira, Paragem *origem, Paragem *destino,
                    double custo, double duracao)
{
    Ligacao novaLigacao;
    novaLigacao.custo = custo;
    novaLigacao.duracao = duracao;
    novaLigacao.idCarreira = carreira->id;
    novaLigacao.origem = origem;
    novaLigacao.destino = destino;
    return novaLigacao;
}

/* Cria um node de uma ligação */
nodeLigacao criaNodeLigacao(Ligacao ligacao, BaseDados *bd)
{
    nodeLigacao novaNode = (nodeLigacao) tenta_malloc(bd, sizeof(struct nL));
    novaNode->ligacao = ligacao;
    novaNode->anterior = NULL;
    novaNode->proxima = NULL;
    return novaNode;
}

/* Interação com carreiras */
/* Altera o custo, duração e numero de paragens de uma carreira */
void alteraInformacaoCarreira(Carreira **carreira, double custo, double duracao,
                              int numParagens){
    (**carreira).custoTotal += custo;
    (**carreira).duracaoTotal += duracao;
    (**carreira).numParagens += numParagens;
}

/* Interacao com paragens */
/* Cria um novo node de ids de carreira (associados a uma paragem) */
nodeIDCarreira criaNodeIDCarreira(int id, BaseDados *bd){
    nodeIDCarreira novoNode = (nodeIDCarreira) tenta_malloc(bd, sizeof(struct nID));
    novoNode->id = id;
    novoNode->proxima = NULL;
    return novoNode;
}

/* Adiciona um novo node de id de carreira à linked list de ids de carreiras 
   associados a uma paragem */
void alteraInformacaoParagem(Paragem **paragem, Carreira *carreira, BaseDados *bd){
    nodeIDCarreira atual;

    if ((**paragem).idCarreiras == NULL)
    {
        (**paragem).idCarreiras = criaNodeIDCarreira(carreira->id, bd);
        (**paragem).numCarreiras += 1;
        return;
    }

    for(atual = (**paragem).idCarreiras; atual->proxima != NULL; atual = atual->proxima)
    {
        if (atual->id == carreira->id)
            return;
    }

    if (atual->id == carreira->id)
            return;

    atual->proxima = criaNodeIDCarreira(carreira->id, bd);
    (**paragem).numCarreiras += 1;
}

/* Adiciona uma nova ligação */
void adicionarLigacao(Carreira **carreira, Paragem **origem, Paragem **destino,
                              double custo, double duracao, int alteraOrigem,
                              int alteraDestino, int primeiraLigacao,
                              BaseDados *bd)
{
    nodeLigacao novaNode;
    int numParagens=1;
    Ligacao novaLigacao = criaLigacao(*carreira, *origem, *destino, custo, duracao);
    if (primeiraLigacao)
    {
        novaNode = criaNodeLigacao(novaLigacao, bd);
        (**carreira).nodeOrigem = novaNode;
        (**carreira).nodeDestino = novaNode;

        alteraInformacaoParagem(origem, *carreira, bd);
        numParagens = 2;

        if (strcmp((**origem).nome, (**destino).nome)!=0)
            alteraInformacaoParagem(destino, *carreira, bd);
    }
    else if(alteraDestino)  /* verifica-se primeiro o destino para no caso de */
    {                       /* ser circular, alterar apenas o destino         */
        novaNode = criaNodeLigacao(novaLigacao, bd);
        novaNode->anterior = (**carreira).nodeDestino;
        (**carreira).nodeDestino->proxima = novaNode;
        (**carreira).nodeDestino = novaNode;
        if (!alteraOrigem) /* no caso de ser circular, não se adiciona carreira */
            alteraInformacaoParagem(destino, *carreira, bd);
    }
    else if(alteraOrigem)
    {
        novaNode = criaNodeLigacao(novaLigacao, bd);
        novaNode->proxima = (**carreira).nodeOrigem;
        (**carreira).nodeOrigem->anterior = novaNode;
        (**carreira).nodeOrigem = novaNode;
        alteraInformacaoParagem(origem, *carreira, bd);
    }
    
    alteraInformacaoCarreira(carreira, custo, duracao, numParagens);
}
