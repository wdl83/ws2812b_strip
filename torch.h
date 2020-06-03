#pragma once

#include <stddef.h>

#include "fx.h"

#define TORCH_MODE_NONE UINT8_C(0)
#define TORCH_MODE_PASSIVE UINT8_C(1)
#define TORCH_MODE_SPARK UINT8_C(2)
#define TORCH_MODE_TEMP UINT8_C(3)

typedef uint8_t torch_energy_t;

typedef struct
{
    uint8_t spark_threshold;
    /* Left/Right energy transfer coefficient */
    uint8_t adjH;
    /* Top/Bottom energy transfer coefficient */
    uint8_t adjV;
    /* how much energy is preserved between cycles */
    uint8_t passive_preserve;
    uint8_t spark_transfer;
    uint8_t spark_preserve;
    rgb_t color_coeff;
    /* each element mode is encoded on 2 bits,
     * so length is dependent on number of elements */
    uint8_t mode[0];
} torch_param_t; // 9

/* (y * stride + x) / 4 */
#define TORCH_MODE_BYTE_OFFSET(stride, x, y) (((y) * (stride) + (x)) >> 2)
/* (y * stride + x) % 4 */
#define TORCH_MODE_BIT_OFFSET(stride, x, y) ((((y) * (stride) + (x)) & UINT8_C(0x3)) << 1)

#define TORCH_MODE_GET(data, stride, x, y) \
    (((data)[TORCH_MODE_BYTE_OFFSET(stride, x, y)] >> TORCH_MODE_BIT_OFFSET(stride, x, y)) & UINT8_C(0x3))

#define TORCH_MODE_SET(data, stride, x, y, mode) \
    (data)[TORCH_MODE_BYTE_OFFSET((stride), (x), (y))] = \
    ((data)[TORCH_MODE_BYTE_OFFSET((stride), (x), (y))] \
     & ((~(UINT8_C(0x3) << TORCH_MODE_BIT_OFFSET((stride), (x), (y)))))) \
     | (((mode) & UINT8_C(0x3)) << TORCH_MODE_BIT_OFFSET((stride), (x), (y)))

typedef struct
{
    map_header_t header; // 3
    torch_energy_t *data; // 2
    torch_param_t *param; // 2
} torch_energy_map_t; // 7

uint8_t torch_energy_quant(uint8_t);
void torch_energy_map_update(torch_energy_map_t *);
void torch_rgb_map_update(rgb_map_t * rgb_map, const torch_energy_map_t *);
void torch_init(torch_energy_map_t *);
