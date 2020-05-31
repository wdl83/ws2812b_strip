#include "palette.h"
#include "rgb.h"

#if defined HOST
#define PROGMEM
#define pgm_read_byte(addr) (*(addr))
#else /* HOST */
#include <avr/pgmspace.h>
#endif /* HOST */

typedef struct
{
    const rgb_t data[16];
} palette_t;

/* Palette values source: FastLED library */
const palette_t palette_[] PROGMEM =
{
    {
        /* Heat */
        .data =
        {
            (rgb_t){.R = 0x00, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x33, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x66, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x99, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0xCC, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0x33, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0x66, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0x99, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0xCC, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0xFF, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0xFF, .B = 0x33},
            (rgb_t){.R = 0xFF, .G = 0xFF, .B = 0x66},
            (rgb_t){.R = 0xFF, .G = 0xFF, .B = 0x99},
            (rgb_t){.R = 0xFF, .G = 0xFF, .B = 0xCC},
            (rgb_t){.R = 0xFF, .G = 0xFF, .B = 0xFF}
        }
    },
    {
        /* Laval */
        .data =
        {
            (rgb_t){.R = 0x00, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x80, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x00, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x80, .G = 0x00, .B = 0x00},

            (rgb_t){.R = 0x8B, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x80, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x8B, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x8B, .G = 0x00, .B = 0x00},

            (rgb_t){.R = 0x8B, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0xA5, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0xFF, .B = 0xFF},

            (rgb_t){.R = 0xFF, .G = 0xA5, .B = 0x00},
            (rgb_t){.R = 0xFF, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x8B, .G = 0x00, .B = 0x00},
            (rgb_t){.R = 0x00, .G = 0x00, .B = 0x00}
        }
    },
    {
        /* Forest */
        .data =
        {
            (rgb_t){.R = 0x00, .G = 0x64, .B = 0x00},
            (rgb_t){.R = 0x00, .G = 0x64, .B = 0x00},
            (rgb_t){.R = 0x55, .G = 0x6B, .B = 0x2F},
            (rgb_t){.R = 0x00, .G = 0x64, .B = 0x00},
            (rgb_t){.R = 0x00, .G = 0x80, .B = 0x00},
            (rgb_t){.R = 0x22, .G = 0x8B, .B = 0x22},
            (rgb_t){.R = 0x6B, .G = 0x8E, .B = 0x23},
            (rgb_t){.R = 0x00, .G = 0x80, .B = 0x00},
            (rgb_t){.R = 0x2E, .G = 0x8B, .B = 0x57},
            (rgb_t){.R = 0x66, .G = 0xCD, .B = 0xAA},
            (rgb_t){.R = 0x32, .G = 0xCD, .B = 0x32},
            (rgb_t){.R = 0x9A, .G = 0xCD, .B = 0x32},
            (rgb_t){.R = 0x90, .G = 0xEE, .B = 0x90},
            (rgb_t){.R = 0x7C, .G = 0xFC, .B = 0x00},
            (rgb_t){.R = 0x66, .G = 0xCD, .B = 0xAA},
            (rgb_t){.R = 0x22, .G = 0x8B, .B = 0x22}
        }
    }
};

inline static
rgb_t rgb_data(palette_id_t id, uint8_t idx)
{
    const palette_t *const palette = palette_ + id.value;
    rgb_t r;

    r.R = pgm_read_byte(&palette->data[idx].R);
    r.G = pgm_read_byte(&palette->data[idx].G);
    r.B = pgm_read_byte(&palette->data[idx].B);
    return r;
}

rgb_t palette_color(palette_id_t id, uint8_t index, uint8_t brightness)
{
    if(0 == brightness) return (rgb_t){.R = 0, .G = 0, .B = 0};

    const uint8_t hi = index >> 4;
    const uint8_t low = index & 0xF;
    rgb_t base = rgb_data(id, hi);

    if(!low) return base;

    rgb_t offset = rgb_data(id, (hi + 1) & 0xF);
    const uint8_t rangeL = low << 4;
    const uint8_t rangeH = 255 - rangeL;

    rgb_scale8(&base, rangeH);
    rgb_scale8(&offset, rangeL);
    rgb_saccum(&base, &offset);

    if(255 == brightness) return base;

    rgb_scale8(&base, brightness);
    return base;
}
