#pragma once

#include <stdint.h>

#include "rgb.h"
#include "palette.h"

typedef struct
{
    map_size_t stride; // 1
    map_size_t width; // 1
    map_size_t height; // 1
} map_header_t; // 3

typedef struct
{
    map_header_t header; // 3
    rgb_t *rgb; // 2
    uint8_t brightness; // 1
    rgb_t color_correction; // 3
    rgb_t temp_correction; // 3
    palette_id_t palette_id; // 1
} rgb_map_t; // 13

STATIC_ASSERT(sizeof(rgb_map_t) == 13);

typedef struct
{
    map_header_t header; // 3
    void *data; // 2
    void *param; // 2
} data_map_t; // 7

STATIC_ASSERT(sizeof(data_map_t) == 7);

#define FX_NONE UINT8_C(0)
#define FX_STATIC UINT8_C(1)
#define FX_FIRE UINT8_C(2)
#define FX_TORCH UINT8_C(3)

/*
 * VERTICAL (BOTTOM->TOP) physical LED layout
 *  ----------------------------------------
 *  | 0   | H    |...| nH     |...| W(H-1) |
 *  | 1   | H+1  |   | nH+1   |   .        .
 *  .     .      .   .        .   .        .
 *  | H-1 | 2H-1 |...| nH+H-1 |...| WH-1   |
 *  ---------------------------------------- */

#define MAP_XY_VERTICAL_BOTTOM_TOP(data, stride, x, y) \
    (*((data) + (x) * (stride) + (y)))

/* HORIZONTAL (TOP->BOTTOM) physical LED layout
 *  -----------------------------------
 *  | 0      | 1        |...| W-1    |
 *  | W      | W+1      |...| 2W-1   |
 *  .        .          .....        .
 *  | nW     | nW+1     |...| nW+W-1 |
 *  .        .          .....        .
 *  | (H-1)W | (H-1)W+1 |...| HW-1   |
 *  ----------------------------------- */

#define MAP_XY_HORIZONTAL_TOP_BOTTOM(data, stride, x, y) \
    (*((data) + (y) * (stride) + (x)))

//#define MAP_XY MAP_XY_VERTICAL_BOTTOM_TOP
#define MAP_XY MAP_XY_HORIZONTAL_TOP_BOTTOM

void fx_calc_fire(rgb_map_t *, data_map_t *);
void fx_init_torch(data_map_t *);
void fx_calc_torch(rgb_map_t *, data_map_t *);
