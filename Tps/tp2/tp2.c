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

    for (i = 0; i < tam; i++)
    {
        imprime_r(vet[i]);
        printf(" ");
    }
        
    printf("\n");       
}

void retiraNaN(struct racional vet[], long tam)
{
    int i;
    
    for (i = 0; i < tam; i++)
    {
        if (!valido_r(vet[i]))
        {
            while (!valido_r(vet[tam-1]))
            {
                tam--;
            }
            
            vet[i] = vet[tam-1];
        }
        
    }
    
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

}
