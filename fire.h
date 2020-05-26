#pragma once

#include "fx.h"

typedef uint8_t fire_heat_t; // 1

typedef struct
{
    map_header_t header; // 3
    fire_heat_t *data; // 2
} fire_heat_map_t; // 5

void fire_heat_map_update(fire_heat_map_t *map);
void fire_rgb_map_update(rgb_map_t * rgb_map, const fire_heat_map_t *heat_map);
