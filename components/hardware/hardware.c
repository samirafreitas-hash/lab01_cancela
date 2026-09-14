/**
 * @file hardware.c
 * @brief Implementação da camada de HARDWARE.
 *
 * Esta é a ÚNICA tradution unit do projeto que inclui driver/gpio.h.
 */
#include "hardware.h"
#include "driver/gpio.h"

/* ------------------------------------------------------------------ */
/* Mapeamento canal lógico -> GPIO física                             */
/* ------------------------------------------------------------------ */
static const gpio_num_t s_input_gpio[HW_IN_COUNT] = {
    [HW_IN_VEICULO_ENTRADA]   = GPIO_NUM_4,
    [HW_IN_VEICULO_PASSAGEM]  = GPIO_NUM_5,
    [HW_IN_FIM_CURSO_ABERTA]  = GPIO_NUM_18,
    [HW_IN_FIM_CURSO_FECHADA] = GPIO_NUM_19,
    [HW_IN_EMERGENCIA]        = GPIO_NUM_21,
};

static const gpio_num_t s_output_gpio[HW_OUT_COUNT] = {
    [HW_OUT_CMD_ABRIR]      = GPIO_NUM_22,
    [HW_OUT_CMD_FECHAR]     = GPIO_NUM_23,
    [HW_OUT_SINAL_VERDE]    = GPIO_NUM_25,
    [HW_OUT_SINAL_VERMELHO] = GPIO_NUM_26,
};

/* Cada entrada define seu próprio modo de pull, conforme especificação
 * do lab (pelo menos uma com pull-up e outra com pull-down). */
static const gpio_pull_mode_t s_input_pull[HW_IN_COUNT] = {
    [HW_IN_VEICULO_ENTRADA]   = GPIO_PULLDOWN_ONLY,
    [HW_IN_VEICULO_PASSAGEM]  = GPIO_PULLDOWN_ONLY,
    [HW_IN_FIM_CURSO_ABERTA]  = GPIO_PULLUP_ONLY,
    [HW_IN_FIM_CURSO_FECHADA] = GPIO_PULLUP_ONLY,
    [HW_IN_EMERGENCIA]        = GPIO_PULLUP_ONLY,
};

void hardware_init(void)
{
    /* --- Configura entradas --- */
    for (int i = 0; i < HW_IN_COUNT; i++) {
        gpio_config_t cfg = {
            .pin_bit_mask = (1ULL << s_input_gpio[i]),
            .mode         = GPIO_MODE_INPUT,
            .pull_up_en   = (s_input_pull[i] == GPIO_PULLUP_ONLY) ? GPIO_PULLUP_ENABLE  : GPIO_PULLUP_DISABLE,
            .pull_down_en = (s_input_pull[i] == GPIO_PULLDOWN_ONLY) ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE,
            .intr_type    = GPIO_INTR_DISABLE,
        };
        gpio_config(&cfg);
    }

    /* --- Configura saídas --- */
    uint64_t out_mask = 0;
    for (int i = 0; i < HW_OUT_COUNT; i++) {
        out_mask |= (1ULL << s_output_gpio[i]);
    }
    gpio_config_t out_cfg = {
        .pin_bit_mask = out_mask,
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_cfg);

    /* Garante estado inicial seguro: tudo desligado */
    for (int i = 0; i < HW_OUT_COUNT; i++) {
        gpio_set_level(s_output_gpio[i], 0);
    }
}

bool hardware_read_raw(hw_input_t input)
{
    if (input >= HW_IN_COUNT) {
        return false;
    }
    return gpio_get_level(s_input_gpio[input]) != 0;
}

void hardware_write_raw(hw_output_t output, bool level)
{
    if (output >= HW_OUT_COUNT) {
        return;
    }
    gpio_set_level(s_output_gpio[output], level ? 1 : 0);
}
