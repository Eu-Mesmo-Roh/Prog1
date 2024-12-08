// programa principal do projeto "The Boys - 2024/2"
// Autor: Ronaldo Pereira da Costa, GRR 20240514

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "entidades.h"
#include "eventos.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main()
{
  struct mundo_t *mundo;
  struct fprio_t *fprio;

  /* cria e inicializa o mundo */
  fprio = fprio_cria();
  mundo = cria_mundo();
  evento_inicio(mundo, fprio);

  processa_evento(mundo, fprio);
  
  destroi_mundo(mundo);

  fprio_destroi(fprio);

  return (0);
}