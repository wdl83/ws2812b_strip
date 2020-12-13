#pragma once

#include <stdint.h>

#include "rgb.h"

#define PALETTE16_ID_INVALID UINT8_C(0xFF)

#define PALETTE16_ID_HEAT                                          UINT8_C(0x00)
#define PALETTE16_ID_LAVA                                          UINT8_C(0x01)
#define PALETTE16_ID_BHW1_SUNSET2                                  UINT8_C(0x02)
#define PALETTE16_ID_BHW2_WHOOO                                    UINT8_C(0x03)
#define PALETTE16_ID_BHW2_47                                       UINT8_C(0x04)
#define PALETTE16_ID_BHW2_G5R                                      UINT8_C(0x05)
#define PALETTE16_ID_BHW3_07                                       UINT8_C(0x06)
#define PALETTE16_ID_BHW3_30                                       UINT8_C(0x07)
#define PALETTE16_ID_BHW4_028                                      UINT8_C(0x08)
#define PALETTE16_ID_BHW4_062                                      UINT8_C(0x09)
#define PALETTE16_ID_BHW1_31                                       UINT8_C(0x0A)
#define PALETTE16_ID_BHW2_57                                       UINT8_C(0x0B)
#define PALETTE16_ID_BHW1_04                                       UINT8_C(0x0C)
#define PALETTE16_ID_BHW1_05                                       UINT8_C(0x0D)

typedef struct
{
    uint8_t value;
} palette16_id_t;

rgb_t palette16_color(palette16_id_t id, uint8_t index, uint8_t brightness);
