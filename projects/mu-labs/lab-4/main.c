#include "workbench.h"
volatile uint32_t g_rtc_matches, g_button_events, g_change_period;
static bool fast;
void RTC_Handler(void)
{
    if (RTC_REGS->MODE0.RTC_INTFLAG & RTC_MODE0_INTFLAG_CMP0(1)) {
        RTC_REGS->MODE0.RTC_INTFLAG=RTC_MODE0_INTFLAG_CMP0(1)|RTC_MODE0_INTFLAG_OVF(1);
        ++g_rtc_matches;
        led_toggle();
    }
}
void DMAC_0_Handler(void)
{
    marker_high();
    uint8_t flags=DMAC_REGS->CHANNEL[0].DMAC_CHINTFLAG;
    DMAC_REGS->CHANNEL[0].DMAC_CHINTFLAG=flags;
    if (flags & DMAC_CHINTFLAG_TERR(1)) {
        ++g_dma_errors;
        DMAC_REGS->CHANNEL[0].DMAC_CHCTRLA &= ~DMAC_CHCTRLA_ENABLE(1);
    } else if (flags & DMAC_CHINTFLAG_SUSP(1)) {
        ++g_dma_blocks;
        __DMB();
        DMAC_REGS->CHANNEL[0].DMAC_CHCTRLB=DMAC_CHCTRLB_CMD_RESUME;
    }
    marker_low();
}
void EIC_EXTINT_4_Handler(void)
{
    EIC_REGS->EIC_INTFLAG=EIC_INTFLAG_EXTINT(1u<<4);
    ++g_button_events;
    g_change_period=1; /* Move synchronized RTC writes out of the ISR. */
}
int main(void)
{
    board_init(); clock_init(); pwm_init();
    events_init(); eic_init();
    rtc_init(false,true);
    adc_init(true,false); dma_init(false,true);
    __enable_irq();
    ADC1_REGS->ADC_SWTRIG=ADC_SWTRIG_START(1); /* Seed conversion, also in Lab 4. */
    for (;;) {
        adc_check_overrun();
        if (g_change_period) {
            __disable_irq(); g_change_period=0; __enable_irq();
            fast=!fast; rtc_set_period(fast?8192:32768);
        }
        /* Running poll avoids claiming CPU sleep/utilization measurements. */
    }
}
