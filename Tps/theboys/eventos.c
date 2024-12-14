#include <stdio.h>
#include <stdlib.h>

#include "eventos.h"
#include "entidades.h"
#include "conjunto.h"
#include "lista.h"
#include "fprio.h"

/* Calcula a Distancia entre os pontos */
int calculo_da_distancia(struct local_t *coordenada, struct local_t *prox_coordenada)
{
    int distancia, x, y;

    x = (prox_coordenada->x - coordenada->x) * (prox_coordenada->x - coordenada->x);
    y = (prox_coordenada->y - coordenada->y) * (prox_coordenada->y - coordenada->y);

    distancia = sqrt(x + y);

    return distancia;
}
/* faz a junção das habilidades dos herois presentes na base */
struct cjto_t *conjunto_habilidades_da_base(struct mundo_t *mundo, int id_base)
{
    struct cjto_t *conj_habilidades, *temp;
    int i;

    conj_habilidades = cjto_cria(1);

    if (!conj_habilidades)
        return NULL;

    i = 0;

    while (i < mundo->NHerois)
    {
        if (cjto_pertence(mundo->bases[id_base].presentes, i))
        {
            temp = cjto_uniao(mundo->herois[i].habilidades, conj_habilidades);
            cjto_destroi(conj_habilidades);
            conj_habilidades = cjto_copia(temp);
            cjto_destroi(temp);
        }

        i++;
    }
    return conj_habilidades;
}

void troca(int vetor[], int vetor_id[], int a, int b)
{
    int aux, aux_id;
    aux = vetor[a];
    aux_id = vetor_id[a];

    vetor[a] = vetor[b];
    vetor_id[a] = vetor_id[b];

    vetor[b] = aux;
    vetor_id[b] = aux_id;
}

/* ordena os vetores de distancia e id_base, com base na ordenação selection sort */
void ordena_vetor(int vetor[], int vetor_id[], int n)
{

    int i, j, min;
    for (i = 0; i < n - 1; i++)
    {
        min = i;
        for (j = i + 1; j < n; j++)
            if (vetor[j] < vetor[min])
                min = j;
        troca(vetor, vetor_id, i, min);
    }
}

/* testa se os ponteiros do mundo e a fila de prioridade existem, se não existem, retorna o erro.*/
int ponteiros_invalidos(struct mundo_t *mundo, struct fprio_t *fprio)
{
    if (!mundo)
    {
        printf("Erro no ponteiro mundo");
        return 1;
    }
    if (!mundo->missoes)
    {
        printf("Erro no ponteiro mundo->missoes");
        return 1;
    }
    if (!mundo->bases)
    {
        printf("Erro no ponteiro mundo->bases");
        return 1;
    }
    if (!mundo->herois)
    {
        printf("Erro no ponteiro mundo->herois");
        return 1;
    }
    if (!fprio)
    {
        printf("Erro no ponteiro fprio");
        return 1;
    }

    return 0;
}

int heroi_V_ou_M(struct mundo_t *mundo, int id_heroi)
{
    if (mundo->herois[id_heroi].status == 0)
        return 0;

    return 1;
}

/* cria um evento */
struct evento_t *cria_evento(int id_heroi, int id_base, int id_missao, int id_base_dest)
{
    struct evento_t *ev;
    ev = malloc(sizeof(struct evento_t));

    if (!ev)
        return NULL;

    ev->id_heroi = id_heroi;
    ev->id_base = id_base;
    ev->id_missao = id_missao;
    ev->id_base_dest = id_base_dest;

    return ev;
}

/*Eventos*/
void evento_chega(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    bool espera = false;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento chega\n");
        return;
    }

    mundo->herois[id_heroi].base = id_base;

    /*se há vagas na base e a fila de espera está vazia:*/
    if (cjto_card(mundo->bases[id_base].presentes) < mundo->bases[id_base].lotacao && lista_tamanho(mundo->bases[id_base].espera) == 0)
        espera = 1;
    else if (mundo->herois[id_heroi].paciencia > (10 * lista_tamanho(mundo->bases[id_base].espera)))
        /*Se for maior espera TRUE, FALSE caso contrario*/
        espera = 1;
    else
        espera = 0;

    /* print do evento */
    printf("%6d: CHEGA  HEROI %2d ", tempo, id_heroi);
    printf("BASE %d (%2d/%2d) ", id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao);

    /* se não espera, chama o evento desiste */
    if (espera)
    {
        fprio_insere(fprio, evento, EVENT_ESPERA, tempo);
        printf("ESPERA");
    }
    else
    {
        fprio_insere(fprio, evento, EVENT_DESISTE, tempo);
        printf("DESISTE");
    }
    printf("\n");
}

