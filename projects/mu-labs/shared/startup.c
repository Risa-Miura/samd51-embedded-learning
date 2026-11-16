/* Application-only startup. The UF2 bootloader remains at 0x00000000.
 * No vendor configuration words, fuse writes, libc, heap, or Arduino runtime.
 */
#include "workbench.h"
extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;
int main(void);
void Reset_Handler(void);
void Default_Handler(void) { for (;;) { __NOP(); } }
void TC3_Handler(void) __attribute__((weak, alias("Default_Handler")));
void RTC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DMAC_0_Handler(void) __attribute__((weak, alias("Default_Handler")));
void EIC_EXTINT_4_Handler(void) __attribute__((weak, alias("Default_Handler")));

__attribute__((used, section(".vectors"), aligned(1024)))
const DeviceVectors g_vectors = {
.pvStack = &_estack,
.pfnReset_Handler = Reset_Handler,
.pfnNMI_Handler = Default_Handler,
.pfnHardFault_Handler = Default_Handler,
.pfnMemManage_Handler = Default_Handler,
.pfnBusFault_Handler = Default_Handler,
.pfnUsageFault_Handler = Default_Handler,
.pvReservedC9 = Default_Handler,
.pvReservedC8 = Default_Handler,
.pvReservedC7 = Default_Handler,
.pvReservedC6 = Default_Handler,
.pfnSVC_Handler = Default_Handler,
.pfnDebugMon_Handler = Default_Handler,
.pvReservedC3 = Default_Handler,
.pfnPendSV_Handler = Default_Handler,
.pfnSysTick_Handler = Default_Handler,
.pfnPM_Handler = Default_Handler,
.pfnMCLK_Handler = Default_Handler,
.pfnOSCCTRL_XOSC0_Handler = Default_Handler,
.pfnOSCCTRL_XOSC1_Handler = Default_Handler,
.pfnOSCCTRL_DFLL_Handler = Default_Handler,
.pfnOSCCTRL_DPLL0_Handler = Default_Handler,
.pfnOSCCTRL_DPLL1_Handler = Default_Handler,
.pfnOSC32KCTRL_Handler = Default_Handler,
.pfnSUPC_OTHER_Handler = Default_Handler,
.pfnSUPC_BODDET_Handler = Default_Handler,
.pfnWDT_Handler = Default_Handler,
.pfnRTC_Handler = RTC_Handler,
.pfnEIC_EXTINT_0_Handler = Default_Handler,
.pfnEIC_EXTINT_1_Handler = Default_Handler,
.pfnEIC_EXTINT_2_Handler = Default_Handler,
.pfnEIC_EXTINT_3_Handler = Default_Handler,
.pfnEIC_EXTINT_4_Handler = EIC_EXTINT_4_Handler,
.pfnEIC_EXTINT_5_Handler = Default_Handler,
.pfnEIC_EXTINT_6_Handler = Default_Handler,
.pfnEIC_EXTINT_7_Handler = Default_Handler,
.pfnEIC_EXTINT_8_Handler = Default_Handler,
.pfnEIC_EXTINT_9_Handler = Default_Handler,
.pfnEIC_EXTINT_10_Handler = Default_Handler,
.pfnEIC_EXTINT_11_Handler = Default_Handler,
.pfnEIC_EXTINT_12_Handler = Default_Handler,
.pfnEIC_EXTINT_13_Handler = Default_Handler,
.pfnEIC_EXTINT_14_Handler = Default_Handler,
.pfnEIC_EXTINT_15_Handler = Default_Handler,
.pfnFREQM_Handler = Default_Handler,
.pfnNVMCTRL_0_Handler = Default_Handler,
.pfnNVMCTRL_1_Handler = Default_Handler,
.pfnDMAC_0_Handler = DMAC_0_Handler,
.pfnDMAC_1_Handler = Default_Handler,
.pfnDMAC_2_Handler = Default_Handler,
.pfnDMAC_3_Handler = Default_Handler,
.pfnDMAC_OTHER_Handler = Default_Handler,
.pfnEVSYS_0_Handler = Default_Handler,
.pfnEVSYS_1_Handler = Default_Handler,
.pfnEVSYS_2_Handler = Default_Handler,
.pfnEVSYS_3_Handler = Default_Handler,
.pfnEVSYS_OTHER_Handler = Default_Handler,
.pfnPAC_Handler = Default_Handler,
.pvReserved42 = Default_Handler,
.pvReserved43 = Default_Handler,
.pvReserved44 = Default_Handler,
.pfnRAMECC_Handler = Default_Handler,
.pfnSERCOM0_0_Handler = Default_Handler,
.pfnSERCOM0_1_Handler = Default_Handler,
.pfnSERCOM0_2_Handler = Default_Handler,
.pfnSERCOM0_OTHER_Handler = Default_Handler,
.pfnSERCOM1_0_Handler = Default_Handler,
.pfnSERCOM1_1_Handler = Default_Handler,
.pfnSERCOM1_2_Handler = Default_Handler,
.pfnSERCOM1_OTHER_Handler = Default_Handler,
.pfnSERCOM2_0_Handler = Default_Handler,
.pfnSERCOM2_1_Handler = Default_Handler,
.pfnSERCOM2_2_Handler = Default_Handler,
.pfnSERCOM2_OTHER_Handler = Default_Handler,
.pfnSERCOM3_0_Handler = Default_Handler,
.pfnSERCOM3_1_Handler = Default_Handler,
.pfnSERCOM3_2_Handler = Default_Handler,
.pfnSERCOM3_OTHER_Handler = Default_Handler,
.pfnSERCOM4_0_Handler = Default_Handler,
.pfnSERCOM4_1_Handler = Default_Handler,
.pfnSERCOM4_2_Handler = Default_Handler,
.pfnSERCOM4_OTHER_Handler = Default_Handler,
.pfnSERCOM5_0_Handler = Default_Handler,
.pfnSERCOM5_1_Handler = Default_Handler,
.pfnSERCOM5_2_Handler = Default_Handler,
.pfnSERCOM5_OTHER_Handler = Default_Handler,
.pfnSERCOM6_0_Handler = Default_Handler,
.pfnSERCOM6_1_Handler = Default_Handler,
.pfnSERCOM6_2_Handler = Default_Handler,
.pfnSERCOM6_OTHER_Handler = Default_Handler,
.pfnSERCOM7_0_Handler = Default_Handler,
.pfnSERCOM7_1_Handler = Default_Handler,
.pfnSERCOM7_2_Handler = Default_Handler,
.pfnSERCOM7_OTHER_Handler = Default_Handler,
.pvReserved78 = Default_Handler,
.pvReserved79 = Default_Handler,
.pfnUSB_OTHER_Handler = Default_Handler,
.pfnUSB_SOF_HSOF_Handler = Default_Handler,
.pfnUSB_TRCPT0_Handler = Default_Handler,
.pfnUSB_TRCPT1_Handler = Default_Handler,
.pvReserved84 = Default_Handler,
.pfnTCC0_OTHER_Handler = Default_Handler,
.pfnTCC0_MC0_Handler = Default_Handler,
.pfnTCC0_MC1_Handler = Default_Handler,
.pfnTCC0_MC2_Handler = Default_Handler,
.pfnTCC0_MC3_Handler = Default_Handler,
.pfnTCC0_MC4_Handler = Default_Handler,
.pfnTCC0_MC5_Handler = Default_Handler,
.pfnTCC1_OTHER_Handler = Default_Handler,
.pfnTCC1_MC0_Handler = Default_Handler,
.pfnTCC1_MC1_Handler = Default_Handler,
.pfnTCC1_MC2_Handler = Default_Handler,
.pfnTCC1_MC3_Handler = Default_Handler,
.pfnTCC2_OTHER_Handler = Default_Handler,
.pfnTCC2_MC0_Handler = Default_Handler,
.pfnTCC2_MC1_Handler = Default_Handler,
.pfnTCC2_MC2_Handler = Default_Handler,
.pfnTCC3_OTHER_Handler = Default_Handler,
.pfnTCC3_MC0_Handler = Default_Handler,
.pfnTCC3_MC1_Handler = Default_Handler,
.pfnTCC4_OTHER_Handler = Default_Handler,
.pfnTCC4_MC0_Handler = Default_Handler,
.pfnTCC4_MC1_Handler = Default_Handler,
.pfnTC0_Handler = Default_Handler,
.pfnTC1_Handler = Default_Handler,
.pfnTC2_Handler = Default_Handler,
.pfnTC3_Handler = TC3_Handler,
.pfnTC4_Handler = Default_Handler,
.pfnTC5_Handler = Default_Handler,
.pfnTC6_Handler = Default_Handler,
.pfnTC7_Handler = Default_Handler,
.pfnPDEC_OTHER_Handler = Default_Handler,
.pfnPDEC_MC0_Handler = Default_Handler,
.pfnPDEC_MC1_Handler = Default_Handler,
.pfnADC0_OTHER_Handler = Default_Handler,
.pfnADC0_RESRDY_Handler = Default_Handler,
.pfnADC1_OTHER_Handler = Default_Handler,
.pfnADC1_RESRDY_Handler = Default_Handler,
.pfnAC_Handler = Default_Handler,
.pfnDAC_OTHER_Handler = Default_Handler,
.pfnDAC_EMPTY_0_Handler = Default_Handler,
.pfnDAC_EMPTY_1_Handler = Default_Handler,
.pfnDAC_RESRDY_0_Handler = Default_Handler,
.pfnDAC_RESRDY_1_Handler = Default_Handler,
.pfnI2S_Handler = Default_Handler,
.pfnPCC_Handler = Default_Handler,
.pfnAES_Handler = Default_Handler,
.pfnTRNG_Handler = Default_Handler,
.pfnICM_Handler = Default_Handler,
.pfnPUKCC_Handler = Default_Handler,
.pfnQSPI_Handler = Default_Handler,
.pfnSDHC0_Handler = Default_Handler,
.pfnSDHC1_Handler = Default_Handler,
};

void Reset_Handler(void)
{
    __disable_irq();
    SysTick->CTRL=0;
    for (unsigned i=0; i<8; ++i) {
        NVIC->ICER[i]=0xffffffffu;
        NVIC->ICPR[i]=0xffffffffu;
    }
    SCB->ICSR=SCB_ICSR_PENDSTCLR_Msk | SCB_ICSR_PENDSVCLR_Msk;
    SCB->VTOR=(uint32_t)&g_vectors;
    __DSB(); __ISB();
    /* Disable/invalidate CMCC before initializing RAM shared with the DMAC.
     * This deliberately prioritizes understandable ownership over caching.
     */
    MCLK_REGS->MCLK_AHBMASK |= MCLK_AHBMASK_CMCC(1);
    CMCC_REGS->CMCC_CTRL=0;
    while (CMCC_REGS->CMCC_SR & CMCC_SR_CSTS(1)) {}
    CMCC_REGS->CMCC_MAINT0=CMCC_MAINT0_INVALL(1);
    uint32_t *src=&_sidata;
    for (uint32_t *dst=&_sdata;dst<&_edata;) *dst++=*src++;
    for (uint32_t *dst=&_sbss;dst<&_ebss;) *dst++=0;
    (void)main();
    Default_Handler();
}
