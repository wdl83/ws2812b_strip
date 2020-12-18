#include <stddef.h>

#include "fire.h"
#include "fx.h"
#include "noise.h"
#include "torch.h"

void fx_calc_fire(rgb_map_t *rgb_map, void *raw)
{
    fire_map_t *fire_map = (fire_map_t *)raw;

    fire_map_update(fire_map);
    fire_rgb_map_update(rgb_map, fire_map);
}

void fx_init_torch(void *raw)
{
    torch_map_t *map = (torch_map_t *)raw;

    torch_init(map);
}

void fx_calc_torch(rgb_map_t *rgb_map, void *raw)
{
    torch_map_t *torch_map = (torch_map_t *)raw;

    torch_map_update(torch_map);
    torch_rgb_map_update(rgb_map, torch_map);
}

void fx_init_noise(void *raw)
{
    noise_map_t *map = (noise_map_t *)raw;

    noise_init(map);
}

void fx_calc_noise(rgb_map_t *rgb_map, void *raw)
{
    noise_map_t *noise_map = (noise_map_t *)raw;

    noise_map_update(noise_map);
    noise_rgb_map_update(rgb_map, noise_map);
}
