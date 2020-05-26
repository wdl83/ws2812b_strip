#include <avr/interrupt.h>

#include <drv/assert.h>
#include <drv/spi0.h>
#include <drv/tmr2.h>

#include <drv/usart0.h>
#include <drv/tlog.h>

#include <stdio.h>
#include <string.h>

#include "fx.h"
#include "ws2812b.h"

/*-----------------------------------------------------------------------------*/
/* ws2812b Timing Requirements
 *
 *           ---------          -
 *  0-code   |       |  <-T0L-> |
 *           |<-T0H->|          |
 *           -       ------------
 *           -----------        -
 *  1-code   |         |<-T1L-> |
 *           | <-T1H-> |        |
 *           -         ----------
 *
 * TH + TL = 1250ns +/- 150ns
 * T0H 0-code 350ns +/- 150ns (TMR2 PWM width) - T0H must be < 500ns!
 * T1H 1-code 900ns +/- 150ns
 * T0L 0-code 950ns +/- 150ns (should be < 9000ns)
 * T1L 1-code 350ns +/- 150ns (should be < 9000ns)
 * RES low voltage time > 50000ns (50us)
 *
 * Most important:
 * Maximum inter-edge delay, other then T0H, must not exceed 9us!
 * Maximim inter-edge delay for T0H must not exceed 500ns!  */

/*-----------------------------------------------------------------------------*/
static
void spi_complete_cb(uintptr_t user_data)
{
    ws2812b_strip_t *strip = (ws2812b_strip_t *)user_data;
    uint8_t *rgb = (uint8_t *)strip->rgb_map.rgb;

    if(strip->flags.abort || strip->rgb_size == strip->rgb_idx)
    {
        TMR2_CLK_DISABLE();
        SPI0_INT_DISABLE();
        strip->flags.updated = strip->rgb_size == strip->rgb_idx;
        strip->flags.aborted = strip->flags.abort;
        strip->rgb_idx = 0;
        return;
    }

    SPI0_WR(rgb[strip->rgb_idx]);
    /* tmr2 reset is required because SPI needs additional time for
     * data reload (this delay is not aligned to SPI clock) */
    TMR2_WR_CNTR(0);
    ++strip->rgb_idx;
}

void ws2812b_init(ws2812b_strip_t *strip)
{
    strip->flags.value = 0;
    strip->rgb_idx = 0;

    /* SPI_CLK = 16MHz (cpu_clk) / 128 (prescaler) = 125kHz ~ 8000ns (MAX delay)
     * T0H - SPI_CLK
     * T0L - SPI_CLK
     * T1H - TMR2_PWM_CLK (HI duration TMR2 period ~ 500ns)
     * T1L - SPI_CLK */

    spi0_complete_cb(spi_complete_cb, (uintptr_t)strip);

    /* switch MOSI (PB.3) & SCK (PB.5) & SS (PB.2) pins to output */
    DDRB |= M3(DDB5, DDB3, DDB2);
    SPI0_MASTER();
    SPI0_CLK_DIV_128();
    SPI0_ENABLE();

    /* PWM TIMER - see timing requirements for ws2812b
     * 1x CPU_CLK === 16MHz === 1s / 16 * 10^6 = 1000ns / 16 = 62,5ns */

    /* switch OC2B (PD.3) pin to output */
    DDRD |= M1(DDD3);

    TMR2_MODE_FAST_PWM();
    TMR2_MODE_FAST_PWM_TOP_OCRA();
    TMR2_MODE_FAST_PWM_OC2B_NON_INVERTING();

    TMR2_WR_A(63); // 62.5ns * (63+1) = 4000ns = 4us (mesured)
    TMR2_WR_B(4); // 62.5ns * (4+1) = 312.5ns (mesured)
}

void ws2812b_update(ws2812b_strip_t *strip)
{
    const uint8_t sreg = SREG;
    cli();
    strip->flags.update = 0;
    strip->flags.updated = 0;
    strip->flags.abort = 0;
    strip->flags.aborted = 0;
    SPI0_INT_ENABLE();
    TMR2_CLK_DIV_1();
    spi_complete_cb((uintptr_t)strip);
    SREG = sreg;
}

void ws2812b_clear(ws2812b_strip_t *strip)
{
    memset(strip->rgb_map.rgb, 0, strip->rgb_size);
}
