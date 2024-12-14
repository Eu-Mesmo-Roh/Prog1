#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "entidades.h"

int gera_valor_aleatorio(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

struct heroi_t *cria_heroi(struct mundo_t *mundo)
{
    struct heroi_t *novo_heroi;
    int i;

    novo_heroi = malloc(sizeof(struct heroi_t) * mundo->NHerois);

    if (!novo_heroi)
        return NULL;

    for (i = 0; i < mundo->NHerois; i++)
    {
        novo_heroi[i].id = i;
        novo_heroi[i].experiencia = 0;
        novo_heroi[i].paciencia = gera_valor_aleatorio(0, 100);
        novo_heroi[i].velocidade = gera_valor_aleatorio(50, 5000);
        novo_heroi[i].habilidades = cjto_cria(gera_valor_aleatorio(1, 3));
        novo_heroi[i].base = -10;
        novo_heroi[i].status = 1;
    }

    return novo_heroi;
}

void destroi_heroi(struct mundo_t *mundo)
{
    int i;

    if (!mundo || !mundo->herois)
        return;

    for (i = 0; i < mundo->NHerois; i++)
    {
        if (mundo->herois[i].habilidades)
            cjto_destroi(mundo->herois[i].habilidades);
    }

    free(mundo->herois);
    mundo->herois =NULL;
}

struct base_t *cria_base(struct mundo_t *mundo)
{
    struct base_t *nova_base;
    int i;

    if (!mundo)
        return NULL;

    nova_base = malloc(sizeof(struct base_t) * mundo->NBases);
    if (!nova_base)
        return NULL;

    for (i = 0; i < mundo->NBases; i++)
    {
        nova_base[i].id = i;
        nova_base[i].lotacao = gera_valor_aleatorio(3, 10);
        nova_base[i].presentes = cjto_cria(nova_base[i].lotacao);
        nova_base[i].espera = lista_cria();

        nova_base[i].coordenadas = malloc(sizeof(struct local_t));
        if (!nova_base[i].coordenadas)
            return NULL;

        nova_base[i].coordenadas->x = gera_valor_aleatorio(0, T_FIM_DO_MUNDO - 1);
        nova_base[i].coordenadas->y = gera_valor_aleatorio(0, T_FIM_DO_MUNDO - 1);
    }
    return nova_base;
}

void destroi_base(struct mundo_t *mundo)
{
    int i;

    if (!mundo || !mundo->bases)
        return;

    for (i = 0; i < mundo->NBases; i++)
    {
        cjto_destroi(mundo->bases[i].presentes);
        lista_destroi(mundo->bases[i].espera);
        free(mundo->bases[i].coordenadas);
    }

    free(mundo->bases);
    mundo->bases = NULL;
}

struct missao_t *cria_missao(struct mundo_t *mundo)
{
    struct missao_t *nova_missao;
    int i;

    nova_missao = malloc(sizeof(struct missao_t) * mundo->NMissoes);
    if (!nova_missao)
        return NULL;

    for (i = 0; i < mundo->NMissoes; i++)
    {
        nova_missao[i].id = i;
        nova_missao[i].habilidades = cjto_cria(gera_valor_aleatorio(6, 10));
        nova_missao[i].perigo = gera_valor_aleatorio(0, 100);
        nova_missao[i].realizadas = 0;
        nova_missao[i].tentativas = 0;

        nova_missao[i].coordenadas = malloc(sizeof(struct local_t));
        if (!nova_missao[i].coordenadas)
            return NULL;

        nova_missao[i].coordenadas->x = gera_valor_aleatorio(0, N_TAMANHO_MUNDO - 1);
        nova_missao[i].coordenadas->y = gera_valor_aleatorio(0, N_TAMANHO_MUNDO - 1);
    }

    return nova_missao;
}

void destroi_missao(struct mundo_t *mundo)
{
    int i;

    if (!mundo || !mundo->missoes)
        return;

    for (i = 0; i < mundo->NMissoes; i++)
    {
        cjto_destroi(mundo->missoes[i].habilidades);
        free(mundo->missoes[i].coordenadas);
    }

    free(mundo->missoes);
    mundo->missoes = NULL;
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
    novo_mundo->tamanhoMundo = N_TAMANHO_MUNDO;
    novo_mundo->NHabilidades = N_HABILIDADES;
    novo_mundo->NHerois = N_herois;
    novo_mundo->herois = cria_heroi(novo_mundo);
    novo_mundo->NBases = N_bases;
    novo_mundo->bases = cria_base(novo_mundo);
    novo_mundo->NMissoes = N_missoes;
    novo_mundo->missoes = cria_missao(novo_mundo);
    novo_mundo->NMissaoImpossivel = 0;
    novo_mundo->tempo = T_INICIO;
    

    return novo_mundo;
}

void destroi_mundo(struct mundo_t *mundo)
{
    if (!mundo)
        return;

    destroi_missao(mundo);

    destroi_base(mundo);

    destroi_heroi(mundo);

    if (mundo)
        free(mundo);

    mundo = NULL;
}
