/**
 * @file app.h
 * @brief Camada de APLICAÇÃO - máquina de estados da cancela.
 *
 * Não inclui hardware.h nem driver/gpio.h em nenhuma hipótese.
 * Conhece apenas os nomes de domínio expostos por services.h.
 */
#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ESTADO_FECHADA = 0,
    ESTADO_ABRINDO,
    ESTADO_ABERTA,
    ESTADO_FECHANDO,
    ESTADO_EMERGENCIA
} estado_cancela_t;

/** Inicializa a aplicação (e, por consequência, os serviços/hardware). */
void app_init(void);

/** Deve ser chamada periodicamente (ex.: a cada 50-100 ms) no loop principal. */
void app_run_once(void);

/** Retorna o estado atual (útil para logs/testes). */
estado_cancela_t app_get_estado(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_H */
