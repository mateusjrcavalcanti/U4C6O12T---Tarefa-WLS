#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "matriz.h"

#define I2C_PORTA i2c1    // Define o barramento I2C
#define I2C_PINO_SDA 14   // Define o pino SDA
#define I2C_PINO_SCL 15   // Define o pino SCL
#define I2C_ENDERECO 0x3C // Define o endereço do display
#define PINO_VERMELHO 13  // GPIO 11 (LED vermelho)
#define PINO_VERDE 11     // GPIO 13 (LED verde)
#define PINO_AZUL 12      // GPIO 12 (LED AZUL)
#define PINO_BOTAO_A 5    // GPIO 5 (botão A)
#define PINO_BOTAO_B 6    // GPIO 6 (botão B)

// struct para armazenar o estado dos dados
typedef struct
{
  char *estado_azul;
  char *estado_verde;
  char caractere[2];
  bool fill;
  int numero_exibido;
} estado_t;

ssd1306_t ssd;   // Inicializa a estrutura do display
estado_t estado; // Inicializa a estrutura do estado

estado_t estado = {
    .estado_azul = "off",  // Inicializa o estado do LED azul
    .estado_verde = "off", // Inicializa o estado do LED verde
    .caractere = " ",      // Inicializa o array com ' ' e '\0'
    .fill = false,         // Inicializa o estado fill
    .numero_exibido = 10   // Inicializa o número exibido
};

volatile uint32_t ultimo_tempo_btn_a = 0;
volatile uint32_t ultimo_tempo_btn_b = 0;

/**
 * Função de debounce para os botões.
 */
void debounce(uint gpio, uint32_t events)
{
  uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());         // Tempo atual em ms
  if (gpio == PINO_BOTAO_A && (tempo_atual - ultimo_tempo_btn_a > 500)) // Debouncing de 500ms
  {
    estado.estado_verde = strcmp(estado.estado_verde, "off") == 0 ? " on" : "off"; // Alterna o estado do LED verde
    printf("Botão A pressionado, LED verde: %s\n", estado.estado_verde);           // Exibe o estado do LED verde
  }
  else if (gpio == PINO_BOTAO_B && (tempo_atual - ultimo_tempo_btn_b > 500)) // Debouncing de 500ms
  {
    estado.estado_azul = strcmp(estado.estado_azul, "off") == 0 ? " on" : "off"; // Alterna o estado do LED azul
    printf("Botão B pressionado, LED azul: %s\n", estado.estado_azul);           // Exibe o estado do LED azul
  }

  gpio_put(PINO_VERDE, strcmp(estado.estado_verde, " on") == 0 ? 1 : 0); // Acende ou apaga o LED verde
  gpio_put(PINO_AZUL, strcmp(estado.estado_azul, " on") == 0 ? 1 : 0);   // Acende ou apaga o LED azul
}

// Função de callback para o alarme
int64_t callback_alarme(alarm_id_t id, void *user_data)
{
  estado.fill = !estado.fill;                          // Inverte o estado fill
  add_alarm_in_ms(1000, callback_alarme, NULL, false); // Reagendar para 1s
  return 0;
}

/*
 * Inicializa o display
 */
void inicializa_display()
{
  i2c_init(I2C_PORTA, 400 * 1000); // Inicializa o barramento I2C com 400 kHz

  gpio_set_function(I2C_PINO_SDA, GPIO_FUNC_I2C); // Configura o pino GPIO para I2C (SDA)
  gpio_set_function(I2C_PINO_SCL, GPIO_FUNC_I2C); // Configura o pino GPIO para I2C (SCL)
  gpio_pull_up(I2C_PINO_SDA);                     // Pull up the data line
  gpio_pull_up(I2C_PINO_SCL);                     // Pull up the clock line

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, I2C_ENDERECO, I2C_PORTA); // Inicializa o display
  ssd1306_config(&ssd);                                              // Configura o display
  ssd1306_send_data(&ssd);                                           // Envia os dados para o display
  ssd1306_fill(&ssd, false);                                         // Limpa o display
  ssd1306_send_data(&ssd);                                           // Envia os dados para o display
}

