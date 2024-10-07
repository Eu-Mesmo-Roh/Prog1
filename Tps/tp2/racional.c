/* 
 * Tipos Abstratos de Dados - TADs
 * Arquivo de implementação para TAD racional.
 * Feito em 20/09/2024 para a disciplina CI1001 - Programação 1.
 *
 * Este arquivo deve conter as implementações das funções cujos protótipos
 * foram definidos em racional.h. Neste arquivo também podem ser definidas
 * funções auxiliares para facilitar a implementação daquelas funções.
*/

/* coloque aqui seus includes (primeiro os <...>, depois os "...") */
#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/*
 * Implemente aqui as funcoes definidas no racionais.h; caso precise,
 * pode definir aqui funcoes auxiliares adicionais, que devem ser usadas
 * somente neste arquivo.
*/

/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max)
{
  /* implemente aqui */
}

/* Máximo Divisor Comum entre a e b      */
/* calcula o MDC pelo método de Euclides */
long mdc (long a, long b)
{
  /* implemente aqui */
}

/* Mínimo Múltiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b)
{
  /* implemente aqui */
}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r (struct racional r)
{
  /* implemente aqui */
}

/* implemente as demais funções de racional.h aqui */


struct racional cria_r(long numerador, long denominador)
{
  struct racional novo; /* racional = tipo,  novo = nome da variavel */

  novo.num = numerador;
  novo.den = denominador;

  return novo;
}


int valido_r(struct racional r)
{
  if (r.den != 0)
    return 1;

  return 0;
}


struct racional sorteia_r(long min, long max)
{
  struct racional fracao;

  long num, den;

  num = aleat(min, max);
  den = aleat(min, max);

  fracao = cria_r(num, den);

  if (valido_r(fracao) == 0)
    return fracao;

  return simplifica_r(fracao);
}


void imprime_r(struct racional r)
{
  r = simplifica_r(r);

  if (valido_r(r) == 0)
    printf("INVALIDO");

  else if (r.num == 0)
    printf("0");

  else if (r.den == 1)
    printf("%ld",r.num);

  else if (r.den == r.num)
    printf("1");

  else if (r.den < 0)
    printf("%ld/%ld", r.num * (-1), r.den * (-1));

  else
    printf("%ld/%ld", r.num, r.den);
}


int compara_r (struct racional r1, struct racional r2);
{
  
  


}