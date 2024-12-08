#ifndef EVENTOS
#define EVENTOS

#include <math.h>
#include <stdbool.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"

/* Definições de cada evento */
#define EVENT_CHEGA 1
#define EVENT_ESPERA 2
#define EVENT_DESISTE 3
#define EVENT_AVISA 4
#define EVENT_ENTRA 5
#define EVENT_SAI 6
#define EVENT_VIAJA 7
#define EVENT_MORRE 8
#define EVENT_MISSAO 9
#define EVENT_FIM 10


/* Descreve um Evento */
struct evento_t
{
    int id_heroi;
    int id_base;
    int id_missao;
    int id_base_dest;
};

/* Cria um Evento */
/* Retorno: ponteiro para o evento criado ou NULL se erro */
struct evento_t *cria_evento(int id_heroi, int id_base, int id_missao, int id_base_dest);

/*Função responsável por tratar a chegada de um novo evento no mundo.*/
void evento_chega(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

/*Função responsável por tratar a espera de um novo evento no mundo.*/
void evento_espera(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

/*Função responsável por tratar a desistencia de um heroi .*/
void evento_desiste(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

/* Função responsável por tirar um heroi da fila caso haja vaga na base */
void evento_avisa(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

void evento_entra(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

void evento_sai(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio);

void evento_viaja(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, int id_base_destino, struct fprio_t *fprio);

void evento_morre(struct mundo_t *mundo, int tempo, int id_heroi, struct fprio_t *fprio);

void evento_missao(struct mundo_t *mundo, int tempo, int id_missao, struct fprio_t *fprio);

void evento_fim(struct mundo_t *mundo, struct fprio_t *fprio);

void evento_inicio(struct mundo_t *mundo, struct fprio_t *fprio);

void processa_evento(struct mundo_t *mundo, struct fprio_t *fprio);


#endif
