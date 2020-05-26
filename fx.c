#include <stddef.h>

#include "fx.h"
#include "fire.h"

void fx_calc_fire(rgb_map_t *rgb_map, data_map_t *data_map)
{
    if(!rgb_map) return;
    if(!data_map) return;

    fire_heat_map_t *heat_map = (fire_heat_map_t *)data_map;

    fire_heat_map_update(heat_map);
    fire_rgb_map_update(rgb_map, heat_map);
}
