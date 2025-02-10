#include <matriz.h>
#include "ws2818b.pio.h"     // Biblioteca gerada pelo arquivo .pio durante compilação.

/**
 * Variáveis globais.
 */
pixel_t leds[NUMLEDSMATRIZ]; // Buffer de pixels.
PIO np_pio;                  // Ponteiro para a máquina PIO.
uint sm;                     // Número da máquina PIO.

/**
 * Matriz de números de 0 a 9.
 */
int numeros[10][5][5][3] = {
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},    // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - * - * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - * - * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - * - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO}     // - * * * -
    },
    {
        {DESLIGADO, DESLIGADO, LIGADO, DESLIGADO, DESLIGADO}, // - - * - -
        {DESLIGADO, LIGADO, LIGADO, DESLIGADO, DESLIGADO},    // - * * - -
        {DESLIGADO, DESLIGADO, LIGADO, DESLIGADO, DESLIGADO}, // - - * - -
        {DESLIGADO, DESLIGADO, LIGADO, DESLIGADO, DESLIGADO}, // - - * - -
        {DESLIGADO, DESLIGADO, LIGADO, DESLIGADO, DESLIGADO}  // - - * - -
    },
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, DESLIGADO, DESLIGADO}, // - * - - -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO}        // - * * * -
    },
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO}        // - * * * -
    },
    {
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO},    // - * - * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO},    // - * - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}  // - - - * -
    },
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, DESLIGADO, DESLIGADO}, // - * - - -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO}        // - * * * -
    },
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, DESLIGADO, DESLIGADO}, // - * - - -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO},    // - * - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO}        // - * * * -
    },
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}  // - - - * -
    },
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},    // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - * - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},    // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - * - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO}     // - * * * -
    },
    {
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, LIGADO, DESLIGADO, LIGADO, DESLIGADO},    // - * - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO},       // - * * * -
        {DESLIGADO, DESLIGADO, DESLIGADO, LIGADO, DESLIGADO}, // - - - * -
        {DESLIGADO, LIGADO, LIGADO, LIGADO, DESLIGADO}        // - * * * -
    }};

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void inicializa(uint pino)
{

  uint offset = pio_add_program(pio0, &ws2818b_program); // Adiciona programa à máquina PIO.
  np_pio = pio0;                                         // Inicia com a máquina PIO 0.

  sm = pio_claim_unused_sm(np_pio, false); // Tenta obter uma máquina livre.
  if (sm < 0)                              // Se nenhuma máquina estiver livre, tenta obter outra máquina.
  {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  ws2818b_program_init(np_pio, sm, offset, pino, 800000.f); // Inicia programa na máquina PIO obtida.

  for (uint i = 0; i < NUMLEDSMATRIZ; ++i) // Limpa buffer de pixels.
  {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void definineCor(const uint indice, const uint8_t r, const uint8_t g, const uint8_t b)
{
  leds[indice].R = r;
  leds[indice].G = g;
  leds[indice].B = b;
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void defineFrame()
{
  for (uint i = 0; i < NUMLEDSMATRIZ; ++i) // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Limpa o buffer de pixels.
 */
void limpa()
{
  for (uint i = 0; i < NUMLEDSMATRIZ; ++i)
    definineCor(i, 0, 0, 0);
  defineFrame();
}

/**
 * Exibe uma matriz de cores na matriz de LEDs.
 */
void mostraFrame(int matriz[5][5][3])
{
  for (int y = 0; y < 5; y++)
  {
    for (int x = 0; x < 5; x++)
    {
      int posicao;
      if (y % 2 == 0)
      {
        posicao = 24 - (y * 5 + x);
      }
      else
      {
        posicao = 24 - (y * 5 + (4 - x));
      }
      definineCor(posicao, matriz[y][x][0], matriz[y][x][1], matriz[y][x][2]);
    }
  }

  defineFrame();
  sleep_ms(1);
}

/**
 * Exibe um número na matriz de LEDs.
 */
void mostraNumero(int numero)
{
  if (numero >= 0 && numero <= 9)
  {
    mostraFrame(numeros[numero]);
  }
}

/**
 * Inicializa a matriz de LEDs.
 */
void inicializaMatriz()
{
  inicializa(MATRIZ);
  limpa();
}