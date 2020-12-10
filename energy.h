#pragma once

#include "rgb.h"

typedef struct
{
    rgb_t color_base;
    rgb_t color_coeff;
} energy_param_t;

typedef uint8_t energy_t;

rgb_t energy2color(const energy_param_t *const, energy_t energy);
