#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"

int gera_valor_aleatorio(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

struct mundo *inicializar_mundo(int t_inicio, int t_fim_do_mundo, int tamanhoMundo, int Nhabilidades)
{
    struct mundo *novo_mundo;

    /* aloca a memoria do mundo */
    novo_mundo = malloc(sizeof(struct mundo));
    if (!novo_mundo)
        return NULL;

    int N_herois = Nhabilidades * 5;
    int N_bases = N_herois / 5;
    int N_missoes = t_fim_do_mundo / 100;

    /* Inicializa parametros */

    novo_mundo->NHerois = N_herois;
    novo_mundo->herois = NULL;
    novo_mundo->NBases = N_bases;
    novo_mundo->bases = NULL;
    novo_mundo->NMissoes = N_missoes;
    novo_mundo->missoes = NULL;
    novo_mundo->NHabilidades = Nhabilidades;
    novo_mundo->tamanhoMundo = tamanhoMundo;
    novo_mundo->relogio = t_inicio;

    return novo_mundo;
}

struct heroi *inicializar_heroi(struct heroi *id, int NHabilidades)
{
    struct heroi *novo_heroi;

    novo_heroi = malloc(sizeof(struct heroi));

    if (!novo_heroi)
        return NULL;
    
    novo_heroi->id = id;
    novo_heroi->experiencia = 0;
    novo_heroi->paciencia = gera_valor_aleatorio(0, 100);
    novo_heroi->velocidade = gera_valor_aleatorio(50, 5000);
    novo_heroi->habilidades = gera_valor_aleatorio(1, 3);
    novo_heroi->base = -10;

    return novo_heroi;
}

void destroi_heroi(struct heroi* heroi)
{
    if (heroi)
        free(heroi);
    
    heroi = NULL;
}