#pragma once

#include <stddef.h>

#include "fx.h"

#define MODE_NONE UINT8_C(0)
#define MODE_PASSIVE UINT8_C(1)
#define MODE_SPARK UINT8_C(2)
#define MODE_TEMP UINT8_C(3)

typedef struct
{
    struct
    {
        uint8_t mode : 2;
        uint8_t reserved : 6;
    };

    uint8_t value;
} torch_energy_t; // 2

typedef struct
{
    map_header_t header; // 3
    torch_energy_t *data; // 2
} torch_energy_map_t; // 5

uint8_t torch_energy_quant(uint8_t);
void torch_energy_map_update(torch_energy_map_t *);
void torch_rgb_map_update(rgb_map_t * rgb_map, const torch_energy_map_t *);
