#include "energy.h"
#include "util.h"

#if defined HOST
#define PROGMEM
#define pgm_read_byte(addr) (*(addr))
#else /* HOST */
#include <avr/pgmspace.h>
#endif /* HOST */

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

rgb_t energy2color(const energy_param_t *const param, energy_t energy)
{
    if(energy)
    {
        uint8_t r = param->color_base.R;
        uint8_t g = param->color_base.G;
        uint8_t b = param->color_base.B;
        const uint8_t quantized = energy;//ENERGY_QUANT(energy);

        r = sadd8(r, SCALE8(param->color_coeff.R, quantized));
        g = sadd8(g, SCALE8(param->color_coeff.G, quantized));
        b = sadd8(b, SCALE8(param->color_coeff.B, quantized));

        return (rgb_t){.R = r, .G = g, .B = b};
    }
    return (rgb_t){.R = 0, .G = 0, .B = 0};
}
