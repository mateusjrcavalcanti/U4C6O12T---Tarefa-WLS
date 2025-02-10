# Projeto Raspberry Pi Pico W com Display SSD1306 e LEDs

Este projeto utiliza o microcontrolador Raspberry Pi Pico W para controlar LEDs e um display SSD1306, além de interagir com botões e UART para exibir informações na tela OLED.

## Funcionalidades

- **Controle de LEDs**: Dois LEDs (azul e verde) podem ser alternados através dos botões A e B.
- **Display SSD1306**: Exibe o estado dos LEDs, caractere recebido via UART e informações adicionais.
- **UART**: Recebe caracteres via UART e exibe o caractere no display.
- **Matriz de LEDs**: Exibe um número (recebido via UART) em uma matriz de LEDs (função `mostraNumero`).

## Componentes

- **Raspberry Pi Pico W**: Microcontrolador utilizado para o projeto.
- **Display SSD1306**: Display OLED conectado via I2C para exibição de informações.
- **LEDs**: LEDs azul, verde e vermelho conectados a pinos GPIO.
- **Botões**: Botões A e B para controlar os LEDs.

## Pinagem

| Componente       | Pino GPIO |
|------------------|-----------|
| SDA (I2C)        | 14        |
| SCL (I2C)        | 15        |
| LED Vermelho     | 13        |
| LED Verde        | 11        |
| LED Azul         | 12        |
| Botão A          | 5         |
| Botão B          | 6         |

## Funcionalidades do Código

### 1. **Debounce para os Botões**
A função `debounce` é utilizada para evitar leituras múltiplas de um único pressionamento de botão, com um intervalo de 500ms.

### 2. **Display SSD1306**
O display SSD1306 é configurado para exibir as seguintes informações:
- Estado dos LEDs (azul e verde).
- Caractere recebido via UART.
- Nome do autor.

A tela é atualizada a cada 100ms.

### 3. **Controle de LEDs**
- O LED **verde** é alternado quando o botão A é pressionado.
- O LED **azul** é alternado quando o botão B é pressionado.

### 4. **Recepção de Dados via UART**
Quando um caractere é recebido pela UART, ele é exibido no display. Caso o caractere seja um número, ele é armazenado e exibido na matriz de LEDs.

## Vídeo

Veja o vídeo demonstrativo deste projeto: [Assistir no YouTube](https://youtu.be/cvxJ4TuaMyw)