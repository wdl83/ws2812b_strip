#pragma once

#include "fx.h"
#include "energy.h"

typedef struct
{
    map_header_t header;
    energy_t energy[0];
} fire_map_t; // 3

STATIC_ASSERT_STRUCT_OFFSET(fire_map_t, header, 0);

#define FIRE_MAP_SIZE(strip_len) \
    ( \
      sizeof(fire_map_t) \
     + (strip_len))

void fire_map_update(fire_map_t *map);
void fire_rgb_map_update(rgb_map_t * rgb_map, const fire_map_t *heat_map);
