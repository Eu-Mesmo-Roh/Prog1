// programa principal do projeto "The Boys - 2024/2"
// Autor: Ronaldo Pereira da Costa, GRR 20240514

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "entidades.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main ()
{
  struct mundo_t *mundo;
  
  mundo = cria_mundo();

  if (!mundo)
  {
    printf("deu pau");
    return (0); /* pensar como liberar memoria se der erro */
  }
    

  destroi_mundo(mundo);

  return (0) ;
}

