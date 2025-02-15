/**
 * PROJETO UNIDADE 7 EMBARCATECH - Rélogio de Xadrez
 * Author: Ruan Victor | @ruanvictordev
 * Campus: Pedro II - PI
*/

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Configurações dos pinos e parâmetros
#define I2C_SDA 14                
#define I2C_SCL 15                
#define BUZZER_PIN 21             
#define BUZZER_FREQUENCY 100      
#define BUZZER_PAUSE_MS 100       
#define LED_PIN 13                
#define BUTTON_A 5    // Brancas
#define BUTTON_B 6    // Pretas

// Atualiza o display mostrando os tempos de ambos os jogadores e o jogador ativo
void display_chess_clock(uint8_t *ssd, struct render_area *area, int white_min, int white_sec, int black_min, int black_sec, int active_player) {
    char buf[30];
    memset(ssd, 0, ssd1306_buffer_length);
    snprintf(buf, sizeof(buf), "Brancas: %02d:%02d", white_min, white_sec);
    ssd1306_draw_string(ssd, 0, 10, buf);
    snprintf(buf, sizeof(buf), "Pretas:  %02d:%02d", black_min, black_sec);
    ssd1306_draw_string(ssd, 0, 30, buf);
    render_on_display(ssd, area);
}

// Pisca o LED para sinalizar um evento
void blink_led() {
    gpio_put(LED_PIN, true);
    sleep_ms(200);
    gpio_put(LED_PIN, false);
    sleep_ms(125);
}

// Inicializa o PWM para o buzzer
void init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

// Emite um beep por uma determinada duração
void beep(uint pin, uint duration_ms) {
    pwm_set_gpio_level(pin, 2048);  // 50% de duty cycle
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
    sleep_ms(BUZZER_PAUSE_MS);
}

// Emite uma sequência de 3 beeps e pisca o LED entre eles
void alert_sequence() {
    for (int i = 0; i < 3; i++) {
        beep(BUZZER_PIN, 1000);
        blink_led();
        sleep_ms(500);
    }
}

int main() {
    stdio_init_all();

    // Inicializa LED e botões (com pull-up)
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Configura o I2C para o display SSD1306
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display e o buzzer
    ssd1306_init();
    init_buzzer(BUZZER_PIN);

    // Configura a área de renderização do display
    struct render_area area = {
        .start_column = 0,
        .end_column = ssd1306_width - 1,
        .start_page = 0,
        .end_page = ssd1306_n_pages - 1
    };
    calculate_render_area_buffer_length(&area);
    uint8_t ssd[ssd1306_buffer_length];

    // Define tempo inicial (em milissegundos) para cada jogador (ex.: 5 minutos)
    int white_time_ms = 5 * 60 * 1000;
    int black_time_ms = 5 * 60 * 1000;
    int active_player = 0; // 0 = Brancas ativas, 1 = Pretas ativas
    bool game_over = false;

    absolute_time_t last = get_absolute_time();
    while (!game_over) {
        absolute_time_t now = get_absolute_time();
        int delta_ms = absolute_time_diff_us(last, now) / 1000;
        last = now;

        // Atualiza o tempo do jogador ativo
        if (active_player == 0) {
            white_time_ms -= delta_ms;
            if (white_time_ms <= 0) {
                white_time_ms = 0;
                game_over = true;
            }
        } else {
            black_time_ms -= delta_ms;
            if (black_time_ms <= 0) {
                black_time_ms = 0;
                game_over = true;
            }
        }

        // Alterna a vez apenas se o botão do jogador ativo for pressionado
        if (active_player == 0 && gpio_get(BUTTON_A) == 0) {
            active_player = 1;
            sleep_ms(200);  // Debounce
        } else if (active_player == 1 && gpio_get(BUTTON_B) == 0) {
            active_player = 0;
            sleep_ms(200);
        }

        // Converte o tempo restante para minutos e segundos
        int white_min = white_time_ms / 60000;
        int white_sec = (white_time_ms / 1000) % 60;
        int black_min = black_time_ms / 60000;
        int black_sec = (black_time_ms / 1000) % 60;

        display_chess_clock(ssd, &area, white_min, white_sec, black_min, black_sec, active_player);
        sleep_ms(50);
    }

    // Determina e exibe o vencedor quando o tempo se esgota
    char final_msg[50];
    if (white_time_ms == 0) {
        int bm = black_time_ms / 60000;
        int bs = (black_time_ms / 1000) % 60;
        snprintf(final_msg, sizeof(final_msg), "Pretas vencem! %02d:%02d", bm, bs);
    } else {
        int wm = white_time_ms / 60000;
        int ws = (white_time_ms / 1000) % 60;
        snprintf(final_msg, sizeof(final_msg), "Brancas vencem! %02d:%02d", wm, ws);
    }
    memset(ssd, 0, ssd1306_buffer_length);
    ssd1306_draw_string(ssd, 0, 20, final_msg);
    render_on_display(ssd, &area);

    alert_sequence();

    while (1)
        sleep_ms(100);

    return 0;
}
