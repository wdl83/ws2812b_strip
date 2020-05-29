#pragma once

#include <stdint.h>

#include <drv/assert.h>

#include "fire.h"
#include "fx.h"
#include "torch.h"

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
} ws2812b_flags_t; // 1

typedef union
{
    data_map_t data_map; // 7
    fire_heat_map_t fire_heat_map;  // 7
    torch_energy_map_t torch_energy_map; //  7
} fx_data_map_t; // 7

STATIC_ASSERT(sizeof(fx_data_map_t) == 7);

typedef struct
{
    rgb_map_t rgb_map; // 12
    uint16_t rgb_idx; // 2
    uint16_t rgb_size; // 2
    fx_data_map_t fx_data_map; // 7
    ws2812b_flags_t flags; // 1
} ws2812b_strip_t; // 24

STATIC_ASSERT(sizeof(ws2812b_strip_t) == 24);

void ws2812b_init(ws2812b_strip_t *);
void ws2812b_update(ws2812b_strip_t *);
void ws2812b_clear(ws2812b_strip_t *);
void ws2812b_apply_correction(ws2812b_strip_t *);
