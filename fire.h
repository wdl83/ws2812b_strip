#pragma once

#include "fx.h"

void fire_heat_map_update(heat_map_t *map);
void fire_rgb_map_update(rgb_map_t * rgb_map, const heat_map_t *heat_map);
