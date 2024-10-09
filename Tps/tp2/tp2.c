/*
 * Tipos Abstratos de Dados - TADs
 * Arquivo do programa principal, que usa o TAD racional.
 * Feito em 24/09/2024 para a disciplina CI1001 - Programação 1.
 */

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* coloque aqui as funções auxiliares que precisar neste arquivo */
void imprimeVet(struct racional vet[], long tam)
{
    int i;

    if (!vet)
        return;

    printf("VETOR = ");

    for (i = 0; i < tam; i++)
    {
        imprime_r(vet[i]);
        printf(" ");
    }

    printf("\n");
}

int retiraNaN(struct racional vet[], long tam)
{
    int i;

    for (i = 0; i < tam; i++)
    {
        if (!valido_r(vet[i]))
        {
            while (!valido_r(vet[tam - 1]))
                tam--;

            if (i != tam)
            {
                vet[i] = vet[tam - 1];
                tam--;
            }
        }
    }

    return tam;
}

void merge(struct racional vet[], int esquerda, int meio, int direita)
{
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    struct racional L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = vet[esquerda + i];
    for (int j = 0; j < n2; j++)
        R[j] = vet[meio + 1 + j];

    int i = 0, j = 0, k = esquerda;

    while (i < n1 && j < n2)
    {
        if (compara_r(L[i], R[j]) <= 0)
        {
            vet[k] = L[i];
            i++;
        }
        else
        {
            vet[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        vet[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        vet[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(struct racional vet[], int esquerda, int direita)
{
    if (esquerda < direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;

        mergeSort(vet, esquerda, meio);
        mergeSort(vet, meio + 1, direita);

        merge(vet, esquerda, meio, direita);
    }
}

struct racional somaTodos(struct racional vet[], long tam)
{
    int i;

    struct racional soma;

    if (!vet)
        return cria_r(0,1);


    soma = vet[0];

    for (i = 1; i < tam; i++)
        soma_r(soma, vet[i], &soma);

    if (!valido_r(soma))
        return cria_r(0,1);

    return soma;
}

/* programa principal */
int main()
{
    struct racional vet[100];
    int n, i;
    long num, den;

    srand(0);

    do
    {
        scanf("%d", &n);
    } while (n < 0 || n > 100); // limita de 0 a 100

    for (i = 0; i < n; i++)
    {
        scanf("%ld %ld", &num, &den);
        vet[i] = cria_r(num, den);
    }
    imprimeVet(vet, n);

    n = retiraNaN(vet, n);
    imprimeVet(vet, n);

    mergeSort(vet, 0, n - 1);
    imprimeVet(vet, n);

    printf("SOMA = ");
    imprime_r(somaTodos(vet, n));

    printf("\n");

        return 0;
}