void evento_espera(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_vazio, id_base, id_vazio, id_vazio);

    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento espera\n");
        return;
    }

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, id_heroi, id_base, cjto_card(mundo->bases[id_base].presentes));

    /* adiciona o heroi na fila de espera da base */
    lista_insere(mundo->bases[id_base].espera, id_heroi, fim_da_lista);

    fprio_insere(fprio, evento, EVENT_AVISA, tempo);
}

void evento_desiste(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int base_dest;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    /* escolhe uma base destino aleatória */
    base_dest = gera_valor_aleatorio(0, mundo->NBases - 1);

    evento = cria_evento(id_heroi, id_vazio, id_vazio, base_dest);

    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento desiste\n");
        return;
    }

    /* print da função */
    printf("%6d: DESISTE HEROI %2d BASE %d \n", tempo, id_heroi, id_base);

    fprio_insere(fprio, evento, EVENT_VIAJA, tempo);
}

void evento_avisa(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int h;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    if (ponteiros_invalidos(mundo, fprio))
        return;

    /* print da função */
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d)", tempo, id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao);
    lista_imprime(mundo->bases[id_base].espera);

    /* enquanto ouver vaga na base e houver herois esperando na fila */
    while ((cjto_card(mundo->bases[id_base].presentes) < mundo->bases[id_base].lotacao) && !(lista_tamanho(mundo->bases[id_base].espera)))
    {
        /* retira o primeiro heroi da fila da base, armazena o id do heroi */
        h = lista_retira(mundo->bases[id_base].espera, &id_heroi, inicio_da_lista);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, id_base, id_heroi);

        /* adiciona o heroi ao conjunto de herois presentes na base */
        cjto_insere(mundo->bases[id_base].presentes, h);

        evento = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
        if (!evento)
        {
            printf("ERRO    Não alocou evento avisa\n");
            return;
        }

        fprio_insere(fprio, evento, EVENT_ENTRA, tempo);
    }
}

void evento_entra(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int tempo_permanencia_base;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_heroi, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento entra\n");
        return;
    }

    /* calcula o tempo em que o heroi permanece na base */
    tempo_permanencia_base = (mundo->herois[id_heroi].paciencia * gera_valor_aleatorio(1, 20) + 15);

    /* print do codigo */
    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, id_heroi, id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao, tempo + tempo_permanencia_base);

    /* cria proximo evento que acontecera em (tempo + tempo_permanencia_base) */
    fprio_insere(fprio, evento, EVENT_SAI, tempo + tempo_permanencia_base);
}

void evento_sai(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento1, *evento2;
    int base_dest;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    /* escolhe uma base com destino aleatorio */
    base_dest = gera_valor_aleatorio(0, mundo->NBases - 1);

    evento1 = cria_evento(id_heroi, id_vazio, id_vazio, base_dest);
    evento2 = cria_evento(id_vazio, id_base, id_vazio, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento1 || !evento2)
    {
        printf("ERRO    Não alocou evento sai\n");
        return;
    }

    /* print do evento */
    printf("%6d: SAI  HEROI %2d BASE %d (%2d/%2d)\n", tempo, id_heroi, id_base, cjto_card(mundo->bases[id_base].presentes), mundo->bases[id_base].lotacao);

    /* retira o heroi do conjunto de herois presente na base */
    cjto_retira(mundo->bases[id_base].presentes, id_heroi);

    /*insere na fila de prioridade o evento viaja*/
    fprio_insere(fprio, evento1, EVENT_VIAJA, tempo);

    /* insere na fila de prioridade o evento avisa */
    fprio_insere(fprio, evento2, EVENT_AVISA, tempo);
}

void evento_viaja(struct mundo_t *mundo, int tempo, int id_heroi, int id_base_destino, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int distancia, duracao;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_heroi, id_vazio, id_vazio, id_base_destino);
    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento viaja\n");
        return;
    }

    /* calcula a distancia entre a base de origem e a base de destino */
    distancia = calculo_da_distancia(mundo->bases[mundo->herois[id_heroi].base].coordenadas, mundo->bases[id_base_destino].coordenadas);

    /* calcula o tempo de viagem */
    duracao = distancia / mundo->herois[id_heroi].velocidade;

    /* print do codigo */
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, id_heroi, mundo->herois[id_heroi].base, id_base_destino, distancia, mundo->herois[id_heroi].velocidade, tempo + duracao);

    fprio_insere(fprio, evento, EVENT_CHEGA, tempo + duracao);
}

