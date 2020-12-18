#pragma once

#include "energy.h"
#include "fx.h"

typedef struct
{
    map_header_t header;
    uint8_t speed_step; // 3
    uint8_t scale; // 4
    uint16_t rx; // 5
    uint16_t ry; // 7
    uint16_t speed; // 9
    energy_t energy[0]; // 11
} noise_map_t;

STATIC_ASSERT_STRUCT_OFFSET(noise_map_t, header, 0);

#define NOISE_MAP_SIZE(strip_len) \
    ( \
      sizeof(noise_map_t) \
      + (strip_len))

void noise_map_update(noise_map_t *);
void noise_rgb_map_update(rgb_map_t *, const noise_map_t *);
void noise_init(noise_map_t *);
