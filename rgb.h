#pragma once

#include <stdint.h>

#include <drv/assert.h>

#define RGB_SIZE 3

typedef union
{
    struct
    {
        /* data layout for WS2812B! 24bits [G:8][R:8][B:8] */
        uint8_t G;
        uint8_t R;
        uint8_t B;
    };

    uint8_t data[RGB_SIZE];
} rgb_t;

STATIC_ASSERT(sizeof(rgb_t) == RGB_SIZE);


/* source: lib8tion
 * data: uint8_t (R/G/B value)
 * scale: uint8_t, 64/256 (fractional representation) */
#define SCALE8(data, scale) \
    ( \
      (uint8_t)( \
        ( \
          ((uint16_t)data) \
          * \
          (1 + (uint16_t)(scale)) \
        ) \
        >> 8 \
      ) \
    )

/* source: FastLED library
 * scale: uint8_t (usually brightness)
 * color: uint8_t
 * temp: uint8_t */
#define COEFF8(scale, color, temp) \
    ( \
      (uint8_t)( \
        0 != (scale) && 0 != (color) && 0 != (temp) \
        ? ( \
            ( \
              ( \
                (((uint32_t)(color)) + 1) \
                * (((uint32_t)(temp)) + 1) \
                * (uint32_t)(scale) \
              ) >> 16 \
            ) \
          ) \
        : 0 \
      ) \
    )

inline
void rgb_scale8(rgb_t *rgb, uint8_t scale)
{
    rgb->R = SCALE8(rgb->R, scale);
    rgb->G = SCALE8(rgb->G, scale);
    rgb->B = SCALE8(rgb->B, scale);
}

inline
void rgb_saccum(rgb_t *x, const rgb_t *const y)
{
    x->R = UINT8_MAX - x->R > y->R ? x->R + y->R : UINT8_MAX;
    x->G = UINT8_MAX - x->G > y->G ? x->G + y->G : UINT8_MAX;
    x->B = UINT8_MAX - x->B > y->B ? x->B + y->B : UINT8_MAX;
}

void rgb_seq_shift(rgb_t *begin, rgb_t *end);


#define VALUE_R(x) ((uint8_t)((x) >> 16))
#define VALUE_G(x) ((uint8_t)((x) >> 8))
#define VALUE_B(x) ((uint8_t)((x) >> 0))

#define COLOR_CORRECTION_5050 0xFFB0F0ul /* 255, 176, 240 */
#define COLOR_CORRECTION_None 0xFFFFFFul /* 255, 255, 255 */

#define TEMP_CORRECTION_Candle 0xFF9329ul /* 1900 K, 255, 147, 41 */
#define TEMP_CORRECTION_Tungsten40W 0xFFC58Ful /* 2600 K, 255, 197, 143 */
#define TEMP_CORRECTION_Tungsten100W 0xFFD6AAul /* 2850 K, 255, 214, 170 */
#define TEMP_CORRECTION_Halogen 0xFFF1E0ul /* 3200 K, 255, 241, 224 */
#define TEMP_CORRECTION_CarbonArc 0xFFFAF4ul /* 5200 K, 255, 250, 244 */
#define TEMP_CORRECTION_HighNoonSun 0xFFFFFBul /* 5400 K, 255, 255, 251 */
#define TEMP_CORRECTION_DirectSunlight 0xFFFFFFul /* 6000 K, 255, 255, 255 */
#define TEMP_CORRECTION_OvercastSky 0xC9E2FFul /* 7000 K, 201, 226, 255 */
#define TEMP_CORRECTION_ClearBlueSky 0x409CFFul /* 20000 K, 64, 156, 255 */
#define TEMP_CORRECTION_WarmFluorescent 0xFFF4E5ul /* 0 K, 255, 244, 229 */
#define TEMP_CORRECTION_StandardFluorescent 0xF4FFFAul /* 0 K, 244, 255, 250 */
#define TEMP_CORRECTION_CoolWhiteFluorescent 0xD4EBFFul /* 0 K, 212, 235, 255 */
#define TEMP_CORRECTION_FullSpectrumFluorescent 0xFFF4F2ul /* 0 K, 255, 244, 242 */
#define TEMP_CORRECTION_GrowLightFluorescent 0xFFEFF7ul /* 0 K, 255, 239, 247 */
#define TEMP_CORRECTION_BlackLightFluorescent 0xA700FFul /* 0 K, 167, 0, 255 */
#define TEMP_CORRECTION_MercuryVapor 0xD8F7FFul /* 0 K, 216, 247, 255 */
#define TEMP_CORRECTION_SodiumVapor 0xFFD1B2ul /* 0 K, 255, 209, 178 */
#define TEMP_CORRECTION_MetalHalide 0xF2FCFFul /* 0 K, 242, 252, 255 */
#define TEMP_CORRECTION_HighPressureSodium 0xFFB74Cul /* 0 K, 255, 183, 76 */
#define TEMP_CORRECTION_None 0xFFFFFFul

