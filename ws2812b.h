#pragma once

#include "fx.h"

typedef union
{
    struct
    {
        uint8_t update : 1;
        uint8_t updated : 1;
        uint8_t abort : 1;
        uint8_t aborted : 1;
        uint8_t fx : 4;
    };

    uint8_t value;
} ws2812b_flags_t;

typedef struct
{
    rgb_map_t rgb_map;
    heat_map_t heat_map;
    uint16_t idx;
    uint16_t size;
    ws2812b_flags_t flags;
} ws2812b_strip_t;

void ws2812b_init(ws2812b_strip_t *);
void ws2812b_update(ws2812b_strip_t *);
void ws2812b_clear(ws2812b_strip_t *);
