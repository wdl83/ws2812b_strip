#pragma once

#include <stdint.h>

#include "rgb.h"
#include "palette.h"

STATIC_ASSERT(sizeof(map_size_t) == 1);

typedef struct
{
    map_size_t stride;
    map_size_t width;
    map_size_t height;
} map_header_t;

STATIC_ASSERT(sizeof(map_header_t) == 3);

typedef struct
{
    uint8_t brightness; // 0
    rgb_t color_correction; // 1
    rgb_t temp_correction; // 4
    palette16_id_t palette16_id; // 7
    map_header_t header; // 8
    rgb_t rgb[0]; // 11
} rgb_map_t;

STATIC_ASSERT(sizeof(rgb_map_t) == 11);

#define FX_NONE                                                      UINT8_C(0x0)
#define FX_STATIC                                                    UINT8_C(0x1)
#define FX_FIRE                                                      UINT8_C(0x2)
#define FX_TORCH                                                     UINT8_C(0x3)
#define FX_NOISE                                                     UINT8_C(0x4)

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

void fx_calc_fire(rgb_map_t *, void *);
void fx_init_torch(void *);
void fx_calc_torch(rgb_map_t *, void *);
void fx_init_noise(void *);
void fx_calc_noise(rgb_map_t *, void *);
