#include "ledstrip_hardware.h"

#define TAG "LEDSTRIP_HW"

void timings_ns_to_ticks(ledstrip_hardware_t *hw)
{
    uint32_t counter_clk_hz = 0;

    if (rmt_get_counter_clock(hw->channel, &counter_clk_hz) == ESP_OK)
    {
        // ns -> ticks
        float ratio = (float)counter_clk_hz / 1e9;
        hw->t0h_ticks = (uint32_t)(ratio * (hw->t0h_ns));
        hw->t0l_ticks = (uint32_t)(ratio * (hw->t0l_ns));
        hw->t1h_ticks = (uint32_t)(ratio * (hw->t1h_ns));
        hw->t1l_ticks = (uint32_t)(ratio * (hw->t1l_ns));
        hw->treset_ticks = (uint32_t)(ratio * (hw->treset_us) * 1000.0);

        ESP_LOGI(TAG, "t0h:%u  t0l:%u  t1h:%u  t1l:%u  trst:%u", hw->t0h_ticks, hw->t0l_ticks,
                 hw->t1h_ticks, hw->t1l_ticks, hw->treset_ticks);
    }
}

void hw_ledstrip_init(ledstrip_hardware_t *hw)
{
    rmt_config_t config = {
        .rmt_mode = RMT_MODE_TX,
        .channel = hw->channel,
        .gpio_num = hw->gpio_num,
        .clk_div = hw->clk_div,
        .mem_block_num = 1,
        .flags = 0,
        .tx_config = {
            .carrier_en = false,
            .loop_en = false,
            .idle_output_en = true,
            .idle_level = RMT_IDLE_LEVEL_LOW,
        },
    };

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    timings_ns_to_ticks(hw);
}

void hw_ledstrip_set_pixel(ledstrip_hardware_t *hw, void *pixel_bufer, uint32_t color)
{
    rmt_item32_t one_bit = {{{hw->t1h_ticks, 1, hw->t1l_ticks, 0}}};
    rmt_item32_t zero_bit = {{{hw->t0h_ticks, 1, hw->t0l_ticks, 0}}};

    uint8_t green = color >> 24;
    uint8_t red = color >> 16;
    uint8_t blue = color >> 8;

    rmt_item32_t *item = (rmt_item32_t *)pixel_bufer;

    // BITS FOR GREEN
    for (int i = 0; i < 8; i++)
    {
        if (green & (1 << (7 - i)))
        {
            item->val = one_bit.val;
        }
        else
        {
            item->val = zero_bit.val;
        }
        item++;
    }

    // BITS FOR RED
    for (int i = 0; i < 8; i++)
    {
        if (red & (1 << (7 - i)))
        {
            item->val = one_bit.val;
        }
        else
        {
            item->val = zero_bit.val;
        }
        item++;
    }

    // BITS FOR BLUE
    for (int i = 0; i < 8; i++)
    {
        if (blue & (1 << (7 - i)))
        {
            item->val = one_bit.val;
        }
        else
        {
            item->val = zero_bit.val;
        }
        item++;
    }
}

esp_err_t hw_ledstrip_print(ledstrip_hardware_t *hw, void *buffer, int len)
{
    return rmt_write_items(hw->channel, (rmt_item32_t *)buffer, len, true);
}