void evento_morre(struct mundo_t *mundo, int tempo, int id_heroi, int id_missao, struct fprio_t *fprio)
{
    struct evento_t *evento;

    if (!heroi_V_ou_M(mundo, id_heroi))
        return;

    evento = cria_evento(id_vazio, mundo->herois[id_heroi].base, id_vazio, id_vazio);

    if (ponteiros_invalidos(mundo, fprio) || !evento)
    {
        printf("ERRO    Não alocou evento morre\n");
        return;
    }

    /* retira o heroi da base */
    cjto_retira(mundo->bases[mundo->herois[id_heroi].base].presentes, id_heroi);

    /* muda o status do herois */
    mundo->herois[id_heroi].status = 0;

    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", tempo, id_heroi, id_missao);

    fprio_insere(fprio, evento, EVENT_AVISA, tempo);
}

void evento_missao(struct mundo_t *mundo, int tempo, int id_missao, struct fprio_t *fprio)
{
    struct evento_t *evento1, *evento2;
    struct cjto_t *habilidades_base;
    int id_base[mundo->NBases], distancia[mundo->NBases];
    int i, j, base_apta, risco;

    evento1 = cria_evento(id_vazio, id_vazio, id_missao, id_vazio);
    if (ponteiros_invalidos(mundo, fprio) || !evento1)
    {
        printf("ERRO    Não Alocou evento missao");
        return;
    }

    /* print do codigo */
    printf("%6d: MISSAO %d TENT %d HAB REQ: ", tempo, id_missao, mundo->missoes[id_missao].tentativas);
    cjto_imprime(mundo->missoes[id_missao].habilidades);
    printf("\n");

    /* mais uma tentativa */
    mundo->missoes[id_missao].tentativas++;

    /* armazena em um vetor o indice e o outro a distancia para serem ordenados */
    for (i = 0; i < mundo->NBases; i++)
    {
        distancia[i] = calculo_da_distancia(mundo->bases[i].coordenadas, mundo->missoes[id_missao].coordenadas);
        id_base[i] = i;
    }

    ordena_vetor(distancia, id_base, mundo->NBases);

    /* apos o vetor ser ordenado, começa a buscar a base mais proxima */
    base_apta = -1;

    for (i = 0; i < mundo->NBases; i++)
    {
        habilidades_base = conjunto_habilidades_da_base(mundo, id_base[i]);

        /* caso o conjuto de habilidades da base tenha todas as habilidades da missao, ela pode ser cumprida */
        if (cjto_contem(mundo->missoes[id_missao].habilidades, habilidades_base) || cjto_iguais(habilidades_base, mundo->missoes[id_missao].habilidades))
        {
            base_apta = id_base[i];
            cjto_destroi(habilidades_base);
            break;
        }
        cjto_destroi(habilidades_base);
    }

    if (base_apta >= 0)
    {
        free(evento1);
        /* print do código */
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", tempo, id_missao, id_base[i]);
        cjto_imprime(habilidades_base);
        printf("\n");

        mundo->missoes[id_missao].realizadas = 1;

        /* incrementa a experiencia dos herois presentes na base */
        for (j = 0; j < mundo->NHerois; j++)
        {
            if (cjto_pertence(mundo->bases[base_apta].presentes, j))
            {
                if (!heroi_V_ou_M(mundo, mundo->herois[j].id))
                    return;

                risco = mundo->missoes[id_missao].perigo / (mundo->herois[j].paciencia + mundo->herois[j].experiencia + 1.0);
                if (risco > gera_valor_aleatorio(0, 30))
                {
                    evento2 = cria_evento(mundo->herois[j].id, base_apta, id_missao, id_vazio);
                    if (!evento2)
                    {
                        printf("ERRO    evento morre");
                        return;
                    }
                    fprio_cria(fprio, evento2, EVENT_MORRE, tempo);
                }
                else
                    mundo->herois[j].experiencia++;
            }
        }
    }
    else
    {
        /* se não saiu da função, a missao é impossivel */
        /*print do codigo*/
        printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, id_missao);
        mundo->NMissaoImpossivel++;

        fprio_insere(fprio, evento1, EVENT_MISSAO, tempo + 60 * 24);
    }
}

