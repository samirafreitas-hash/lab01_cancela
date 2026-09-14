/**
 * @file services.h
 * @brief Camada de SERVIÇOS (sensores e atuadores simulados).
 *
 * Traduz níveis elétricos brutos (camada de hardware) em conceitos de
 * domínio ("veículo detectado", "cancela aberta" etc.), escondendo da
 * camada de aplicação a existência de GPIOs, pull-up/pull-down ou lógica
 * ativa em alto/baixo.
 *
 * A camada de aplicação SÓ pode incluir este header (nunca hardware.h).
 */
#ifndef SERVICES_H
#define SERVICES_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Inicializa a camada de serviços (por sua vez inicializa o hardware). */
void services_init(void);

/* ---------------------- Sensores (entradas) ---------------------- */
bool sensor_veiculo_aguardando_entrada(void);  /* ativo em nível ALTO (pull-down) */
bool sensor_veiculo_na_passagem(void);         /* ativo em nível ALTO (pull-down) */
bool sensor_cancela_aberta(void);              /* ativo em nível BAIXO (pull-up)  */
bool sensor_cancela_fechada(void);             /* ativo em nível BAIXO (pull-up)  */
bool sensor_emergencia_acionada(void);         /* ativo em nível BAIXO (pull-up)  */

/* ---------------------- Atuadores (saídas) ------------------------ */
void atuador_comando_abrir(bool ativo);
void atuador_comando_fechar(bool ativo);
void atuador_sinal_verde(bool ativo);
void atuador_sinal_vermelho(bool ativo);

#ifdef __cplusplus
}
#endif

#endif /* SERVICES_H */
