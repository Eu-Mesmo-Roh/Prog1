#ifndef EVENTOS
#define EVENTOS

#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"


/* Descreve um Evento */
struct eventos_t
{
    int instante;       /*Momento em que o evento ocorre*/
    int tipo;           /* tipo do evento */
    int id_heroi;       /* id do heroi que esta no evento */
    int id_base;        /* id da base que esta no evento */
    int id_missao;      /* id da missão (caso tenha) */
    struct Eventos* prox;   /* próximo evento*/
};

/* Tipos de Eventos */
enum
{
    CHEGA,
    ESPERA,
    DESISTE,
    AVISA,
    ENTRA,
    SAI,
    VIAJA,
    MORRE,
    MISSAO,
    FIM
};

/* Cria e inicializa os eventos */
/* Retorno: Ponteiro do novo evento */
struct eventos_t *cria_evento();

/* Insere eventos na LEF */
/* Retorno: NULL */
void insere_evento();

/* Retira Eventos da LEF */
/* Retorno: Evento retirado */
struct eventos_t *retira_evento();

/* Processa o Evento Retirado */
/* Retorno: NULL */
void processa_evento();

/* Libera todas as estruturas de dados do Evento */
/* Retorno: NULL */
void destroi_evento();



#endif
