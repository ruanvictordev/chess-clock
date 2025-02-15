# Projeto Unidade 7 EmbarcaTech - Relógio de Xadrez

### Autor: Ruan Victor | [@ruanvictordev](https://github.com/ruanvictordev)
**Campus:** Pedro II - PI

---

## Descrição do Projeto
Este projeto consiste em um relógio de xadrez implementado no Raspberry Pi Pico utilizando a biblioteca SSD1306 para controle de display OLED via I2C, além de manipulação de GPIOs para LEDs, botões e um buzzer para alertas sonoros.

O relógio permite a contagem regressiva do tempo de jogo para dois jogadores (Brancas e Pretas), alternando entre eles com o pressionar de botões, e emite um alerta sonoro quando o tempo de um jogador se esgota.

---

## Funcionalidades
- **Display OLED SSD1306**: Exibição do tempo restante para cada jogador.
- **LED indicador**: Pisca a cada alternância de turno.
- **Buzzer PWM**: Emite alertas sonoros configuráveis.
- **Botões físicos**: Utilizados para alternar o jogador ativo.
- **Sistema de tempo real**: Controle preciso do tempo através das funções do Pico SDK.

---

## Estrutura do Projeto
```
├── inc/
│   └── ssd1306.h          # Biblioteca SSD1306
├── src/
│   └── chess-clock.c             # Código principal do projeto
├── CMakeLists.txt         # Configuração de build
└── README.md              # Este documento
```

---

## Componentes Utilizados
- **Microcontrolador**: Raspberry Pi Pico W
- **Display OLED**: SSD1306 (128x64 pixels)
- **LED**: LED de sinalização
- **Buzzer**: Buzzer Piezoelétrico
- **Botões**: Dois botões de entrada para controle

---

## Pinagem do Projeto
| **Componente** | **Pino Pico** |
|----------------|---------------|
| SDA (I2C)      | GP14          |
| SCL (I2C)      | GP15          |
| LED            | GP13          |
| Buzzer         | GP21          |
| Botão A        | GP5           |
| Botão B        | GP6           |

---

## Como Executar

### Pré-requisitos
- Raspberry Pi Pico configurado com o SDK do Pico
- Ferramentas de compilação (CMake, GCC ARM)

### Passos
1. Clone o repositório:
   ```bash
   git clone https://github.com/ruanvictordev/relogio-xadrez.git
   ```
2. Acesse o diretório do projeto:
   ```bash
   cd relogio-xadrez
   ```
3. Configure o build:
   ```bash
   mkdir build && cd build
   cmake ..
   ```
4. Compile o projeto:
   ```bash
   make
   ```
5. Carregue o binário no Raspberry Pi Pico (segurando o BOOTSEL ao conectar via USB).

---

## Detalhes Técnicos
- **Linguagem**: C
- **SDK**: Raspberry Pi Pico SDK
- **I2C**: Comunicação com display OLED
- **PWM**: Controle do buzzer
- **GPIO**: Entrada e saída digital

---

## Contato
- **GitHub**: [ruanvictordev](https://github.com/ruanvictordev)
- **LinkedIn**: [Ruan Victor](https://linkedin.com/in/ruanvictordev)
- **Instagram**: [@ruanvictordev](https://instagram.com/ruanvictordev)

---

> Projeto desenvolvido como parte da disciplina de Sistemas Embarcados na Unidade 7 do EmbarcaTech - Campus Pedro II - PI.