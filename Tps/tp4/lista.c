// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


struct lista_t *lista_cria ()
{
    struct lista_t* lista;

    lista = malloc (sizeof(struct lista_t));
    if (lista == NULL)
        return NULL;
    
    lista->prim = NULL;
    lista->ult = NULL;
    lista->tamanho = 0;

    return lista;    
}

struct lista_t *lista_destroi (struct lista_t *lst)
{
    if (lst == NULL)
        return NULL;

    struct item_t *atual, *prox;

    atual = lst->prim; /*atual recebe a primeira posição da lista*/

    while (atual != NULL)
    {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(lst);
        
}

int lista_insere (struct lista_t *lst, int item, int pos)
{
    

}

