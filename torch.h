#pragma once

#include <stddef.h>

#include "fx.h"
#include "energy.h"

#define TORCH_MODE_NONE UINT8_C(0)
#define TORCH_MODE_PASSIVE UINT8_C(1)
#define TORCH_MODE_SPARK UINT8_C(2)
#define TORCH_MODE_TEMP UINT8_C(3)

typedef struct
{
    map_header_t header;
    uint8_t spark_threshold; // 3
    /* Left/Right energy transfer coefficient */
    uint8_t adjH; // 4
    /* Top/Bottom energy transfer coefficient */
    uint8_t adjV; // 5
    /* how much energy is preserved between cycles */
    uint8_t passive_preserve; // 6
    uint8_t spark_transfer; // 7
    uint8_t spark_preserve; // 8
    rgb_t color_coeff; // 9
    uint8_t raw[0]; // 12
} torch_map_t;

STATIC_ASSERT_STRUCT_OFFSET(torch_map_t, header, 0);

/* each element mode is encoded on 2 bits,
 * so length is dependent on number of elements */
#define TORCH_MAP_SIZE(strip_len) \
    ( \
     sizeof(torch_map_t) \
     + (strip_len) /* energy */ \
     + ((strip_len) >> 2) /* mode */ )

/* (y * stride + x) / 4 */
#define TORCH_MODE_BYTE_OFFSET(stride, x, y) \
    (((y) * (stride) + (x)) >> 2)

/* (y * stride + x) % 4 */
#define TORCH_MODE_BIT_OFFSET(stride, x, y) \
    ((((y) * (stride) + (x)) & UINT8_C(0x3)) << 1)

#define TORCH_MODE_GET(data, stride, x, y) \
    ( \
     ( \
      (data)[TORCH_MODE_BYTE_OFFSET(stride, x, y)] \
      >> TORCH_MODE_BIT_OFFSET(stride, x, y) \
     ) \
     & UINT8_C(0x3))

#define TORCH_MODE_SET(data, stride, x, y, mode) \
    (data)[TORCH_MODE_BYTE_OFFSET((stride), (x), (y))] = \
    ((data)[TORCH_MODE_BYTE_OFFSET((stride), (x), (y))] \
     & ((~(UINT8_C(0x3) << TORCH_MODE_BIT_OFFSET((stride), (x), (y)))))) \
     | (((mode) & UINT8_C(0x3)) << TORCH_MODE_BIT_OFFSET((stride), (x), (y)))

void torch_map_update(torch_map_t *);
void torch_rgb_map_update(rgb_map_t * rgb_map, const torch_map_t *);
void torch_init(torch_map_t *);
