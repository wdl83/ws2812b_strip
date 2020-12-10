#pragma once

#include "energy.h"
#include "fx.h"

typedef struct
{
    uint8_t speed_step;
    uint8_t scale;
    uint16_t rx;
    uint16_t ry;
    uint16_t speed;
} noise_param_t;

typedef struct
{
    map_header_t header; // 3
    energy_t *data; // 2
    noise_param_t *param; // 2
} noise_energy_map_t; // 7

void noise_energy_map_update(noise_energy_map_t *);
void noise_rgb_map_update(rgb_map_t *, const noise_energy_map_t *);
void noise_init(noise_energy_map_t *);
