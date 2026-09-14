/**
 * @file services.c
 * @brief Implementação da camada de SERVIÇOS.
 *
 * Único módulo, além da própria camada de hardware, que inclui hardware.h.
 * Aqui fica concentrada a decisão de "ativo em alto" x "ativo em baixo"
 * para cada sensor.
 */
#include "services.h"
#include "hardware.h"

void services_init(void)
{
    hardware_init();
}

/* Entradas com pull-down: repouso = 0, acionada = 1 -> ativo em ALTO */
bool sensor_veiculo_aguardando_entrada(void)
{
    return hardware_read_raw(HW_IN_VEICULO_ENTRADA) == true;
}

bool sensor_veiculo_na_passagem(void)
{
    return hardware_read_raw(HW_IN_VEICULO_PASSAGEM) == true;
}

/* Entradas com pull-up: repouso = 1, acionada = 0 -> ativo em BAIXO */
bool sensor_cancela_aberta(void)
{
    return hardware_read_raw(HW_IN_FIM_CURSO_ABERTA) == false;
}

bool sensor_cancela_fechada(void)
{
    return hardware_read_raw(HW_IN_FIM_CURSO_FECHADA) == false;
}

bool sensor_emergencia_acionada(void)
{
    return hardware_read_raw(HW_IN_EMERGENCIA) == false;
}

/* Saídas (LEDs) - ativo em alto por simplicidade */
void atuador_comando_abrir(bool ativo)
{
    hardware_write_raw(HW_OUT_CMD_ABRIR, ativo);
}

void atuador_comando_fechar(bool ativo)
{
    hardware_write_raw(HW_OUT_CMD_FECHAR, ativo);
}

void atuador_sinal_verde(bool ativo)
{
    hardware_write_raw(HW_OUT_SINAL_VERDE, ativo);
}

void atuador_sinal_vermelho(bool ativo)
{
    hardware_write_raw(HW_OUT_SINAL_VERMELHO, ativo);
}
