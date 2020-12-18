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
} ws2812b_flags_t;

STATIC_ASSERT(sizeof(ws2812b_flags_t) == 1);

typedef struct
{
    ws2812b_flags_t flags; // 0
    uint16_t rgb_idx; // 1
    uint16_t rgb_size; // 3
    rgb_map_t rgb_map; // 5
} ws2812b_strip_t;

STATIC_ASSERT(sizeof(ws2812b_strip_t) == 5 + sizeof(rgb_map_t));

#define WS2812B_STRIP_SIZE(strip_len) \
    ( \
      sizeof(ws2812b_strip_t) \
     + (strip_len) * 3)

void ws2812b_init(ws2812b_strip_t *);
void ws2812b_update(ws2812b_strip_t *);
void ws2812b_clear(ws2812b_strip_t *);
void ws2812b_apply_correction(ws2812b_strip_t *);
void ws2812b_power_on(ws2812b_strip_t *);
void ws2812b_power_off(ws2812b_strip_t *);
