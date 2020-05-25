#pragma once

#include <stdint.h>

#include "rgb.h"

typedef struct
{
    map_size_t stride;
    map_size_t width;
    map_size_t height;
    map_size_t size;
} map_header_t;

typedef struct
{
    map_header_t header;
    void *data;
} data_map_t;

typedef struct
{
    map_size_t stride;
    map_size_t width;
    map_size_t height;
    map_size_t size;
    uint8_t brightness;
    rgb_t color_correction;
    rgb_t temp_correction;
    rgb_t *led;
} rgb_map_t;

typedef struct
{
    map_size_t stride;
    map_size_t width;
    map_size_t height;
    map_size_t size;
    uint8_t *data;
} heat_map_t;

#define FX_NONE UINT8_C(0)
#define FX_STATIC UINT8_C(1)
#define FX_FIRE UINT8_C(2)

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

void fx_calc_fire(rgb_map_t *, heat_map_t *);
