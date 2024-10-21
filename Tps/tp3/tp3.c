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
void imprimeVet(struct racional **vet, long tam)
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

int retiraNaN(struct racional **vet, long tam)
{
  int i;

  for (i = 0; i < tam; i++)
  {
    /* verifica se o elemento do vetor na posição v[i] é valido*/
    if (!valido_r(vet[i]))
    {
      /* verifica se o elemento na ultima posição do vetor é valido
       * caso seja o remove */
      while (!valido_r(vet[tam - 1]))
        tam--;

      /* remove o elemento da posição v[i] */
      if (i != tam)
      {
        vet[i] = vet[tam - 1];
        tam--;
      }
    }
  }

  return tam;
}

/* programa principal */
int main()
{
  int n, i;
  long num, den;

  srand(0);

  do
  {
    scanf("%d", &n);
  } while (n < 0 || n > 100); // limita de 0 a 100

  struct racional **vet = malloc(sizeof(struct racional *) * n);
  if (vet == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória.\n");
    return 1; // Saída com erro
  }

  for (i = 0; i < n; i++)
  {
    scanf("%ld %ld", &num, &den);
    vet[i] = cria_r(num, den);
  }

  imprimeVet(vet, n);

  n = retiraNaN(vet, n);
  imprimeVet(vet, n);

  



  return (0);
}
