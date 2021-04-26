#include <stdio.h>
#include "ledstrip.h"

#define TAG "LEDSTRIP"

void ledstrip_init(ledstrip_t *ledstrip)
{
    hw_ledstrip_init(&(ledstrip->hw));
    ledstrip->buffer = calloc(ledstrip->bits_per_pixel * ledstrip->total_pixels, sizeof(uint32_t));
    ledstrip_resetAllPixel(ledstrip);
}
void ledstrip_setPixel(ledstrip_t *ledstrip, uint8_t pixel_pos, uint32_t color)
{
    if (ledstrip->total_pixels >= pixel_pos && pixel_pos > 0)
    {
        void *pixel_bufer = ledstrip->buffer;
        pixel_bufer += ((pixel_pos - 1) * ledstrip->bits_per_pixel) * sizeof(uint32_t);
        hw_ledstrip_set_pixel(&(ledstrip->hw), pixel_bufer, color);
    }
}
void ledstrip_resetPixel(ledstrip_t *ledstrip, uint8_t pixel_pos)
{
    ledstrip_setPixel(ledstrip, pixel_pos, 0x00000000);
}
void ledstrip_resetAllPixel(ledstrip_t *ledstrip)
{
    int len = ledstrip->total_pixels;
    for (int i = 1; i <= len; i++)
    {
        ledstrip_resetPixel(ledstrip, i);
    }
}
void ledstrip_print(ledstrip_t *ledstrip)
{
    int len = ledstrip->bits_per_pixel * ledstrip->total_pixels;
    hw_ledstrip_print(&(ledstrip->hw), ledstrip->buffer, len);
}
