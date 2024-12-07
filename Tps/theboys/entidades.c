#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "entidades.h"

int gera_valor_aleatorio(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

struct heroi_t *cria_heroi(int NHerois)
{
    struct heroi_t *novo_heroi;
    int i;

    novo_heroi = malloc(sizeof(struct heroi_t) * NHerois);

    if (!novo_heroi)
        return NULL;

    for (i = 0; i < NHerois; i++)
    {
        novo_heroi[i].id = i;
        novo_heroi[i].experiencia = 0;
        novo_heroi[i].paciencia = gera_valor_aleatorio(0, 100);
        novo_heroi[i].velocidade = gera_valor_aleatorio(50, 5000);
        novo_heroi[i].habilidades = cjto_cria(gera_valor_aleatorio(1, 3));
        novo_heroi[i].base = -10;
    }

    return novo_heroi;
}

void destroi_heroi(struct heroi_t *heroi)
{
    if (heroi)
        free(heroi);

    heroi = NULL;
}

struct base_t *cria_base(int NBases, int tamanhoMundo)
{
    struct base_t *nova_base;
    int i;

     nova_base = malloc(sizeof(struct base_t) * NBases);

    if (!nova_base)
        return NULL;

    for (i = 0; i < NBases; i++)
    {
        nova_base[i].id = i;
        nova_base[i].coordenadas->x = gera_valor_aleatorio(0, T_FIM_DO_MUNDO);
        nova_base[i].coordenadas->y = gera_valor_aleatorio(0, T_FIM_DO_MUNDO);
        nova_base[i].lotacao = gera_valor_aleatorio(3, 10);
        nova_base[i].presentes = cjto_cria(nova_base[i].lotacao);
        nova_base[i].espera = lista_cria();
    }
    return nova_base;
}

void destroi_base(struct base_t* base)
{
    if (base)
        free(base);

    base = NULL;
}


struct missao_t *cria_missao(int NMissao, int tamanhoMundo)
{
    struct missao_t *nova_missao;
    int i;

    nova_missao = malloc(sizeof(struct missao_t) * NMissao);

    if (!nova_missao)
        return NULL;
    
    for (i = 0; i < NMissao; i++)
    {
        nova_missao[i].id = i;
        nova_missao[i].coordenadas->x = gera_valor_aleatorio(0, tamanhoMundo);
        nova_missao[i].coordenadas->y = gera_valor_aleatorio(0, tamanhoMundo);
        nova_missao[i].habilidades = cjto_cria(gera_valor_aleatorio(6, 10));
        nova_missao[i].perigo = gera_valor_aleatorio(0, 100);


    }
    return nova_missao;
}

void destroi_missao(struct missao_t *missao)
{
    if (missao)
        free(missao);

    missao = NULL;
}


struct mundo_t *cria_mundo()
{
    struct mundo_t *novo_mundo;

    /* aloca a memoria do mundo */
    novo_mundo = malloc(sizeof(struct mundo_t));
    if (!novo_mundo)
        return NULL;
    int N_herois, N_bases, N_missoes;

    N_herois = N_HABILIDADES * 5;
    N_bases = N_herois / 5;
    N_missoes = T_FIM_DO_MUNDO / 100;

    /* Inicializa parametros */

    novo_mundo->tamanhoMundo = T_INICIO;
    novo_mundo->NHabilidades = N_HABILIDADES;
    novo_mundo->NHerois = N_herois;
    novo_mundo->herois = cria_heroi(N_herois);
    novo_mundo->NBases = N_bases;
    novo_mundo->bases = cria_base(N_bases, N_TAMANHO_MUNDO);
    novo_mundo->NMissoes = N_missoes;
    novo_mundo->missoes = cria_missao(N_missoes, N_TAMANHO_MUNDO);
    novo_mundo->relogio = T_INICIO;


    return novo_mundo;
}

void destroi_mundo(struct mundo_t *mundo)
{
    if (!mundo)
        return;
    
    destroi_missao(mundo->missoes);

    destroi_base(mundo->bases);

    destroi_heroi(mundo->herois);

    if (mundo)
        free(mundo);
    
    mundo = NULL;    
}
