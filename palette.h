#pragma once

#include <stdint.h>

#include "rgb.h"

#define PALETTE_ID_INVALID 0xFF

typedef struct
{
    uint8_t value;
} palette_id_t;

rgb_t palette_color(palette_id_t id, uint8_t index, uint8_t brightness);
