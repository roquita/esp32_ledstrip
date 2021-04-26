#pragma once

#include "stdint.h"

#include "esp_log.h"
#include "esp_err.h"
#include "driver/rmt.h"

typedef struct
{
#if (ESP_PLATFORM == 1)
    rmt_channel_t channel;
    uint8_t clk_div;
    int gpio_num;
    rmt_tx_config_t tx_config;
#endif
    union
    {
        struct
        {
            uint32_t t0h_ticks;
            uint32_t t0l_ticks;
            uint32_t t1h_ticks;
            uint32_t t1l_ticks;
            uint32_t treset_ticks;
        };
        struct
        {
            uint32_t t0h_ns;
            uint32_t t0l_ns;
            uint32_t t1h_ns;
            uint32_t t1l_ns;
            uint32_t treset_us;
        };
    };
} ledstrip_hardware_t;

void timings_ns_to_ticks(ledstrip_hardware_t *hw);
void hw_ledstrip_init(ledstrip_hardware_t *hw);
void hw_ledstrip_set_pixel(ledstrip_hardware_t *hw, void *pixel_bufer, uint32_t color);
esp_err_t hw_ledstrip_print(ledstrip_hardware_t *hw, void *buffer, int len);