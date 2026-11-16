#include "workbench.h"
volatile uint32_t g_rtc_matches;
void RTC_Handler(void)
{
    marker_high();
    if (RTC_REGS->MODE0.RTC_INTFLAG & RTC_MODE0_INTFLAG_CMP0(1)) {
        RTC_REGS->MODE0.RTC_INTFLAG=RTC_MODE0_INTFLAG_CMP0(1)|RTC_MODE0_INTFLAG_OVF(1);
        ++g_rtc_matches; led_toggle();
    }
    marker_low();
}
int main(void)
{
    board_init(); clock_init(); rtc_init(true,false); __enable_irq();
    for (;;) __WFI();
}