void evento_fim(struct mundo_t *mundo, struct fprio_t *fprio)
{
    int missoes_cumpridas, tentaivas, i;
    missoes_cumpridas = 0;
    tentaivas = 0;

    if (ponteiros_invalidos(mundo, fprio))
        return;

    printf("%d: FIM\n", mundo->tempo);

    /* imprime o status final de casa heroi */
    for (i = 0; i < mundo->NHerois; i++)
    {
        /*print do codigo*/
        if (heroi_V_ou_M(mundo, mundo->herois[i].id))
        {
            printf("HEROI %2d VIVO  PAC %3d", i, mundo->herois[i].paciencia);
        }
        else
            printf("HEROI %2d MORTO  PAC %3d", i, mundo->herois[i].paciencia);

        printf(" VEL %4d EXP %4d HABS ", mundo->herois[i].velocidade, mundo->herois[i].experiencia);
        cjto_imprime(mundo->herois[i].habilidades);
        printf("\n");
    }

    /* faz o calculo das missoes cumpridas e das tentativas */
    for (i = 0; i < mundo->NMissoes; i++)
    {
        tentaivas = mundo->missoes[i].tentativas + tentaivas;
        missoes_cumpridas += mundo->missoes[i].realizadas;
    }

    float taxa;

    taxa = (missoes_cumpridas/ mundo->NMissoes) * 100;

    /* print do código */
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)", mundo->NMissoes, missoes_cumpridas, taxa);
}

void evento_inicio(struct mundo_t *mundo, struct fprio_t *fprio)
{
    struct evento_t *evento = NULL;
    int base, tempo, i;

    if (ponteiros_invalidos(mundo, fprio))
        return;

    /* cria evento chega para cada heroi */
    for (i = 0; i < mundo->NHerois; i++)
    {
        base = gera_valor_aleatorio(0, mundo->NBases - 1);
        tempo = gera_valor_aleatorio(0, 4320); /*  4320 = 60*24*3 = 3 dias  */
        if (!(evento = cria_evento(i, base, id_vazio, id_vazio)))
            return;
        fprio_insere(fprio, evento, EVENT_CHEGA, tempo);
    }

    /* cria evento missao para cada missao */
    for (i = 0; i < mundo->NMissoes; i++)
    {
        tempo = gera_valor_aleatorio(4320, T_FIM_DO_MUNDO - 1);
        if (!(evento = cria_evento(id_vazio, id_vazio, i, id_vazio)))
            return;
        fprio_insere(fprio, evento, EVENT_MISSAO, tempo);
    }
    /* cria fim do mundo */
    if (!(evento = cria_evento(id_vazio, id_vazio, id_vazio, id_vazio)))
        return;
    fprio_insere(fprio, evento, EVENT_FIM, T_FIM_DO_MUNDO);
}

void processa_evento(struct mundo_t *mundo, struct fprio_t *fprio)
{
    struct evento_t *evento = NULL;

    int tipo, tempo;

    evento = fprio_retira(fprio, &tipo, &tempo);
    if (!evento)
    {
        printf("ERRO    Erro no switch");
        return;
    }

    while (evento && tempo <= T_FIM_DO_MUNDO)
    {
        switch (tipo)
        {
        case EVENT_CHEGA:
            evento_chega(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            break;
        case EVENT_ESPERA:
            evento_espera(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            break;
        case EVENT_DESISTE:
            evento_desiste(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            break;
        case EVENT_AVISA:
            evento_avisa(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            break;
        case EVENT_ENTRA:
            evento_entra(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            break;
        case EVENT_SAI:
            evento_sai(mundo, tempo, evento->id_heroi, evento->id_base, fprio);
            break;
        case EVENT_VIAJA:
            evento_viaja(mundo, tempo, evento->id_heroi, evento->id_base_dest, fprio);
            break;
        case EVENT_MORRE:
            evento_morre(mundo, tempo, evento->id_heroi, evento->id_missao, fprio);
            break;
        case EVENT_MISSAO:
            evento_missao(mundo, tempo, evento->id_missao, fprio);
            break;
        case EVENT_FIM:
            evento_fim(mundo, fprio);
            free(evento);
            printf("%d\n", fprio_tamanho(fprio));
            return;
        }
        free(evento);
        evento = fprio_retira(fprio, &tipo, &tempo);
    }
    if (evento)
        free(evento);
}
