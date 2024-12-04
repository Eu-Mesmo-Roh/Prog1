#ifndef ENTIDADES
#define ENTIDADES

/* Defines */
#define T_INICIO = 0;
#define T_FIM_DO_MUNDO = 525600;
#define N_TAMANHO_MUNDO = 20000;
#define N_HABILIDADES = 10;

/* Descreve um local */
struct local
{
    int x; /* cordenadas x */
    int y; /* cordenadas y */
};

/* Descreve um Heroi */
struct heroi
{
    int id;          /* número inteiro ≥ 0 que identifica unicamente o herói */
    int habilidades; /* conjunto de habilidades que o herói possui */
    int paciencia;   /* número inteiro ≥ 0 que indica quão paciente um Heroi é */
    int velocidade;  /* número inteiro ≥ 0 indicando a velocidade de deslocamento de um herói */
    int experiencia; /* número inteiro ≥ 0 que indica o número de missões em que o herói já participou */
    int base;        /* ID da base onde o herói se encontra no momento */
};

/* cria um heroi */
/* Retorno: ponteiro para o heroi criado ou NULL se erro */
struct heroi *inicializar_heroi();

/* Libera todas as estruturas de dados do heroi */
/* Retorno: NULL */
void destroi_heroi();

/* Descreve a base dos Herois */
struct base
{
    int id;                   /* número inteiro ≥ 0 que identifica cada base */
    int lotacao;              /* número máximo de heróis na base */
    struct cjto_t *presentes; /* conjunto que contitue quais herois estão atualmente na base */
    struct fprio *espera;     /* fila de espera para poder entrar na base */
    struct local;             /* localização da base */
};

/* cria uma base */
/* Retorno: ponteiro para a base criada ou NULL se erro */
struct base *cria_base();

/* Libera todas as estruturas de dados da base */
/* Retorno: NULL */
void destroi_base();

/* Descreve uma misão */
struct missao
{
    int id;                    /* número inteiro ≥ 0 que identifica a missão */
    struct cjto_ *habilidades; /* conjunto de habilidades necessárias para cumprir a missão */
    int perigo;                /* nível de perigo da missão */
    struct local;              /* localização da missão */
};

/* cria uma missão */
/* Retorno: ponteiro para a missão criada ou NULL se erro */
struct missao *cria_missao();

/* Libera todas as estruturas de dados da missão */
/* Retorno: NULL */
void destroi_missao();

/* Descreve um mundo */
struct mundo
{
    int NHerois;             /* número total de heróis no mundo */
    struct heroi *herois;    /* vetor representando todos os heróis */
    int NBases;              /* número total de bases no mundo */
    struct base *bases;      /* vetor representando todas as bases */
    int NMissoes;            /* número total de missões a cumprir */
    struct missoes *missoes; /* vetor representando todas as missões */
    int NHabilidades;        /* número de habilidades distintas possíveis */
    int tamanhoMundo;        /* coordenadas máximas do mundo */
    int relogio;             /* número inteiro ≥ 0 indicando o tempo atual no mundo. */
};

/* cria um mundo */
/* Retorno: ponteiro para o mundo criado ou NULL se erro */
struct mundo *inicializar_mundo(int t_inicio, int t_fim_do_mundo, int tamanhoMundo, int Nhabilidades);

/* Libera todas as estruturas de dados do mundo */
/* Retorno: NULL */
struct mundo *liberar_mundo();

#endif