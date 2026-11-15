/* Bare-metal blink for the Adafruit Grand Central M4 Express (ATSAMD51P20A).
 * The ordinary D13 LED is PB01; this does not drive the NeoPixel.
 * XC32 supplies the reset/vector-table and C runtime startup from the DFP.
 */
#include <xc.h>
#include <stdint.h>

#if !defined(__SAMD51P20A__)
#error "Select ATSAMD51P20A for this project"
#endif

#define LED_MASK (1UL << 1)
#define CPU_HZ 48000000UL

static void clock_init(void)
{
    /* Use the internal DFLL in open-loop mode; no external crystal required.
     * This example is intended for reset entry, not a bootloader handoff.
     */
    OSCCTRL_REGS->OSCCTRL_DFLLCTRLA |= OSCCTRL_DFLLCTRLA_ENABLE(1);
    while (OSCCTRL_REGS->OSCCTRL_DFLLSYNC & OSCCTRL_DFLLSYNC_ENABLE(1)) {}
    OSCCTRL_REGS->OSCCTRL_DFLLCTRLB = 0;
    while (OSCCTRL_REGS->OSCCTRL_DFLLSYNC & OSCCTRL_DFLLSYNC_DFLLCTRLB(1)) {}
    while (!(OSCCTRL_REGS->OSCCTRL_STATUS & OSCCTRL_STATUS_DFLLRDY(1))) {}
    GCLK_REGS->GCLK_GENCTRL[0] = GCLK_GENCTRL_SRC_DFLL
        | GCLK_GENCTRL_GENEN(1) | GCLK_GENCTRL_DIV(1);
    while (GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL_GCLK0) {}
    MCLK_REGS->MCLK_CPUDIV = MCLK_CPUDIV_DIV_DIV1;
}

static void delay_ms(uint32_t milliseconds)
{
    while (milliseconds-- != 0U) {
        /* Poll SysTick at 1 ms; no interrupt handler is needed. */
        while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0U) {}
    }
}

int main(void)
{
    clock_init();
    PORT_REGS->GROUP[1].PORT_PINCFG[1] = 0;
    PORT_REGS->GROUP[1].PORT_OUTCLR = LED_MASK;
    PORT_REGS->GROUP[1].PORT_DIRSET = LED_MASK;

    SysTick->LOAD = CPU_HZ / 1000UL - 1UL;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    for (;;) {
        PORT_REGS->GROUP[1].PORT_OUTTGL = LED_MASK;
        delay_ms(500);
    }
}
