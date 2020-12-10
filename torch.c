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

/* torch simulation ideas from: https://github.com/plan44/messagetorch */

typedef torch_energy_t energy_t;
typedef torch_param_t param_t;

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

void torch_energy_map_update(torch_energy_map_t *map)
{
    if(NULL == map) return;
    if(NULL == map->data) return;

    const map_size_t stride = map->header.stride;
    const map_size_t h = map->header.height;
    const map_size_t w = map->header.width;
    param_t *const param = map->param;
    const uint8_t adjH = param->adjH;
    const uint8_t adjV = param->adjV;
    const uint8_t passive_preserve = param->passive_preserve;
    const uint8_t spark_preserve = param->spark_preserve;
    const uint8_t spark_transfer = param->spark_transfer;

    /* 1. random new energy sources at the 'bottom' */
    for(map_size_t x = 0; x < w; ++x)
    {
        TORCH_MODE_SET(param->mode, stride, x, 0, TORCH_MODE_NONE);
        // <100, 220>
        MAP_XY(map->data, stride, x, 0) = 100 + rand8_bdry(121);
    }

    /* 2. random sparking on second row */
    for(map_size_t x = 0, y = 1; y < h && x < w; ++x)
    {
        if(TORCH_MODE_SPARK == TORCH_MODE_GET(param->mode, stride, x, y)) continue;
        if(param->spark_threshold < rand8_bdry(255)) continue;

        TORCH_MODE_SET(param->mode, stride, x, y, TORCH_MODE_SPARK);
        // <200, 255>
        MAP_XY(map->data, stride, x, y) = 200 + rand8_bdry(56);
    }

    /* 3.  */
    for(map_size_t y = 0; y < h; ++y)
    {
        for(map_size_t x = 0; x < w; ++x)
        {
            energy_t energy = MAP_XY(map->data, stride, x, y);
            uint8_t mode = TORCH_MODE_GET(param->mode, stride, x, y);

            if(TORCH_MODE_PASSIVE == mode)
            {
                /* absorb adjacent energy
                 * | adjL | x    | adjR |
                 * |      | adjB |      | */
                if(y)
                {
                    energy_t adjB = MAP_XY(map->data, stride, x, y - 1);
                    energy_t adjL = MAP_XY(map->data, stride, 0 == x ? w - 1 : x - 1, y);
                    energy_t adjR = MAP_XY(map->data, stride, w - 1 == x ? 0 : x + 1, y);

                    // adjB mode
                    uint8_t deltaLR =
                        ((uint32_t)((uint16_t)adjL + (uint16_t)adjR) * adjH) >> 9;
                    uint8_t deltaB = SCALE8(adjB, adjV);

                    energy = SCALE8(energy, passive_preserve);
                    energy = sadd8(energy, deltaLR);
                    energy = sadd8(energy, deltaB);
                }
            }
            else if(TORCH_MODE_SPARK == mode)
            {
                energy = ssub8(energy, spark_transfer);

                if(h - 1 > y)
                {
                    // adjT
                    TORCH_MODE_SET(param->mode, stride, x, y + 1, TORCH_MODE_TEMP);
                }
                else if(h - 1 == y)
                {
                    mode = TORCH_MODE_PASSIVE;
                }
            }
            else if(TORCH_MODE_TEMP == mode)
            {
                if(y)
                {
                    const energy_t adjB = MAP_XY(map->data, stride, x, y - 1);

                    if(spark_transfer > adjB)
                    {
                        TORCH_MODE_SET(param->mode, stride, x, y - 1, TORCH_MODE_PASSIVE);
                        energy = sadd8(energy, adjB);
                        energy = SCALE8(energy, spark_preserve);
                        mode = TORCH_MODE_SPARK;
                    }
                    else
                    {
                        energy = sadd8(energy, spark_transfer);
                    }
                }
            }

            MAP_XY(map->data, stride, x, y) = energy;
            TORCH_MODE_SET(param->mode, stride, x, y, mode);
        }
    }
}

static
rgb_t energy2color(const param_t *const param, energy_t energy)
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

        r = sadd8(r, SCALE8(param->color_coeff.R, quantized));
        g = sadd8(g, SCALE8(param->color_coeff.G, quantized));
        b = sadd8(b, SCALE8(param->color_coeff.B, quantized));

        return (rgb_t){.R = r, .G = g, .B = b};
    }
    return (rgb_t){.R = 0, .G = 0, .B = 0};
}

void torch_rgb_map_update(rgb_map_t * rgb_map, const torch_energy_map_t *map)
{
    const map_size_t len = rgb_map->header.width * rgb_map->header.height;

    if(PALETTE16_ID_INVALID == rgb_map->palette16_id.value)
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
            rgb_map->rgb[i] = energy2color(map->param, map->data[i]);
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
                palette16_color(
                    rgb_map->palette16_id,
                    SCALE8(map->data[i], 240),
                    rgb_map->brightness);
        }
    }
}

void torch_init(torch_energy_map_t *map)
{
    if(!map) return;
    if(!map->param) return;

    map->param->spark_threshold = 4;
    map->param->adjH = 35;
    map->param->adjV = 40;
    map->param->passive_preserve = 120;
    map->param->spark_transfer = 40;
    map->param->spark_preserve = 200;
    map->param->color_coeff.R = 180;
    map->param->color_coeff.G = 20;
    map->param->color_coeff.B = 0;

    // mode is encoded 2 bits
    // TODO: round-up
    memset(
        map->param->mode,
        TORCH_MODE_NONE,
        (map->header.width * map->header.height) >> 2);
}
