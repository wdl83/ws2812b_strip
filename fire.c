#include <stddef.h>

#include "fire.h"
#include "palette.h"

#define MIN(x, y) (y < x ? y : x)
#define MAX(x, y) (y > x ? y : x)

static
uint16_t rand8seed_ = 1337;

static inline
uint8_t rand8(void)
{
    rand8seed_ = rand8seed_ * ((uint16_t)2053) + ((uint16_t)13849);
    return ((uint8_t)(rand8seed_ & (uint16_t)0xFF)) + ((uint8_t)(rand8seed_ >> 8));
}

static inline
uint8_t rand8_bdry(uint8_t max)
{
    uint16_t r = rand8();
    return (uint8_t)((r * ((uint16_t)max)) >> 8);
}

typedef fire_heat_map_t heat_map_t;

static
rgb_t heat2color(uint8_t temp)
{
    rgb_t rgb;

    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = (uint8_t)(((uint16_t)temp * (uint16_t)192) >> 8) + (uint8_t)(!!temp);

    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80)
    {
        // we're in the hottest third
        rgb.R = 255; // full red
        rgb.G = 255; // full green
        rgb.B = heatramp; // ramp up blue

    }
    else if(t192 & 0x40)
    {
        // we're in the middle third
        rgb.R = 255; // full red
        rgb.G = heatramp; // ramp up green
        rgb.B = 0; // no blue

    } else
    {
        // we're in the coolest third
        rgb.R  = heatramp; // ramp up red
        rgb.G  = 0; // no green
        rgb.B  = 0; // no blue
    }

    return rgb;
}

void fire_heat_map_update(heat_map_t *map)
{
    if(NULL == map) return;
    if(NULL == map->data) return;

    const map_size_t stride = map->header.stride;
    const map_size_t h = map->header.height;
    const map_size_t w = map->header.width;

    const uint8_t cooling_offset = MIN(16, rand8_bdry(128));
    /* 1. random cooling */
    for(map_size_t y = 0; y < h; ++y)
    {
        for(map_size_t x = 0; x < w; ++x)
        {
            uint8_t value = MAP_XY(map->data, stride, x, y);
            /* min(16+y, 0.75 * value) */
            uint8_t delta = rand8_bdry(
                MIN(cooling_offset + y<<3, (value >> 2) + (value >> 1)));
            MAP_XY(map->data, stride, x, y) -= delta;
        }
    }
    /* 2. heat transfer 'up'
     * | aL       | a        | aR       |
     * | bL (0.25)| b (0.50) | bR (0.25)|
     *
     * a = 0.5 * b + 0.25 * c */
    for(map_size_t x = 0; x < w; ++x)
    {
        for(map_size_t y = 1; y < h; ++y)
        {
            uint8_t b = MAP_XY(map->data, stride, x, y - 1);
            uint8_t bL = MAP_XY(map->data, stride, 0 == x ? w - 1 : x - 1, y - 1);
            uint8_t bR = MAP_XY(map->data, stride, w - 1 == x ? 0 : x + 1, y - 1);

            MAP_XY(map->data, stride, x, y) = (b >> 1) + (bL >> 2) + (bR >> 2);
        }
    }
    /* 3. random new heat srouces at the 'bottom' */
    for(map_size_t y = 0; y < 1; ++y)
    {
        map_size_t n = rand8_bdry(w >> 1);

        while(0 != n)
        {
            map_size_t x = rand8_bdry(w);
            uint8_t delta = rand8_bdry(240 - MAP_XY(map->data, stride, x, y));

            MAP_XY(map->data, stride, x, y) += delta;
            --n;
        }
    }
}

void fire_rgb_map_update(rgb_map_t * rgb_map, const heat_map_t *heat_map)
{
    const map_size_t len = rgb_map->header.width * rgb_map->header.height;

    if(PALETTE_ID_INVALID == rgb_map->palette_id.value)
    {
        const uint8_t coeff_R =
            COEFF8(
                rgb_map->brightness,
                rgb_map->color_correction.R,
                rgb_map->temp_correction.R);
        const uint8_t coeff_G =
            COEFF8(
                rgb_map->brightness,
                rgb_map->color_correction.G,
                rgb_map->temp_correction.G);
        const uint8_t coeff_B =
            COEFF8(
                rgb_map->brightness,
                rgb_map->color_correction.B,
                rgb_map->temp_correction.B);


        for(map_size_t i = 0; i < len; ++i)
        {
            rgb_map->rgb[i] = heat2color(heat_map->data[i]);
            rgb_map->rgb[i].R = SCALE8(rgb_map->rgb[i].R, coeff_R);
            rgb_map->rgb[i].G = SCALE8(rgb_map->rgb[i].G, coeff_G);
            rgb_map->rgb[i].B = SCALE8(rgb_map->rgb[i].B, coeff_B);
        }
    }
    else
    {
        for(map_size_t i = 0; i < len; ++i)
        {
            rgb_map->rgb[i] =
                palette_color(
                    rgb_map->palette_id,
                    SCALE8(heat_map->data[i], 240),
                    rgb_map->brightness);
        }
    }
}
