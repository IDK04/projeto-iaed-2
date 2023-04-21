#include <stdlib.h>
#ifndef MAIN
#define MAIN

#define VERDADE 1
#define FALSO 0
#define NAO_EXISTE -1

#define MAX_TAMANHO_LINHA 65535

/* Estruturas */

typedef struct nID
{
    int id;
    struct nID *proxima; 
} *nodeIDCarreira;

typedef struct paragem
{
    char *nome;
    double latitude, longitude;
    int numCarreiras;
    nodeIDCarreira idCarreiras;
} Paragem;

typedef struct ligacao
{
    Paragem *origem, *destino;
    int idCarreira;
    double custo, duracao;
} Ligacao;

typedef struct nL
{
    Ligacao ligacao;
    struct nL *proxima, *anterior;
} *nodeLigacao;

typedef struct 
{
    char *nome;
    int id;
    double custoTotal;
    double duracaoTotal;
    nodeLigacao nodeOrigem, nodeDestino;
    int numParagens;
} Carreira;

typedef struct nP
{
    Paragem paragem;
    struct nP *proxima;
} *nodeParagem;

typedef struct nC
{
    Carreira carreira;
    struct nC *proxima;
} *nodeCarreira;

typedef struct 
{
    nodeCarreira carreiras;
    int idCarreira;
    nodeParagem paragens;
} BaseDados;


/* ---------- */

/* Prototipos */
char limparComando();
int lerArgumento(char *argumento);

void leArgumentoCarreira(int* numArgumentos, char* buffer,
                         char* argumentoOpcional);
int verificaArgumentosCarreira(char *argumentoOpcional);
nodeCarreira adicionarCarreira(char *nomeCarreira, BaseDados *bd);
Carreira* encontraCarreira(int id, char *nome, nodeCarreira head);
void listarCarreiras(nodeCarreira head);
void listarParagensCarreira(Carreira *carreira, int inverso);
void removeCarreira(BaseDados **bd, char *nomeCarreira);
void removeCarreiraDeParagem(Paragem *paragem, int id);
nodeLigacao popNodeLigacao(nodeLigacao head);
void removeTodasCarreiras(BaseDados **bd);

void leArgumentosParagem(int* numArgumentos, char *nomeParagem,
                         double *latitude, double *longitude);
int verificaArgumentosParagem(int numArgumentos, Paragem *paragem,
                              char *nomeParagem);
Paragem* encontraParagem(char *nomeParagem, nodeParagem head);
nodeParagem adicionarParagem(char *nomeParagem, double latitude,
                             double longitude, BaseDados *bd);
void listarParagens(nodeParagem head);
void detalharParagem(Paragem *paragem);
void removeParagem(BaseDados **bd, char *nomeParagem);
void removeTodasParagens(BaseDados **bd);

void leArgumentoLigacoes(char *nomeCarreira, char *nomeParagemOrigem, 
                         char *nomeParagemDestino, double *custo, 
                         double *duracao);
int verificaArgumentosLigacoes1(Carreira *carreira, Paragem *origem,
                               Paragem *destino, char *nomeCarreira,
                               char *nomeParagemDestino, char *nomeParagemOrigem,
                               double custo, double duracao);
int verificaArgumentosLigacoes2(int alteraDestino, int alteraOrigem,
                                int primeiraLigacao);
int comparaDestinoCarreira(Carreira *carreira, Paragem *paragem);
int comparaOrigemCarreira(Carreira *carreira, Paragem *paragem);
void adicionarLigacao(Carreira **carreira, Paragem **origem, Paragem **destino,
                              double custo, double duracao, int alteraOrigem,
                              int alteraDestino, int primeiraLigacao,
                              BaseDados *bd);

void mostraIntercesoes(BaseDados *bd);

int strVazia(char str[]);
void chamarRemoverTudo(BaseDados *bd);
void* tenta_malloc(BaseDados *bd, size_t tamanho);

void troca(int* n1, int* n2);
void bubbleSort(int vetor[], int tamanho, int (*comparacao)(int,int,nodeCarreira),
                nodeCarreira head);
/* ---------- */

#endif