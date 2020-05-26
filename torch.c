#include <stddef.h>

#if defined HOST
#define PROGMEM
#define pgm_read_byte(addr) (*(addr))
#else /* HOST */
#include <avr/pgmspace.h>
#endif /* HOST */

#include "torch.h"

/* torch simulation ideas from: https://github.com/plan44/messagetorch */

#define MIN(x, y) (y < x ? y : x)
#define MAX(x, y) (y > x ? y : x)

static inline
uint8_t sadd8(uint8_t value, uint8_t delta)
{
    return UINT8_MAX - value > delta ? value + delta : UINT8_MAX;
}

static inline
uint8_t sadd8_bdry(uint8_t value, uint8_t delta, uint8_t max)
{
    return max - value > delta ? value + delta : max;
}

static inline
uint8_t ssub8(uint8_t value, uint8_t delta)
{
    return value > delta ? value - delta : 0;
}

static
uint16_t rand8seed_ = UINT16_C(1337);

static inline
uint8_t rand8(void)
{
    rand8seed_ = rand8seed_ * UINT16_C(2053) + UINT16_C(13849);
    return ((uint8_t)(rand8seed_ & (uint16_t)0xFF)) + ((uint8_t)(rand8seed_ >> 8));
}

static inline
uint8_t rand8_bdry(uint8_t max)
{
    uint16_t r = rand8();
    return (uint8_t)((r * ((uint16_t)max)) >> 8);
}

typedef torch_energy_t energy_t;

#define ENERGY_LEVEL_NUM UINT8_C(32)

const uint8_t energy_quant_[ENERGY_LEVEL_NUM] PROGMEM =
{
      0,  64,  96, 112, 128, 144, 152, 160,
    168, 176, 184, 184, 192, 200, 200, 208,
    208, 216, 216, 224, 224, 224, 232, 232,
    232, 240, 240, 240, 240, 248, 248, 248
};

#define ENERGY_QUANT(value) (pgm_read_byte(energy_quant_ + (value >> 3)))
#define SPARK_THREASHOLD 16

uint8_t torch_energy_quant(uint8_t energy)
{
    return ENERGY_QUANT(energy);
};

void torch_data_map_update(torch_energy_map_t *map)
{
    if(NULL == map) return;
    if(NULL == map->data) return;

    const map_size_t stride = map->header.stride;
    const map_size_t h = map->header.height;
    const map_size_t w = map->header.width;

    /* 1. random new energy sources at the 'bottom' */
    for(map_size_t x = 0; x < w; ++x)
    {
        MAP_XY(map->data, stride, x, 0).mode = MODE_NONE;
        // 100-120
        MAP_XY(map->data, stride, x, 0).value = 100 + rand8_bdry(120);
    }

    /* 2. random sparking on second row */
    for(map_size_t x = 0, y = 1; y < h && x < w; ++x)
    {
        energy_t energy = MAP_XY(map->data, stride, x, y);

        if(MODE_SPARK == energy.mode) continue;
        if(SPARK_THREASHOLD < rand8_bdry(255)) continue;

        energy.mode = MODE_SPARK;
        // 200-255
        energy.value = 200 + rand8_bdry(55);
        MAP_XY(map->data, stride, x, y) = energy;
    }

    /* 3.  */
    for(map_size_t y = 0; y < h; ++y)
    {
        for(map_size_t x = 0; x < w; ++x)
        {
            energy_t energy = MAP_XY(map->data, stride, x, y);

            if(MODE_PASSIVE == energy.mode)
            {
                /* absorb adjacent energy
                 * | adjL | x    | adjR |
                 * |      | adjB |      | */
                if(0 < y)
                {
                    energy_t adjB =
                        0 < y
                        ? MAP_XY(map->data, stride, x, y - 1)
                        : (energy_t){.value = 0, .mode = MODE_NONE};

                    energy_t adjL =
                        MAP_XY(map->data, stride, 0 == x ? w - 1 : x - 1, y);

                    energy_t adjR =
                        MAP_XY(map->data, stride, w - 1 == x ? 0 : x + 1, y);

                    adjB.value = SCALE8(adjB.value, 40);
                    adjL.value = SCALE8(adjL.value, 35);
                    adjR.value = SCALE8(adjR.value, 35);

                    energy.value = SCALE8(energy.value, 128);
                    energy.value = sadd8(energy.value, (adjL.value >> 1)  + (adjR.value >> 1));
                    energy.value = sadd8(energy.value, adjB.value);
                }
            }
            else if(MODE_SPARK == energy.mode)
            {
                energy.value = ssub8(energy.value, 40);

                if(h - 1 > y)
                {
                    // adjT
                    MAP_XY(map->data, stride, x, y + 1).mode = MODE_TEMP;
                }
            }
            else if(MODE_TEMP == energy.mode)
            {
                if(0 < y)
                {
                    energy_t adjB = MAP_XY(map->data, stride, x, y - 1);

                    if(40 > adjB.value)
                    {
                        adjB.mode = MODE_PASSIVE;
                        MAP_XY(map->data, stride, x, y - 1) = adjB;
                        energy.value = sadd8(energy.value, adjB.value);
                        energy.value = SCALE8(energy.value, 200);
                        energy.mode = MODE_SPARK;
                    }
                    else
                    {
                        energy.value = sadd8(energy.value, 40);
                    }
                }
            }

            MAP_XY(map->data, stride, x, y) = energy;
        }
    }
}

static
rgb_t energy2color(energy_t energy)
{
    if(250 < energy.value)
    {
        return (rgb_t){.R = 170, .G = 170, .B = energy.value};
    }
    else if(0 != energy.value)
    {
        uint8_t r = 10;
        uint8_t g = 0;
        uint8_t b = 0;
        const uint8_t quantized = ENERGY_QUANT(energy.value);

        r = sadd8(r, SCALE8(180, quantized));
        g = sadd8(g, SCALE8(20, quantized));
        b = sadd8(b, SCALE8(0, quantized));

        return (rgb_t){.R = r, .G = g, .B = b};
    }
    return (rgb_t){.R = 0, .G = 0, .B = 0};
}


void torch_rgb_map_update(rgb_map_t * rgb_map, const torch_energy_map_t *map)
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

    const map_size_t len = rgb_map->header.width * rgb_map->header.height;

    for(map_size_t i = 0; i < len; ++i)
    {
        rgb_map->rgb[i] = energy2color(map->data[i]);
        rgb_map->rgb[i].R = SCALE8(rgb_map->rgb[i].R, coeff_R);
        rgb_map->rgb[i].G = SCALE8(rgb_map->rgb[i].G, coeff_G);
        rgb_map->rgb[i].B = SCALE8(rgb_map->rgb[i].B, coeff_B);
    }
}
