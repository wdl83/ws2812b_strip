#include <stddef.h>
#include <string.h>

#if defined HOST
#define PROGMEM
#define pgm_read_byte(addr) (*(addr))
#else /* HOST */
#include <avr/pgmspace.h>
#endif /* HOST */

#include "palette.h"
#include "torch.h"
#include "util.h"

static
uint8_t *mode_addr(torch_map_t *map)
{
    uint16_t offset = map->header.width * map->header.height;
    return map->raw +  offset;
}

static
energy_t *energy_addr(torch_map_t *map)
{
    return map->raw;
}

/* torch simulation ideas from: https://github.com/plan44/messagetorch */

#define ENERGY_LEVEL_NUM UINT8_C(32)

static
const uint8_t energy_quant_[ENERGY_LEVEL_NUM] PROGMEM =
{
      0,  64,  96, 112, 128, 144, 152, 160,
    168, 176, 184, 184, 192, 200, 200, 208,
    208, 216, 216, 224, 224, 224, 232, 232,
    232, 240, 240, 240, 240, 248, 248, 248
};

#define ENERGY_QUANT(value) (pgm_read_byte(energy_quant_ + (value >> 3)))

void torch_map_update(torch_map_t *map)
{
    const map_size_t stride = map->header.stride;
    const map_size_t h = map->header.height;
    const map_size_t w = map->header.width;
    const uint8_t adjH = map->adjH;
    const uint8_t adjV = map->adjV;
    const uint8_t passive_preserve = map->passive_preserve;
    const uint8_t spark_preserve = map->spark_preserve;
    const uint8_t spark_transfer = map->spark_transfer;
    uint8_t *mode = mode_addr(map);
    energy_t *energy = energy_addr(map);

    /* 1. random new energy sources at the 'bottom' */
    for(map_size_t x = 0; x < w; ++x)
    {
        TORCH_MODE_SET(mode, stride, x, 0, TORCH_MODE_NONE);
        // <100, 220>
        MAP_XY(energy, stride, x, 0) = 100 + rand8_bdry(121);
    }

    /* 2. random sparking on second row */
    for(map_size_t x = 0, y = 1; y < h && x < w; ++x)
    {
        if(TORCH_MODE_SPARK == TORCH_MODE_GET(mode, stride, x, y)) continue;
        if(map->spark_threshold < rand8_bdry(255)) continue;

        TORCH_MODE_SET(mode, stride, x, y, TORCH_MODE_SPARK);
        // <200, 255>
        MAP_XY(energy, stride, x, y) = 200 + rand8_bdry(56);
    }

    /* 3.  */
    for(map_size_t y = 0; y < h; ++y)
    {
        for(map_size_t x = 0; x < w; ++x)
        {
            energy_t e = MAP_XY(energy, stride, x, y);
            uint8_t m = TORCH_MODE_GET(mode, stride, x, y);

            if(TORCH_MODE_PASSIVE == m)
            {
                /* absorb adjacent energy
                 * | adjL | x    | adjR |
                 * |      | adjB |      | */
                if(y)
                {
                    energy_t adjB = MAP_XY(energy, stride, x, y - 1);
                    energy_t adjL = MAP_XY(energy, stride, 0 == x ? w - 1 : x - 1, y);
                    energy_t adjR = MAP_XY(energy, stride, w - 1 == x ? 0 : x + 1, y);

                    // adjB
                    uint8_t deltaLR =
                        ((uint32_t)((uint16_t)adjL + (uint16_t)adjR) * adjH) >> 9;
                    uint8_t deltaB = SCALE8(adjB, adjV);

                    e = SCALE8(e, passive_preserve);
                    e = sadd8(e, deltaLR);
                    e = sadd8(e, deltaB);
                }
            }
            else if(TORCH_MODE_SPARK == m)
            {
                e = ssub8(e, spark_transfer);

                if(h - 1 > y)
                {
                    // adjT
                    TORCH_MODE_SET(mode, stride, x, y + 1, TORCH_MODE_TEMP);
                }
                else if(h - 1 == y)
                {
                    m = TORCH_MODE_PASSIVE;
                }
            }
            else if(TORCH_MODE_TEMP == m)
            {
                if(y)
                {
                    const energy_t adjB = MAP_XY(energy, stride, x, y - 1);

                    if(spark_transfer > adjB)
                    {
                        TORCH_MODE_SET(mode, stride, x, y - 1, TORCH_MODE_PASSIVE);
                        e = sadd8(e, adjB);
                        e = SCALE8(e, spark_preserve);
                        m = TORCH_MODE_SPARK;
                    }
                    else
                    {
                        e = sadd8(e, spark_transfer);
                    }
                }
            }

            MAP_XY(energy, stride, x, y) = e;
            TORCH_MODE_SET(mode, stride, x, y, m);
        }
    }
}

static
rgb_t energy2rgb(const torch_map_t *const map, energy_t energy)
{
    if(250 < energy)
    {
        return (rgb_t){.R = 170, .G = 170, .B = energy};
    }
    else if(0 != energy)
    {
        uint8_t r = 10;
        uint8_t g = 0;
        uint8_t b = 0;
        const uint8_t quantized = ENERGY_QUANT(energy);

        r = sadd8(r, SCALE8(map->color_coeff.R, quantized));
        g = sadd8(g, SCALE8(map->color_coeff.G, quantized));
        b = sadd8(b, SCALE8(map->color_coeff.B, quantized));

        return (rgb_t){.R = r, .G = g, .B = b};
    }
    return (rgb_t){.R = 0, .G = 0, .B = 0};
}

void torch_rgb_map_update(rgb_map_t * rgb_map, const torch_map_t *map)
{
    const map_size_t len = rgb_map->header.width * rgb_map->header.height;
    energy_t *energy = energy_addr((torch_map_t *)map);

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
        rgb_map->rgb[i] = energy2rgb(map, energy[i]);
        rgb_map->rgb[i].R = SCALE8(rgb_map->rgb[i].R, coeff_R);
        rgb_map->rgb[i].G = SCALE8(rgb_map->rgb[i].G, coeff_G);
        rgb_map->rgb[i].B = SCALE8(rgb_map->rgb[i].B, coeff_B);
    }
}

void torch_init(torch_map_t *map)
{
#if 0
    map->spark_threshold = 4;
    map->adjH = 35;
    map->adjV = 40;
    map->passive_preserve = 120;
    map->spark_transfer = 40;
    map->spark_preserve = 200;
    map->color_coeff.R = 180;
    map->color_coeff.G = 20;
    map->color_coeff.B = 0;
#endif
    uint16_t size = map->header.width * map->header.height;
    energy_t *energy = energy_addr(map);
    uint8_t *mode = mode_addr(map);

    memset(energy, 0, size);
    memset(mode, TORCH_MODE_NONE, size >> 2);
}
