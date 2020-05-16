#include "rgb.h"

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
