#include "util.h"

static
uint16_t rand8seed_ = UINT16_C(1337);

uint8_t rand8(void)
{
    rand8seed_ = rand8seed_ * UINT16_C(2053) + UINT16_C(13849);
    return ((uint8_t)(rand8seed_ & (uint16_t)0xFF)) + ((uint8_t)(rand8seed_ >> 8));
}

static
uint16_t rand16seed_ = UINT16_C(1337);

uint16_t rand16(void)
{
    rand16seed_ = rand16seed_ * UINT16_C(2053) + UINT16_C(13849);
    return rand16seed_;
}
