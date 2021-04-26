#pragma once

#include "ledstrip_hardware.h"

/* USAGE
* 1. Init hardware and software using ledstrip_init().
* 2. Modify memory map using set and reset functions.
* 3. Colorize pixels, according your memory map, using ledstrip_print().
*/

typedef struct
{
    ledstrip_hardware_t hw;
    const uint32_t total_pixels;
    const uint8_t bits_per_pixel;
    void *buffer;
} ledstrip_t;

/**
* @brief Initialize hardware configuration and software structures
* @param ledstrip: pointer to structure
* @return void
*/
void ledstrip_init(ledstrip_t *ledstrip);

/**
* @brief Set color,on memory map (not yet printed), for desired pixel.
* @param ledstrip: pointer to structure 
* @param pixel_pos: position of pixel on the strip. From 1 to total_pixels.
* @param color: color to write in desired pixel. Format:
            0xAB  FF  1C   00
           green red blue unused
* @return void
*/
void ledstrip_setPixel(ledstrip_t *ledstrip, uint8_t pixel_pos, uint32_t color);

/**
* @brief Set color 0x00000000 ,on memory map (not yet printed), for desired pixel.
* @param ledstrip: pointer to structure 
* @param pixel_pos: position of pixel on the strip. From 1 to total_pixels.
* @return void
*/
void ledstrip_resetPixel(ledstrip_t *ledstrip, uint8_t pixel_pos);

/**
* @brief Set color 0x00000000 ,on memory map (not yet printed), for 
*       all pixels on the strip.
* @param ledstrip: pointer to structure 
* @return void
*/
void ledstrip_resetAllPixel(ledstrip_t *ledstrip);

/**
* @brief Print colors, according to the memory map, in all pixels on the strip.
* @param ledstrip: pointer to structure 
* @return void
*/
void ledstrip_print(ledstrip_t *ledstrip);

