/**
 * @file hardware.h
 * @brief Camada de HARDWARE (Lab 01 - Portas Digitais e Abstração de Hardware)
 *
 * Única camada do sistema que conhece números de GPIO e chama diretamente
 * o driver gpio.h do ESP-IDF. Nenhum outro módulo deve incluir "driver/gpio.h".
 *
 * Expõe um conjunto de "canais" lógicos (enum), desacoplando quem usa
 * (camada de serviços) do número físico do pino.
 */
#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------- Canais de ENTRADA (chaves) ---------------- */
typedef enum {
    HW_IN_VEICULO_ENTRADA = 0,   /* veículo aguardando entrada   - pull-down */
    HW_IN_VEICULO_PASSAGEM,      /* veículo na passagem           - pull-down */
    HW_IN_FIM_CURSO_ABERTA,      /* cancela totalmente aberta     - pull-up   */
    HW_IN_FIM_CURSO_FECHADA,     /* cancela totalmente fechada    - pull-up   */
    HW_IN_EMERGENCIA,            /* botão/chave de emergência     - pull-up   */
    HW_IN_COUNT
} hw_input_t;

/* ---------------- Canais de SAÍDA (LEDs) --------------------- */
typedef enum {
    HW_OUT_CMD_ABRIR = 0,        /* LED comando de abertura   */
    HW_OUT_CMD_FECHAR,           /* LED comando de fechamento */
    HW_OUT_SINAL_VERDE,          /* LED sinal verde            */
    HW_OUT_SINAL_VERMELHO,       /* LED sinal vermelho         */
    HW_OUT_COUNT
} hw_output_t;

/**
 * @brief Inicializa todos os pinos de entrada e saída utilizados no lab.
 *        Configura pull-up/pull-down internos conforme cada canal.
 */
void hardware_init(void);

/**
 * @brief Lê o nível ELÉTRICO BRUTO do pino associado à entrada (1 = alto, 0 = baixo).
 *        Não faz nenhuma interpretação de "ativo em alto/baixo" — isso é
 *        responsabilidade da camada de serviços.
 */
bool hardware_read_raw(hw_input_t input);

/**
 * @brief Escreve o nível ELÉTRICO BRUTO no pino associado à saída.
 */
void hardware_write_raw(hw_output_t output, bool level);

#ifdef __cplusplus
}
#endif

#endif /* HARDWARE_H */
