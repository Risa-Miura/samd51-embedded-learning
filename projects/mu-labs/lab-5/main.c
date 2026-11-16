#include "workbench.h"
volatile uint32_t g_capture_request=1;
static bool capture_started;
void DMAC_0_Handler(void)
{
    marker_high();
    uint8_t flags=DMAC_REGS->CHANNEL[0].DMAC_CHINTFLAG;
    DMAC_REGS->CHANNEL[0].DMAC_CHINTFLAG=flags;
    if (flags & DMAC_CHINTFLAG_TERR(1)) ++g_dma_errors;
    if (flags & DMAC_CHINTFLAG_TCMPL(1)) {
        /* One-shot chain (DESCADDR=0) has finished: DMAC no longer owns buffer. */
        ADC1_REGS->ADC_CTRLA &= ~ADC_CTRLA_ENABLE(1);
        __DMB(); ++g_dma_blocks; g_buffer_ready=1; led_toggle();
    }
    marker_low();
}
int main(void)
{
    board_init(); clock_init();
    for (;;) {
        if (g_capture_request && (!capture_started || g_buffer_ready)) {
            __disable_irq(); g_capture_request=0; g_buffer_ready=0;
            capture_started=true;
            for (unsigned i=0;i<SAMPLE_COUNT;++i) g_samples[i]=0xffffu;
            adc_init(false,true); dma_init(true,false);
            __enable_irq();
            ADC1_REGS->ADC_SWTRIG=ADC_SWTRIG_START(1);
        }
        adc_check_overrun();
    }
}
