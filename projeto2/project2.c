/* iaed-23 - ist1106876 - project2 */
/*
 * Ficheiro:  main.c
 * Autor:  Duarte Laia
 * Descrição: Funcionalidade principal do projeto 2 de IAED.
 */
#include "proj2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Função principal do comando 'c' */
void chamarCarreira(BaseDados *bd)
{
    char nomeCarreira[MAX_TAMANHO_LINHA], inverso[MAX_TAMANHO_LINHA];
    Carreira *carreira;
    int numArgumentos = 0;

    leArgumentoCarreira(&numArgumentos, nomeCarreira, inverso);

    carreira = encontraCarreira(NAO_EXISTE, nomeCarreira, bd->carreiras);

    switch (numArgumentos)
    {
    case 0:
        listarCarreiras(bd->carreiras);
        break;
    case 1:
        if (carreira == NULL)
        {
            bd->carreiras = adicionarCarreira(nomeCarreira, bd);
            bd->idCarreira++;
        }
        else if (carreira->nodeOrigem != NULL)
            listarParagensCarreira(carreira, FALSO);
        break;
    case 2:
        if (!verificaArgumentosCarreira(inverso))
            return;
        if (carreira->nodeOrigem != NULL)
            listarParagensCarreira(carreira, VERDADE);
        break;
    }
}

/* Função principal do comando 'p' */
void chamarParagem(BaseDados *bd)
{
    int numArgumentos = 0;
    char nomeParagem[MAX_TAMANHO_LINHA];
    double latitude, longitude;
    Paragem *paragem;

    leArgumentosParagem(&numArgumentos, nomeParagem, &latitude, &longitude);

    paragem = encontraParagem(nomeParagem, bd->paragens);

    if (!verificaArgumentosParagem(numArgumentos, paragem, nomeParagem))
        return;

    switch (numArgumentos)
    {
    case 0:
        listarParagens(bd->paragens);
        break;

    case 1:
        detalharParagem(paragem);
        break;

    case 3:
        bd->paragens = adicionarParagem(nomeParagem, latitude, longitude, bd);
        break;
    }
}

/* Função principal do comando 'l' */
void chamarLigacoes(BaseDados *bd)
{
    int primeiraLigacao, alteraDestino, alteraOrigem;
    Carreira *carreira;
    Paragem *origem, *destino;

    char nomeCarreira[MAX_TAMANHO_LINHA], nomeParagemOrigem[MAX_TAMANHO_LINHA],
        nomeParagemDestino[MAX_TAMANHO_LINHA];
    double custo, duracao;

    leArgumentoLigacoes(nomeCarreira, nomeParagemOrigem, nomeParagemDestino,
                        &custo, &duracao);

    carreira = encontraCarreira(NAO_EXISTE, nomeCarreira, bd->carreiras);

    origem = encontraParagem(nomeParagemOrigem, bd->paragens);
    destino = encontraParagem(nomeParagemDestino, bd->paragens);

    if (!verificaArgumentosLigacoes1(carreira, origem,
                                     destino, nomeCarreira,
                                     nomeParagemDestino, nomeParagemOrigem,
                                     custo, duracao))
        return;

    alteraDestino = comparaDestinoCarreira(carreira, origem);
    alteraOrigem = comparaOrigemCarreira(carreira, destino);
    primeiraLigacao = (carreira->nodeOrigem == NULL);

    if (!verificaArgumentosLigacoes2(alteraDestino, alteraOrigem, primeiraLigacao))
        return;

    adicionarLigacao(&carreira, &origem, &destino, custo, duracao, alteraOrigem,
                     alteraDestino, primeiraLigacao, bd);
}

/* Função principal do comando 'r' */
void chamaRemoverCarreira(BaseDados *bd)
{
    char nomeCarreira[MAX_TAMANHO_LINHA];
    lerArgumento(nomeCarreira);
    /* a verificacao do argumento é feita posteriormente */
    removeCarreira(&bd, nomeCarreira);
}

/* Função principal do comando 'e' */
void chamarRemoverParagem(BaseDados *bd)
{
    char nomeParagem[MAX_TAMANHO_LINHA];
    lerArgumento(nomeParagem);
    /* a verificação do argumento é feita posteriormente */
    removeParagem(&bd, nomeParagem);
}

/* Função principal do comando 'a' */
void chamarRemoverTudo(BaseDados *bd)
{
    bd->idCarreira = 0;
    removeTodasCarreiras(&bd);
    removeTodasParagens(&bd);
}

/* Inicia a base de dados */
void iniciaBD(BaseDados *bd)
{
    bd->carreiras = NULL;
    bd->paragens = NULL;
    bd->idCarreira = 0;
}

int main()
{
    char c;
    BaseDados bd;
    iniciaBD(&bd);

    while ((c = getchar()) != EOF && (c != '\0'))
    {
        switch (c)
        {
        case 'q': chamarRemoverTudo(&bd);
            return 0;
            break;
        case 'c': chamarCarreira(&bd);
            break;
        case 'p': chamarParagem(&bd);
            break;
        case 'l': chamarLigacoes(&bd);
            break;
        case 'i': mostraIntercesoes(&bd);
            break;
        case 'r': chamaRemoverCarreira(&bd);
            break;
        case 'e': chamarRemoverParagem(&bd);
            break;
        case 'a': chamarRemoverTudo(&bd);
            break;
        }
    }
    chamarRemoverTudo(&bd);
    return 0;
}