/**
 * Configura os botões A e B.
 */
void inicializa_botoes()
{
  gpio_init(PINO_BOTAO_A);                                                               // Inicializa o pino do botão A
  gpio_set_dir(PINO_BOTAO_A, GPIO_IN);                                                   // Configura o pino do botão A como entrada
  gpio_pull_up(PINO_BOTAO_A);                                                            // Habilita o pull-up no pino do botão A
  gpio_set_irq_enabled_with_callback(PINO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &debounce); // Habilita a interrupção no pino do botão A

  gpio_init(PINO_BOTAO_B);                                                               // Inicializa o pino do botão B
  gpio_set_dir(PINO_BOTAO_B, GPIO_IN);                                                   // Configura o pino do botão B como entrada
  gpio_pull_up(PINO_BOTAO_B);                                                            // Habilita o pull-up no pino do botão B
  gpio_set_irq_enabled_with_callback(PINO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &debounce); // Habilita a interrupção no pino do botão B
}

/**
 * Inicializa LEDs
 */
void inicializaLEDs()
{
  gpio_init(PINO_VERMELHO);
  gpio_init(PINO_AZUL);
  gpio_init(PINO_VERDE);
  gpio_set_dir(PINO_VERMELHO, GPIO_OUT);
  gpio_set_dir(PINO_AZUL, GPIO_OUT);
  gpio_set_dir(PINO_VERDE, GPIO_OUT);

  gpio_put(PINO_VERMELHO, 0);
  gpio_put(PINO_VERDE, 0);
  gpio_put(PINO_AZUL, 0);
}

/**
 * Função de callback para a UART
 */
void uart_rx()
{
  int  c = getchar_timeout_us(0);
  if (c == PICO_ERROR_TIMEOUT || c == '\r') 
  {
    return;
  }

  // Verifica se é letra (A-Z ou a-z) ou número (0-9)
  if (!( (c >= 'A' && c <= 'Z') || 
         (c >= 'a' && c <= 'z') || 
         (c >= '0' && c <= '9') ))
  {
    estado.caractere[0] = ' ';
  }
  else
  {
    estado.caractere[0] = (char)c;
    
    // Verifica se é dígito numérico
    if (c >= '0' && c <= '9')
    {
      estado.numero_exibido = c - '0';
    }
  }
  estado.caractere[1] = '\0';

  printf("recebido: %c\n", c);
}

int main()
{
  stdio_init_all();     // Inicializa entradas e saídas.
  inicializaMatriz();   // Inicializa a matriz de LEDs
  inicializa_display(); // Inicializa o display
  inicializa_botoes();  // Inicializa os botões
  inicializaLEDs();     // Inicializa os LEDs

  add_alarm_in_ms(1000, callback_alarme, NULL, false); // Adiciona um alarme para 1s

  while (true)
  {
    ssd1306_fill(&ssd, !estado.fill);                             // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, estado.fill, !estado.fill); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "LED AZUL", 8, 6);                  // Desenha o texto para o LED azul
    ssd1306_draw_string(&ssd, estado.estado_azul, 12 * 8, 6);     // Desenha o valor do estado do LED azul
    ssd1306_draw_string(&ssd, "LED VERDE", 8, 20);                // Desenha o texto para o LED verde
    ssd1306_draw_string(&ssd, estado.estado_verde, 12 * 8, 20);   // Desenha o valor do estado do LED verde
    ssd1306_draw_string(&ssd, "CARACTERE:", 10, 34);              // Desenha o texto para o caractere
    ssd1306_draw_string(&ssd, estado.caractere, 13 * 8, 34);      // Desenha o caractere atual
    ssd1306_draw_string(&ssd, "Mateus Jr", 28, 48);               // Desenha o texto com o nome do autor
    ssd1306_send_data(&ssd);                                      // Atualiza o display

    sleep_ms(100); // Aguarda 100 ms

    mostraNumero(estado.numero_exibido); // Exibe o número na matriz de LEDs
    tight_loop_contents();               // Aguarda a interrupção dos botões

    uart_rx(); // Recebe o caractere da UART
  }
}