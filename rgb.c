#include "rgb.h"

void rgb_scale8(rgb_t *rgb, uint8_t scale)
{
    rgb->R = SCALE8(rgb->R, scale);
    rgb->G = SCALE8(rgb->G, scale);
    rgb->B = SCALE8(rgb->B, scale);
}

void rgb_saccum(rgb_t *x, const rgb_t *const y)
{
    x->R = UINT8_MAX - x->R > y->R ? x->R + y->R : UINT8_MAX;
    x->G = UINT8_MAX - x->G > y->G ? x->G + y->G : UINT8_MAX;
    x->B = UINT8_MAX - x->B > y->B ? x->B + y->B : UINT8_MAX;
}

void rgb_seq_shift(rgb_t *begin, rgb_t *end)
{
    if(begin == end) return;

    rgb_t last = *(--end);

    while(begin != end)
    {
        *end  = *(end - 1);
        --end;
    }

    *begin = last;
}
