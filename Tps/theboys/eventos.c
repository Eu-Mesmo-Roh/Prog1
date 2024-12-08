#include <stdio.h>
#include <stdlib.h>

#include "eventos.h"
#include "entidades.h"
#include "conjunto.h"
#include "lista.h"
#include "fprio.h"

/* Calcula a Distancia entre os pontos */
long calculo_da_distancia(struct local_t *coordenada, struct local_t *prox_coordenada)
{
    long distancia, x, y;

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
int ponteiros_validos(struct mundo_t *mundo, struct fprio_t *fprio)
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
    evento = cria_evento(id_heroi, id_base, -10, -10);

    if (ponteiros_validos(mundo, fprio) || !evento)
        return;

    mundo->herois[id_heroi].base = id_base;

    bool espera = false;
    /*se há vagas na base e a fila de espera está vazia:*/
    if (cjto_card(mundo->bases[id_base].presentes) < mundo->bases[id_base].lotacao && lista_tamanho(mundo->bases[id_base].espera) == 0)
        espera = true;
    else if (lista_tamanho(mundo->bases[id_base].espera) > 0)
        /*Se for maior espera TRUE, FALSE caso contrario*/
        espera = (mundo->herois[id_heroi].paciencia > 10 * lista_tamanho(mundo->bases[id_base].espera));

    /* impreção do evento */

    /* se não espera, chama o evento desiste */
    if (espera)
        fprio_insere(fprio, evento, EVENT_ESPERA, tempo);
    else
        fprio_insere(fprio, evento, EVENT_DESISTE, tempo);
}

void evento_espera(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    evento = cria_evento(id_heroi, id_base, -10, -10);

    if (ponteiros_validos(mundo, fprio) || !evento)
        return;

    /* adiciona o heroi na fila de espera da base */
    lista_insere(mundo->bases[id_base].espera, id_heroi, tempo);

    fprio_insere(fprio, evento, EVENT_AVISA, tempo);
}

void evento_desiste(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;
    int base_dest;

    /* escolhe uma base destino aleatória */
    base_dest = gera_valor_aleatorio(0, mundo->NBases - 1);

    evento = cria_evento(id_heroi, id_base, -10, base_dest);

    if (ponteiros_validos(mundo, fprio) || !evento)
        return;

    /* print da função */

    fprio_insere(fprio, evento, EVENT_VIAJA, tempo);
}

void evento_avisa(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;

    if (ponteiros_validos(mundo, fprio))
        return;

    /* print da função */
    /* lista_imprime(mundo->bases[id_base].espera); */

    /* enquanto ouver vaga na base e houver herois esperando na fila */
    while ((cjto_card(mundo->bases[id_base].presentes) < mundo->bases[id_base].lotacao) && !(lista_tamanho(mundo->bases[id_base].espera)))
    {

        /* retira o primeiro heroi da fila da base, armazena o id do heroi */
        lista_retira(mundo->bases[id_base].espera, &id_heroi, 0);

        evento = cria_evento(id_heroi, id_base, -10, -10);

        if (!evento)
            return;

        /*print do codigo*/

        fprio_insere(fprio, evento, EVENT_ENTRA, tempo);
    }
}

void evento_entra(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento;

    int tempo_na_base;

    evento = cria_evento(id_heroi, id_base, -10, -10);

    if (ponteiros_validos(mundo, fprio) || !evento)
        return;
    /* adiciona o heroi ao conjunto de herois presentes na base */
    cjto_insere(mundo->bases[id_base].presentes, id_heroi);

    /* calcula o tempo em que o heroi permanece na base */
    tempo_na_base = (mundo->herois[id_heroi].paciencia * gera_valor_aleatorio(1, 13) + 15);

    /* print do codigo */

    fprio_insere(fprio, evento, EVENT_SAI, tempo + tempo_na_base);
}

void evento_sai(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, struct fprio_t *fprio)
{
    struct evento_t *evento1, *evento2;

    int base_dest;

    base_dest = gera_valor_aleatorio(0, mundo->NBases - 1);

    evento1 = cria_evento(id_heroi, id_base, -10, base_dest);
    evento2 = cria_evento(id_heroi, id_base, -10, -10);

    if (ponteiros_validos(mundo, fprio) || !evento1 || !evento2)
        return;

    /* retira o heroi do conjunto de herois presente na base */
    cjto_retira(mundo->bases[id_base].presentes, id_heroi);

    /* escolhe uma base com destino aleatorio */

    /*insere na fila de prioridade o evento viaja*/
    fprio_insere(fprio, evento1, EVENT_VIAJA, tempo);

    /* insere na fila de prioridade o evento avisa */
    fprio_insere(fprio, evento2, EVENT_AVISA, tempo);
}

