#pragma once

#include <stddef.h>
#include <stdint.h>

#define MIN(x, y) ((y) < (x) ? (y) : (x))
#define MAX(x, y) ((y) > (x) ? (y) : (x))


#define sadd8_impl(/* uint8_t */ value, /* uint8_t */ delta) \
    (UINT8_MAX - (value) > (delta) ? (value) + (delta) : UINT8_MAX)

#define sadd8(value, delta) \
    sadd8_impl((uint8_t)(value), (uint8_t)(delta))

#define sadd8_bdry_impl(/* uint8_t */ value, /* uint8_t */ delta, /* uint8_t */ max) \
    ((max) - (value) > (delta) ? (value) + (delta) : (max))

#define sadd8_bdry(value, delta, max) \
    sadd8_bdry_impl((uint8_t)(value), (uint8_t)(delta), (uint8_t)(max))

#define ssub8_impl(/* uint8_t */ value, /* uint8_t */ delta) \
    ((value) > (delta) ? (value) - (delta) : UINT8_C(0))

#define ssub8(value, delta) \
    ssub8_impl((uint8_t)(value), (uint8_t)(delta))

#define rand8_bdry_impl(/* uint8_t */ max) \
    ((uint8_t)((((uint16_t)rand8()) * ((uint16_t)(max))) >> 8))

#define rand8_bdry(max) \
    rand8_bdry_impl((uint8_t)(max))

uint8_t rand8(void);
uint16_t rand16(void);


#define SCALE16_IMPL(i, scale) \
{ \
    ((uint32_t)(i) * (1 + (uint32_t)(scale))) / UINT32_C(UINT16_MAX + 1) \
} \

#define SCALE16(i, scale) SCALE16_IMPL((uint16_t)(i), (uint16_t)(scale))
