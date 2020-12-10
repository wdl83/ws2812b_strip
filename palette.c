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
} palette16_t;

/* Palette sources:
 * 1) FastLED library
 * 2) http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_47.png.index.html
 * */
static
const palette16_t palette16_[] PROGMEM =
{
    { /* Heat */
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
    { /* Laval */
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
    { /* bhw1_sunset2 */
        .data =
        {
            (rgb_t){.R = 255,  .G = 175,  .B =   8},
            (rgb_t){.R = 251,  .G = 145,  .B =   8},
            (rgb_t){.R = 247,  .G = 116,  .B =   8},
            (rgb_t){.R = 244,  .G =  87,  .B =   9},
            (rgb_t){.R = 240,  .G =  58,  .B =   9},
            (rgb_t){.R = 237,  .G =  29,  .B =   9},
            (rgb_t){.R = 237,  .G =  29,  .B =  10},
            (rgb_t){.R = 192,  .G =  43,  .B =  26},
            (rgb_t){.R = 148,  .G =  57,  .B =  42},
            (rgb_t){.R = 148,  .G =  57,  .B =  42},
            (rgb_t){.R =  68,  .G =  54,  .B =  54},
            (rgb_t){.R =  68,  .G =  54,  .B =  54},
            (rgb_t){.R =  55,  .G =  46,  .B =  47},
            (rgb_t){.R =  43,  .G =  38,  .B =  41},
            (rgb_t){.R =  30,  .G =  30,  .B =  35},
            (rgb_t){.R =  18,  .G =  23,  .B =  29},
        }
    },
    { /* bhw2 whooo */
        .data =
        {
            (rgb_t){.R =   7,  .G = 128,  .B =  95},
            (rgb_t){.R =   8,  .G =   9,  .B =  35},
            (rgb_t){.R =   8,  .G =   9,  .B =  35},
            (rgb_t){.R =  21,  .G =  24,  .B =  25},
            (rgb_t){.R =  21,  .G =  24,  .B =  25},
            (rgb_t){.R =  77,  .G =  55,  .B =  96},
            (rgb_t){.R = 255,  .G = 255,  .B = 125},
            (rgb_t){.R =  45,  .G =  39,  .B =   4},
            (rgb_t){.R = 224,  .G =  45,  .B = 144},
            (rgb_t){.R =  82,  .G =   1,  .B =  34},
            (rgb_t){.R =  39,  .G =  47,  .B =  35},
            (rgb_t){.R =  39,  .G =  47,  .B =  35},
            (rgb_t){.R = 155,  .G =  99,  .B =  91},
            (rgb_t){.R =   4,  .G =  10,  .B =  21},
            (rgb_t){.R =   5,  .G =  66,  .B =  68},
            (rgb_t){.R =   5,  .G =  66,  .B =  68},
        }
    },

    { /* bhw2_47 */
       .data =
        {
            (rgb_t){.R =  78,  .G =  43,  .B = 205},
            (rgb_t){.R =   1,  .G = 111,  .B = 114},
            (rgb_t){.R =   1,  .G = 111,  .B = 114},
            (rgb_t){.R = 121,  .G = 255,  .B = 125},
            (rgb_t){.R = 121,  .G = 255,  .B = 125},
            (rgb_t){.R =  38,  .G = 191,  .B = 122},
            (rgb_t){.R =  46,  .G =  93,  .B = 119},
            (rgb_t){.R =  78,  .G =  43,  .B = 205},
            (rgb_t){.R =  78,  .G =  43,  .B = 205},
            (rgb_t){.R =  78,  .G =  43,  .B = 205},
            (rgb_t){.R =  46,  .G =  93,  .B = 119},
            (rgb_t){.R =  46,  .G =  93,  .B = 119},
            (rgb_t){.R =  38,  .G = 191,  .B = 122},
            (rgb_t){.R =  38,  .G = 191,  .B = 122},
            (rgb_t){.R = 121,  .G = 255,  .B = 125},
            (rgb_t){.R =   1,  .G = 111,  .B = 114}
        }
    },
    { /* bhw2 grrrrr */
        .data =
        {
            (rgb_t){.R = 184,  .G =  15,  .B = 155},
            (rgb_t){.R = 131,  .G =  30,  .B = 161},
            (rgb_t){.R =  78,  .G =  46,  .B = 168},
            (rgb_t){.R =  78,  .G =  46,  .B = 168},
            (rgb_t){.R =  71,  .G = 107,  .B = 199},
            (rgb_t){.R =  65,  .G = 169,  .B = 230},
            (rgb_t){.R =  65,  .G = 169,  .B = 230},
            (rgb_t){.R =  37,  .G = 148,  .B = 208},
            (rgb_t){.R =   9,  .G = 127,  .B = 186},
            (rgb_t){.R =   9,  .G = 127,  .B = 186},
            (rgb_t){.R =  77,  .G = 182,  .B = 109},
            (rgb_t){.R =  77,  .G = 182,  .B = 109},
            (rgb_t){.R = 242,  .G = 246,  .B =  55},
            (rgb_t){.R = 142,  .G = 128,  .B = 103},
            (rgb_t){.R = 142,  .G = 128,  .B = 103},
            (rgb_t){.R =  72,  .G =  50,  .B = 168}
        }
    },
    { /* bhw3 07 */
        .data =
        {
            (rgb_t){.R = 132,  .G =   5,  .B =   1},
            (rgb_t){.R =  12,  .G =  13,  .B = 147},
            (rgb_t){.R =  12,  .G =  13,  .B = 147},
            (rgb_t){.R =  54,  .G =  32,  .B = 166},
            (rgb_t){.R =  54,  .G =  32,  .B = 166},
            (rgb_t){.R = 103,  .G =  39,  .B =  98},
            (rgb_t){.R = 239,  .G =  52,  .B =  27},
            (rgb_t){.R =  71,  .G =   2,  .B =   1},
            (rgb_t){.R =  46,  .G =   8,  .B =  33},
            (rgb_t){.R =  54,  .G =  32,  .B = 166},
            (rgb_t){.R =  10,  .G =   7,  .B =  78},
            (rgb_t){.R =  10,  .G =   7,  .B =  78},
            (rgb_t){.R = 239,  .G =  52,  .B =  27},
            (rgb_t){.R = 239,  .G =  52,  .B =  27},
            (rgb_t){.R = 120,  .G =  26,  .B =  20},
            (rgb_t){.R =   1,  .G =   1,  .B =  14}
        }
    },
    { /* bhw3 30 */
        .data =
        {
            (rgb_t){.R =   1,  .G =   4,  .B =   1},
            (rgb_t){.R =   1,  .G =  28,  .B =   2},
            (rgb_t){.R =  16,  .G =  68,  .B =  11},
            (rgb_t){.R =  18,  .G =  11,  .B =  28},
            (rgb_t){.R =  18,  .G =  11,  .B =  28},
            (rgb_t){.R =  31,  .G =  27,  .B =  48},
            (rgb_t){.R =   2,  .G =  13,  .B =  34},
            (rgb_t){.R =   2,  .G =  13,  .B =  34},
            (rgb_t){.R =  30,  .G =  86,  .B = 123},
            (rgb_t){.R =   2,  .G =  42,  .B =  62},
            (rgb_t){.R =  74,  .G = 128,  .B = 155},
            (rgb_t){.R =  60,  .G =  31,  .B =  41},
            (rgb_t){.R =  60,  .G =  31,  .B =  41},
            (rgb_t){.R = 167,  .G = 211,  .B = 223},
            (rgb_t){.R = 167,  .G = 211,  .B = 223},
            (rgb_t){.R = 179,  .G =  70,  .B =  78},
        }
    },
    { /* bhw4 028 */
        .data =
        {
            (rgb_t){.R =  73,  .G =  22,  .B =  74},
            (rgb_t){.R =   7,  .G =  78,  .B = 186},
            (rgb_t){.R =   7,  .G =  78,  .B = 186},
            (rgb_t){.R = 128,  .G = 162,  .B = 201},
            (rgb_t){.R = 128,  .G = 162,  .B = 201},
            (rgb_t){.R =  88,  .G =  99,  .B = 151},
            (rgb_t){.R =  88,  .G =  99,  .B = 151},
            (rgb_t){.R = 155,  .G = 107,  .B = 140},
            (rgb_t){.R = 159,  .G =  25,  .B =   1},
            (rgb_t){.R = 159,  .G =  25,  .B =   1},
            (rgb_t){.R = 208,  .G = 207,  .B =  60},
            (rgb_t){.R = 208,  .G = 207,  .B =  60},
            (rgb_t){.R = 173,  .G =  20,  .B =  38},
            (rgb_t){.R = 173,  .G =  20,  .B =  38},
            (rgb_t){.R = 169,  .G =  12,  .B =  16},
            (rgb_t){.R = 169,  .G =  12,  .B =  16}
        }
    },
    { /* bhw4 062 */
        .data =
        {
            (rgb_t){.R =   4,  .G =   1,  .B =  12},
            (rgb_t){.R =  10,  .G =   2,  .B =  25},
            (rgb_t){.R =  28,  .G =  16,  .B = 138},
            (rgb_t){.R =  28,  .G =  16,  .B = 138},
            (rgb_t){.R = 210,  .G = 108,  .B = 205},
            (rgb_t){.R = 128,  .G =  63,  .B = 139},
            (rgb_t){.R =  47,  .G =  18,  .B =  74},
            (rgb_t){.R =  47,  .G =  18,  .B =  74},
            (rgb_t){.R = 229,  .G = 244,  .B = 255},
            (rgb_t){.R = 229,  .G = 244,  .B = 255},
            (rgb_t){.R = 165,  .G = 118,  .B = 228},
            (rgb_t){.R =  83,  .G =  53,  .B = 174},
            (rgb_t){.R =  83,  .G =  53,  .B = 174},
            (rgb_t){.R =   8,  .G =   2,  .B =  42},
            (rgb_t){.R =   8,  .G =   2,  .B =  42},
            (rgb_t){.R =   1,  .G =   1,  .B =  12}
        }
    }
};

static
rgb_t rgb_data(palette16_id_t id, uint8_t idx)
{
    const palette16_t *const palette = palette16_ + id.value;
    rgb_t r;

    r.R = pgm_read_byte(&palette->data[idx].R);
    r.G = pgm_read_byte(&palette->data[idx].G);
    r.B = pgm_read_byte(&palette->data[idx].B);
    return r;
}

rgb_t palette16_color(palette16_id_t id, uint8_t index, uint8_t brightness)
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