void evento_viaja(struct mundo_t *mundo, int tempo, int id_heroi, int id_base, int id_base_destino, struct fprio_t *fprio)
{
    struct evento_t *evento;

    int base_origem, distancia, duracao;

    evento = cria_evento(id_heroi, id_base_destino, -10, -10);

    if (ponteiros_validos(mundo, fprio) || !evento)
        return;

    base_origem = mundo->herois[id_heroi].base;

    /* calcula a distancia entre a base de origem e a base de destino */
    distancia = calculo_da_distancia(mundo->bases[base_origem].coordenadas, mundo->bases[id_base].coordenadas);

    /* calcula o tempo de viagem */
    duracao = distancia / mundo->herois[id_heroi].velocidade;

    /* print do codigo */

    fprio_insere(fprio, evento, EVENT_CHEGA, tempo + duracao);
}

void evento_morre(struct mundo_t *mundo, int tempo, int id_heroi, struct fprio_t *fprio)
{
    struct evento_t *evento;

    evento = cria_evento(-10, mundo->herois[id_heroi].base, -10, -10);

    if (ponteiros_validos(mundo, fprio) || !evento)
        return;

    cjto_retira(mundo->bases[mundo->herois[id_heroi].base].presentes, id_heroi);

    fprio_insere(fprio, evento, EVENT_AVISA, tempo);
}

void evento_missao(struct mundo_t *mundo, int tempo, int id_missao, struct fprio_t *fprio)
{
    struct evento_t *evento;
    struct cjto_t *habilidades_base;

    int id_base[mundo->NBases], distancia[mundo->NBases];
    int i, j, base_apta;

    evento = cria_evento(-10, -10, id_missao, -10);

    if (ponteiros_validos(mundo, fprio) || !evento)
        return;

    /* print do codigo */

    cjto_imprime(mundo->missoes[id_missao].habilidades);

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

        /* print do código */
        cjto_imprime(habilidades_base);

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
        /* print do código */
        cjto_imprime(mundo->bases[id_base[i]].presentes);

        mundo->missoes[id_missao].realizadas = 1;

        /* incrementa a experiencia dos herois presentes na base */
        for (j = 0; j < mundo->NHerois; j++)
            if (cjto_pertence(mundo->bases[id_base[i]].presentes, j))
                mundo->herois[j].experiencia++;
        
        free(evento);
    }
    else
    {
        /* se não saiu da função, a missao é impossivel */
        /*print do codigo*/
        mundo->NMissaoImpossivel++;

        fprio_insere(fprio, evento, EVENT_MISSAO, tempo + 60 * 24);
    }
}

void evento_fim(struct mundo_t *mundo, struct fprio_t *fprio)
{
    int missoes_cumpridas, tentaivas, i;

    missoes_cumpridas = 0;
    tentaivas = 0;

    ponteiros_validos(mundo, fprio);

    printf("%d: FIM\n", mundo->tempo);

    /* imprime o status final de casa heroi */
    for (i = 0; i < mundo->NHerois; i++)
    {
        /*print do codigo*/
        cjto_imprime(mundo->herois[i].habilidades);
    }

    /* faz o calculo das missoes cumpridas e das tentativas */
    for (i = 0; i < mundo->NMissoes; i++)
    {
        tentaivas = mundo->missoes[i].tentativas + tentaivas;
        missoes_cumpridas += mundo->missoes[i].realizadas;
    }

    /* print do código */
}

void evento_inicio(struct mundo_t *mundo, struct fprio_t *fprio)
{
    struct evento_t *evento = NULL;
    int base, tempo, i;

    if (ponteiros_validos(mundo, fprio))
        return;

    /* cria evento chega para cada heroi */
    for (i = 0; i < mundo->NHerois; i++)
    {
        base = gera_valor_aleatorio(0, mundo->NBases - 1);
        tempo = gera_valor_aleatorio(0, 4320); /*  4320 = 60*24*3 = 3 dias  */
        if (!(evento = cria_evento(i, base, -10, -10)))
            return;
        fprio_insere(fprio, evento, EVENT_CHEGA, tempo);
    }

    /* cria evento missao para cada missao */
    for (i = 0; i < mundo->NMissoes; i++)
    {
        tempo = gera_valor_aleatorio(4320, T_FIM_DO_MUNDO - 1);
        if (!(evento = cria_evento(-10, -10, i, -10)))
            return;
        fprio_insere(fprio, evento, EVENT_MISSAO, tempo);
    }

    /* cria fim do mundo */
    if (!(evento = cria_evento(-10, -10, -10, -10)))
        return;
    fprio_insere(fprio, evento, EVENT_FIM, T_FIM_DO_MUNDO);
}

void processa_evento(struct mundo_t *mundo, struct fprio_t *fprio)
{
    struct evento_t *evento = NULL;

    int tipo, tempo;

    evento = fprio_retira(fprio, &tipo, &tempo);

    while (!evento && tempo <= T_FIM_DO_MUNDO)
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
            evento_viaja(mundo, tempo, evento->id_heroi, evento->id_base, evento->id_base_dest, fprio);
            break;
        case EVENT_MORRE:
            evento_morre(mundo, tempo, evento->id_heroi, fprio);
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
