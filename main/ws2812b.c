
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ledstrip.h"

#define TAG "example"


void app_main(void *arg)
{
    ESP_LOGI(TAG, "Configuring transmitter");

    ledstrip_t ledstrip = {
        .hw = {
            .channel = 0,
            .clk_div = 4,
            .gpio_num = 18,
            .tx_config = {
                .carrier_en = false,
                .loop_en = false,
                .idle_output_en = true,
                .idle_level = RMT_IDLE_LEVEL_LOW,
            },
            .t0h_ns = 400,
            .t0l_ns = 850,
            .t1h_ns = 800,
            .t1l_ns = 450,
            .treset_us = 55,
        },
        .total_pixels = 30,
        .bits_per_pixel = 24,
    };

    ledstrip_init(&ledstrip);

    while (1)
    {
        uint32_t color = 0xFFff0000; //rgba
        int len = ledstrip.total_pixels;
        for (int i = 1; i <= len; i++)
        {
            ledstrip_resetPixel(&ledstrip, i - 1);
            ledstrip_setPixel(&ledstrip, i, color);
            ledstrip_print(&ledstrip);
            vTaskDelay(pdMS_TO_TICKS(25));
        }
        for (int i = len - 1; i > 0; i--)
        {
            ledstrip_resetPixel(&ledstrip, i + 1);
            ledstrip_setPixel(&ledstrip, i, color);
            ledstrip_print(&ledstrip);
            vTaskDelay(pdMS_TO_TICKS(25));
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
