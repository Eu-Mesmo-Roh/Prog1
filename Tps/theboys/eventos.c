#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "eventos.h"
#include "entidades.h"
#include "conjunto.h"
#include "lista.h"
#include "fprio.h"

struct eventos_t *cria_evento(int instante, int tipo, int id_heroi, int id_base, int id_missao)
{
    struct eventos_t *novo_evento;

    novo_evento = malloc(sizeof(struct eventos_t));

    if (!novo_evento)
        return NULL;

    novo_evento->instante = instante;
    novo_evento->tipo = tipo;
    novo_evento->id_heroi = id_heroi;
    novo_evento->id_base = id_base;
    novo_evento->id_missao = id_missao;
    novo_evento->prox = NULL;

    return novo_evento;
}

void insere_evento()
{
    struct eventos_t **atual;
}

void processa_evento(struct eventos_t *evento, struct mundo_t *mundo)
{
    switch (evento->tipo)
    {
    case CHEGA:
        processa_chega(evento, mundo);
        break;
    case ESPERA:
        processa_espera(evento, mundo);
        break;
    case DESISTE:
        processa_desiste(evento, mundo);
        break;
    case AVISA:
        processa_avisa(evento, mundo);
        break;
    case ENTRA:
        processa_entra(evento, mundo);
        break;
    case SAI:
        processa_sai(evento, mundo);
        break;
    case VIAJA:
        processa_viaja(evento, mundo);
        break;
    case MORRE:
        processa_morre(evento, mundo);
        break;
    case MISSAO:
        processa_missao(evento, mundo);
        break;
    case FIM:
        processa_fim(evento, mundo);
        break;

    default:
        break;
    }
}

void processa_chega(struct eventos_t *evento, struct mundo_t *mundo)
{
    struct heroi_t *H;
    struct base_t *B;
    struct eventos_t *novo_evento;

    int id_heroi, id_base, paciencia, fila_tamanho;
    bool espera;

    id_heroi = evento->id_heroi;
    id_base = evento->id_base;

    H = &mundo->herois->id;
    B = &mundo->bases->id;

    H->base = id_base;

    if (B->lotacao > lista_tamanho(B->espera))
    {
        novo_evento = cria_evento(mundo->relogio, ESPERA, evento->id_heroi, evento->id_base, NULL);
        insere_evento();
    }

    else
    {
        paciencia = H->paciencia;
        fila_tamanho = lista_tamanho(B->espera);

        if (paciencia > 10 * fila_tamanho)
        {
            struct eventos_t *novo_evento = cria_evento(mundo->relogio, ESPERA, evento->id_heroi, evento->id_base, NULL);
            insere_evento();
        }
        else
        {
            struct eventos_t *novo_evento = cria_evento(mundo->relogio, DESISTE, evento->id_heroi, evento->id_base, NULL);
            insere_evento();
        }
    }
}

void destroi_evento(struct eventos_t *Evento)
{
    if (Evento)
        free(Evento);

    Evento = NULL;
}
