#include "workbench.h"
volatile uint32_t g_ms, g_led_toggles, g_button_pressed;
static uint16_t stable_low_ms, blink_ms;
static bool led;
void TC3_Handler(void)
{
    marker_high();
    if (TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC0(1)) {
        TC3_REGS->COUNT16.TC_INTFLAG=TC_INTFLAG_MC0(1)|TC_INTFLAG_OVF(1);
        ++g_ms;
        if (!(PORT_REGS->GROUP[2].PORT_IN & BUTTON_MASK)) {
            if (stable_low_ms<50) ++stable_low_ms;
        } else if (stable_low_ms) --stable_low_ms;
        if (stable_low_ms==50) g_button_pressed=1;
        if (stable_low_ms==0) g_button_pressed=0;
        if (!LAB_BUTTON_GATED || g_button_pressed) {
            if (++blink_ms==500) { blink_ms=0; led=!led; ++g_led_toggles; }
        } else { blink_ms=0; led=true; }
        if (led) {
            PORT_REGS->GROUP[1].PORT_OUTSET=LED_MASK;
            PORT_REGS->GROUP[1].PORT_OUTCLR=MIRROR_MASK;
        } else {
            PORT_REGS->GROUP[1].PORT_OUTCLR=LED_MASK;
            PORT_REGS->GROUP[1].PORT_OUTSET=MIRROR_MASK;
        }
    }
    marker_low();
}
int main(void)
{
    board_init(); clock_init(); tc3_init(); __enable_irq();
    for (;;) __WFI();
}
