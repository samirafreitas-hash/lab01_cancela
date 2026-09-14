/**
 * @file app.c
 * @brief Implementação da máquina de estados da cancela automática.
 */
#include "app.h"
#include "services.h"
#include "esp_log.h"

static const char *TAG = "app";

static estado_cancela_t s_estado = ESTADO_FECHADA;
static estado_cancela_t s_estado_anterior_emergencia = ESTADO_FECHADA;
static uint32_t s_blink_tick = 0;

/* Garante, em um único lugar, que abrir/fechar nunca fiquem ativos
 * ao mesmo tempo — chamado sempre que os comandos de movimento mudam. */
static void set_comando_movimento(bool abrir, bool fechar)
{
    if (abrir && fechar) {
        /* condição inválida: nunca deve ocorrer, mas se ocorrer,
         * prevalece a segurança -> nenhum dos dois é ativado. */
        abrir = false;
        fechar = false;
    }
    atuador_comando_abrir(abrir);
    atuador_comando_fechar(fechar);
}

static void set_sinalizacao(bool verde, bool vermelho)
{
    atuador_sinal_verde(verde);
    atuador_sinal_vermelho(vermelho);
}

static void entrar_estado(estado_cancela_t novo)
{
    if (novo != ESTADO_EMERGENCIA) {
        s_estado_anterior_emergencia = novo;
    }
    s_estado = novo;

    switch (novo) {
        case ESTADO_FECHADA:
            set_comando_movimento(false, false);
            set_sinalizacao(false, true);
            ESP_LOGI(TAG, "-> FECHADA");
            break;
        case ESTADO_ABRINDO:
            set_comando_movimento(true, false);
            set_sinalizacao(false, true);
            ESP_LOGI(TAG, "-> ABRINDO");
            break;
        case ESTADO_ABERTA:
            set_comando_movimento(false, false);
            set_sinalizacao(true, false);
            ESP_LOGI(TAG, "-> ABERTA");
            break;
        case ESTADO_FECHANDO:
            set_comando_movimento(false, true);
            set_sinalizacao(false, true);
            ESP_LOGI(TAG, "-> FECHANDO");
            break;
        case ESTADO_EMERGENCIA:
            set_comando_movimento(false, false);
            /* sinalização em EMERGENCIA pisca o vermelho, ver app_run_once */
            ESP_LOGI(TAG, "-> EMERGENCIA");
            break;
    }
}

void app_init(void)
{
    services_init();
    entrar_estado(ESTADO_FECHADA);
}

estado_cancela_t app_get_estado(void)
{
    return s_estado;
}

void app_run_once(void)
{
    /* Requisito 7: emergência interrompe qualquer movimento,
     * tem prioridade sobre todas as outras transições. */
    if (sensor_emergencia_acionada()) {
        if (s_estado != ESTADO_EMERGENCIA) {
            entrar_estado(ESTADO_EMERGENCIA);
        }
        /* Sinal vermelho piscando enquanto em emergência. */
        s_blink_tick++;
        bool aceso = (s_blink_tick % 6) < 3; /* pisca a cada ~N ciclos de chamada */
        set_sinalizacao(false, aceso);
        return;
    }

    /* Emergência acabou de ser liberada: decide para onde retomar
     * com base no estado dos fins de curso (posição real da cancela). */
    if (s_estado == ESTADO_EMERGENCIA) {
        if (sensor_cancela_fechada()) {
            entrar_estado(ESTADO_FECHADA);
        } else if (sensor_cancela_aberta()) {
            entrar_estado(ESTADO_ABERTA);
        } else {
            /* posição intermediária: reabre por segurança */
            entrar_estado(ESTADO_ABRINDO);
        }
        return;
    }

    switch (s_estado) {

        case ESTADO_FECHADA:
            /* 1. inicia abertura quando um veículo chegar */
            if (sensor_veiculo_aguardando_entrada()) {
                entrar_estado(ESTADO_ABRINDO);
            }
            break;

        case ESTADO_ABRINDO:
            /* 2. mantém comando de abertura até fim de curso aberto */
            if (sensor_cancela_aberta()) {
                entrar_estado(ESTADO_ABERTA);
            }
            break;

        case ESTADO_ABERTA:
            /* 3. passagem liberada (sinal verde já setado em entrar_estado)
             * 4. inicia fechamento após detectar passagem do veículo */
            if (sensor_veiculo_na_passagem()) {
                entrar_estado(ESTADO_FECHANDO);
            }
            break;

        case ESTADO_FECHANDO:
            /* 6. volta a abrir caso um veículo seja detectado durante o fechamento */
            if (sensor_veiculo_aguardando_entrada() || sensor_veiculo_na_passagem()) {
                entrar_estado(ESTADO_ABRINDO);
                break;
            }
            /* 5. finaliza o fechamento quando o fim de curso fechado for acionado */
            if (sensor_cancela_fechada()) {
                entrar_estado(ESTADO_FECHADA);
            }
            break;

        case ESTADO_EMERGENCIA:
            /* tratado acima */
            break;
    }
}
