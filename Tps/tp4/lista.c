// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct item_t *item_cria(int valor)
{
    struct item_t *item;

    item = malloc(sizeof(struct item_t));
    if (item == NULL)
        return NULL;

    item->ant = NULL;
    item->prox = NULL;
    item->valor = valor;

    return item;
}

void item_destroi(struct item_t *item)
{
    if (item != NULL)
        free(item);

    item = NULL;
}

struct lista_t *lista_cria()
{
    struct lista_t *lista;

    lista = malloc(sizeof(struct lista_t));
    if (lista == NULL)
        return NULL;

    lista->prim = NULL;
    lista->ult = NULL;
    lista->tamanho = 0;

    return lista;
}

struct lista_t *lista_destroi(struct lista_t *lst)
{
    struct item_t *atual, *prox;

    if (lst == NULL)
        return NULL;

    atual = lst->prim; /*atual recebe a primeira posição da lista*/

    while (atual != NULL)
    {
        prox = atual->prox;
        item_destroi(atual);
        atual = prox;
    }

    free(lst);

    lst = NULL;

    return NULL;
}

int lista_insere(struct lista_t *lst, int item, int pos)
{
    struct item_t *novo_item, *aux;

    if (lst == NULL)
        return -1;

    if ((novo_item = item_cria(item)) == NULL)
        return -1;

    if (lst->tamanho == 0)
    {
        lst->prim = novo_item;
        lst->ult = novo_item;
    }

    else if (pos > lst->tamanho || pos < 0)
    {
        novo_item->ant = lst->ult;
        lst->ult->prox = novo_item;
        lst->ult = novo_item;
    }

    else if (pos == 0)
    {
        novo_item->prox = lst->prim;
        lst->prim->ant = novo_item;
        lst->prim = novo_item;
    }

    else
    {
        aux = lst->prim;
        while (aux->prox != NULL || pos != 0)
        {
            aux = aux->prox;
            pos--;
        }
        novo_item->prox = aux->prox;
        novo_item->ant = aux;
        aux->prox = novo_item;
        aux->prox->ant = novo_item;
    }
    lst->tamanho++;

    return lst->tamanho;
}


int lista_retira (struct lista_t *lst, int *item, int pos)
{
    struct intem_t *aux;

    if (lst == NULL)
        return -1;
    
    if (lst->tamanho == 0 || pos > lst->tamanho)
        return -1;

    if (lst->tamanho == 1)
    {
        free(lst->prim);
        lst->prim = NULL;
        lst->ult = NULL;
    }
    
    
    if (pos < 0 || pos == lst->tamanho - 1)
    {
        
    }
    
    

    
    

    
    
    return lst->tamanho;
}

/* int lista_consulta (struct lista_t *lst, int *item, int pos)
{


    if (lst->tamanho == 0)
        return -1;
    
    if (pos > lst->tamanho || pos < 0)
    {
        
    }

    
} */
