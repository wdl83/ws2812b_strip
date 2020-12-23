#include <string.h>

#include "noise.h"
#include "util.h"

#if defined HOST
#define PROGMEM
#define pgm_read_byte(addr) (*(addr))
#else /* HOST */
#include <avr/pgmspace.h>
#endif /* HOST */

/* source FastLED */
static
const uint8_t P_[] PROGMEM =
{
    151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225,
    140,  36, 103,  30,  69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148,
    247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,  11,  32,
     57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,
     74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
     60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,
     65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169,
    200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,
     52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212,
    207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
    119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9,
    129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104,
    218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,
     81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157,
    184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
    222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180,
    151
};

#define P(i) pgm_read_byte(P_ + (i))
#define N() UINT8_C(0x80)

static
uint8_t ease8InOutQuad(uint8_t i)
{
    uint8_t j = i;

    if(j & 0x80) j = 255 - j;

    uint8_t jj  = SCALE8(j, j);
    uint8_t jj2 = jj << 1;

    if(i & 0x80) jj2 = 255 - jj2;

    return jj2;
}

static
int8_t lerp7by8( int8_t a, int8_t b, uint8_t frac)
{
    int8_t result;
    if( b > a) {
        uint8_t delta = b - a;
        uint8_t scaled = SCALE8(delta, frac);
        result = a + scaled;
    } else {
        uint8_t delta = a - b;
        uint8_t scaled = SCALE8(delta, frac);
        result = a - scaled;
    }
    return result;
}

static
int8_t selectBasedOnHashBit(uint8_t hash, uint8_t bit_no, int8_t a, int8_t b)
{
	return hash & (1 << bit_no) ? a : b;
}

static
int8_t avg7(int8_t i, int8_t j)
{
    return ((i + j) >> 1) + (i & 1);
}

static
int8_t grad8(uint8_t hash, int8_t x, int8_t y, int8_t z)
{
    hash &= 0xF;

    int8_t u = selectBasedOnHashBit(hash, 3, y, x);
    int8_t v =
        hash < 4
        ? y
        : (hash == 12 || hash == 14 ? x : z);

    if(hash & 1)  u = -u;
    if(hash & 2)  v = -v;

    return avg7(u, v);
}

/* return -64..+64 */
static
int8_t inoise8(uint16_t x, uint16_t y, uint16_t z)
{
    // Find the unit cube containing the point
    uint8_t X = x >> 8;
    uint8_t Y = y >> 8;
    uint8_t Z = z >> 8;

    // Hash cube corner coordinates
    uint8_t A = P(X)+Y;
    uint8_t AA = P(A)+Z;
    uint8_t AB = P(A+1)+Z;
    uint8_t B = P(X+1)+Y;
    uint8_t BA = P(B) + Z;
    uint8_t BB = P(B+1)+Z;

    // Get the relative position of the point in the cube
    uint8_t u = x;
    uint8_t v = y;
    uint8_t w = z;

    // Get a signed version of the above for the grad function
    int8_t xx = ((uint8_t)(x)>>1) & UINT8_C(0x7F);
    int8_t yy = ((uint8_t)(y)>>1) & UINT8_C(0x7F);
    int8_t zz = ((uint8_t)(z)>>1) & UINT8_C(0x7F);

    u = ease8InOutQuad(u);
    v = ease8InOutQuad(v);
    w = ease8InOutQuad(w);

    int8_t X1 = lerp7by8(grad8(P(AA), xx, yy, zz), grad8(P(BA), xx - N(), yy, zz), u);
    int8_t X2 = lerp7by8(grad8(P(AB), xx, yy-N(), zz), grad8(P(BB), xx - N(), yy - N(), zz), u);
    int8_t X3 = lerp7by8(grad8(P(AA+1), xx, yy, zz-N()), grad8(P(BA+1), xx - N(), yy, zz-N()), u);
    int8_t X4 = lerp7by8(grad8(P(AB+1), xx, yy-N(), zz-N()), grad8(P(BB+1), xx - N(), yy - N(), zz - N()), u);
    int8_t Y1 = lerp7by8(X1,X2,v);
    int8_t Y2 = lerp7by8(X3,X4,v);
    int8_t ans = lerp7by8(Y1,Y2,w);
    return ans;
}

static
uint8_t noise8(uint16_t x, uint16_t y, uint16_t z)
{
    uint8_t n = inoise8(x, y, z) + INT8_C(64);  // (-64..+64) + 64
    return sadd8(n, n); // 0..255
}

void noise_map_update(noise_map_t *map)
{
    const map_size_t h = map->header.height;
    const map_size_t stride = map->header.stride;
    const uint16_t rx = map->rx;
    const uint16_t ry = map->ry;
    const uint16_t speed = map->speed;
    const uint16_t scale = map->scale;
    energy_t *energy = map->energy;

    for(map_size_t y = 0; y < h; y++)
    {
        const uint16_t y_offset = scale * y;

        for(map_size_t x = 0; x < stride; x++)
        {
            uint16_t x_offset = scale * x;
            MAP_XY(energy, stride, x, y) = noise8(rx + x_offset, ry + y_offset, speed);
        }
    }
    map->speed += map->speed_step;
}

void noise_rgb_map_update(rgb_map_t * rgb_map, const noise_map_t *map)
{
    if(PALETTE16_ID_INVALID == rgb_map->palette16_id.value) return;

    const map_size_t size = rgb_map->header.stride * rgb_map->header.height;
    const energy_t *energy = map->energy;
    const uint8_t brightness = rgb_map->brightness;

    for(map_size_t i = 0; i < size; ++i)
    {
        rgb_map->rgb[i] =
            palette16_color(rgb_map->palette16_id, energy[i], brightness);
    }
}

void noise_init(noise_map_t *map)
{
    map->rx = rand16();
    map->ry = rand16();
    map->speed = rand16();
#if 0
    map->speed_step = 20;
    map->scale = 30;
#endif
    memset(map->energy, 0, (uint16_t)(map->header.height * map->header.width) << 1);
}
