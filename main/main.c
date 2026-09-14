/**
 * @file main.c
 * @brief Ponto de entrada. Apenas inicializa a aplicação e chama
 *        app_run_once() periodicamente - não conhece GPIOs nem sensores.
 */
#include "app.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    app_init();

    const TickType_t periodo = pdMS_TO_TICKS(100); /* 100 ms */

    while (1) {
        app_run_once();
        vTaskDelay(periodo);
    }
}