/* this value will be used by default */
#define RGB_AliceBlue 0xF0F8FFul
#define RGB_Amethyst 0x9966CCul /* 153 102 204 */
#define RGB_AntiqueWhite 0xFAEBD7ul
#define RGB_Aqua 0x00FFFFul
#define RGB_Aquamarine 0x7FFFD4ul
#define RGB_Azure 0xF0FFFFul
#define RGB_Beige 0xF5F5DCul
#define RGB_Bisque 0xFFE4C4ul
#define RGB_Black 0x000000ul
#define RGB_BlanchedAlmond 0xFFEBCDul
#define RGB_Blue 0x0000FFul
#define RGB_BlueViolet 0x8A2BE2ul
#define RGB_Brown 0xA52A2Aul
#define RGB_BurlyWood 0xDEB887ul
#define RGB_CadetBlue 0x5F9EA0ul
#define RGB_Chartreuse 0x7FFF00ul
#define RGB_Chocolate 0xD2691Eul
#define RGB_Coral 0xFF7F50ul
#define RGB_CornflowerBlue 0x6495EDul
#define RGB_Cornsilk 0xFFF8DCul
#define RGB_Crimson 0xDC143Cul
#define RGB_Cyan 0x00FFFFul
#define RGB_DarkBlue 0x00008Bul
#define RGB_DarkCyan 0x008B8Bul
#define RGB_DarkGoldenrod 0xB8860Bul
#define RGB_DarkGray 0xA9A9A9ul
#define RGB_DarkGrey 0xA9A9A9ul
#define RGB_DarkGreen 0x006400ul
#define RGB_DarkKhaki 0xBDB76Bul
#define RGB_DarkMagenta 0x8B008Bul /* 184, 0, 139 */
#define RGB_DarkOliveGreen 0x556B2Ful
#define RGB_DarkOrange 0xFF8C00ul
#define RGB_DarkOrchid 0x9932CCul
#define RGB_DarkRed 0x8B0000ul
#define RGB_DarkSalmon 0xE9967Aul
#define RGB_DarkSeaGreen 0x8FBC8Ful
#define RGB_DarkSlateBlue 0x483D8Bul
#define RGB_DarkSlateGray 0x2F4F4Ful
#define RGB_DarkSlateGrey 0x2F4F4Ful
#define RGB_DarkTurquoise 0x00CED1ul
#define RGB_DarkViolet 0x9400D3ul
#define RGB_DeepPink 0xFF1493ul
#define RGB_DeepSkyBlue 0x00BFFFul
#define RGB_DimGray 0x696969ul
#define RGB_DimGrey 0x696969ul
#define RGB_DodgerBlue 0x1E90FFul
#define RGB_FireBrick 0xB22222ul
#define RGB_FloralWhite 0xFFFAF0ul
#define RGB_ForestGreen 0x228B22ul
#define RGB_Fuchsia 0xFF00FFul
#define RGB_Gainsboro 0xDCDCDCul
#define RGB_GhostWhite 0xF8F8FFul
#define RGB_Gold 0xFFD700ul
#define RGB_Goldenrod 0xDAA520ul
#define RGB_Gray 0x808080ul
#define RGB_Grey 0x808080ul
#define RGB_Green 0x008000ul
#define RGB_GreenYellow 0xADFF2Ful
#define RGB_Honeydew 0xF0FFF0ul
#define RGB_HotPink 0xFF69B4ul
#define RGB_IndianRed 0xCD5C5Cul
#define RGB_Indigo 0x4B0082ul
#define RGB_Ivory 0xFFFFF0ul
#define RGB_Khaki 0xF0E68Cul
#define RGB_Lavender 0xE6E6FAul
#define RGB_LavenderBlush 0xFFF0F5ul
#define RGB_LawnGreen 0x7CFC00ul
#define RGB_LemonChiffon 0xFFFACDul
#define RGB_LightBlue 0xADD8E6ul
#define RGB_LightCoral 0xF08080ul
#define RGB_LightCyan 0xE0FFFFul
#define RGB_LightGoldenrodYellow 0xFAFAD2ul
#define RGB_LightGreen 0x90EE90ul
#define RGB_LightGrey 0xD3D3D3ul
#define RGB_LightPink 0xFFB6C1ul
#define RGB_LightSalmon 0xFFA07Aul
#define RGB_LightSeaGreen 0x20B2AAul
#define RGB_LightSkyBlue 0x87CEFAul
#define RGB_LightSlateGray 0x778899ul
#define RGB_LightSlateGrey 0x778899ul
#define RGB_LightSteelBlue 0xB0C4DEul
#define RGB_LightYellow 0xFFFFE0ul
#define RGB_Lime 0x00FF00ul
#define RGB_LimeGreen 0x32CD32ul
#define RGB_Linen 0xFAF0E6ul
#define RGB_Magenta 0xFF00FFul
#define RGB_Maroon 0x800000ul
#define RGB_MediumAquamarine 0x66CDAAul
#define RGB_MediumBlue 0x0000CDul
#define RGB_MediumOrchid 0xBA55D3ul
#define RGB_MediumPurple 0x9370DBul
#define RGB_MediumSeaGreen 0x3CB371ul
#define RGB_MediumSlateBlue 0x7B68EEul
#define RGB_MediumSpringGreen 0x00FA9Aul
#define RGB_MediumTurquoise 0x48D1CCul
#define RGB_MediumVioletRed 0xC71585ul
#define RGB_MidnightBlue 0x191970ul
#define RGB_MintCream 0xF5FFFAul
#define RGB_MistyRose 0xFFE4E1ul
#define RGB_Moccasin 0xFFE4B5ul
#define RGB_NavajoWhite 0xFFDEADul
#define RGB_Navy 0x000080ul
#define RGB_OldLace 0xFDF5E6ul
#define RGB_Olive 0x808000ul
#define RGB_OliveDrab 0x6B8E23ul
#define RGB_Orange 0xFFA500ul
#define RGB_OrangeRed 0xFF4500ul
#define RGB_Orchid 0xDA70D6ul
#define RGB_PaleGoldenrod 0xEEE8AAul
#define RGB_PaleGreen 0x98FB98ul
#define RGB_PaleTurquoise 0xAFEEEEul
#define RGB_PaleVioletRed 0xDB7093ul
#define RGB_PapayaWhip 0xFFEFD5ul
#define RGB_PeachPuff 0xFFDAB9ul
#define RGB_Peru 0xCD853Ful
#define RGB_Pink 0xFFC0CBul
#define RGB_Plaid 0xCC5533ul
#define RGB_Plum 0xDDA0DDul
#define RGB_PowderBlue 0xB0E0E6ul
#define RGB_Purple 0x800080ul
#define RGB_Red 0xFF0000ul
#define RGB_RosyBrown 0xBC8F8Ful
#define RGB_RoyalBlue 0x4169E1ul
#define RGB_SaddleBrown 0x8B4513ul
#define RGB_Salmon 0xFA8072ul
#define RGB_SandyBrown 0xF4A460ul
#define RGB_SeaGreen 0x2E8B57ul
#define RGB_Seashell 0xFFF5EEul
#define RGB_Sienna 0xA0522Dul
#define RGB_Silver 0xC0C0C0ul
#define RGB_SkyBlue 0x87CEEBul
#define RGB_SlateBlue 0x6A5ACDul
#define RGB_SlateGray 0x708090ul
#define RGB_SlateGrey 0x708090ul
#define RGB_Snow 0xFFFAFAul
#define RGB_SpringGreen 0x00FF7Ful
#define RGB_SteelBlue 0x4682B4ul
#define RGB_Tan 0xD2B48Cul
#define RGB_Teal 0x008080ul
#define RGB_Thistle 0xD8BFD8ul
#define RGB_Tomato 0xFF6347ul
#define RGB_Turquoise 0x40E0D0ul
#define RGB_Violet 0xEE82EEul
#define RGB_Wheat 0xF5DEB3ul
#define RGB_White 0xFFFFFFul
#define RGB_WhiteSmoke 0xF5F5F5ul
#define RGB_Yellow 0xFFFF00ul
#define RGB_YellowGreen 0x9ACD32ul
#define RGB_FairyLight 0xFFE42Dul

