#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdio.h>           // Para uso de funções de entrada e saída padrão.
#include "pico/stdlib.h"     // Para uso de funções de entrada e saída padrão.
#include "hardware/clocks.h" // Para configuração de clock.
#include "hardware/irq.h"    // Para uso de interrupções.

// Definições de pinos.
#define MATRIZ 7 // Pino de controle da matriz de LEDs.

// Definições de constantes para a matriz de LEDs.
#define NUMLEDSMATRIZ 25    // Número de LEDs na matriz.
#define LIGADO {1, 0, 0}    // Cor vermelha.
#define DESLIGADO {0, 0, 0} // Cor preta.

/**
 * Estruturas de dados.
 */
struct pixel_t // Estrutura de pixel.
{
    uint8_t R, G, B; // Três valores (Red, Green, Blue) de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t; // Define um tipo de dado pixel_t.

void mostraNumero(int numero);         // Exibe um número na matriz de LEDs.
void inicializaMatriz();

#endif