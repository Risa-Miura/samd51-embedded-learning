#ifndef WORKBENCH_H
#define WORKBENCH_H
#include <samd51p20a.h>
#include <stdint.h>
#include <stdbool.h>

#if !defined(__SAMD51P20A__)
#error "These labs require ATSAMD51P20A, not SAME54 or another SAMD51 package."
#endif
#ifndef LAB_CLOSED_LOOP
#define LAB_CLOSED_LOOP 0
#endif
#ifndef LAB_BUTTON_GATED
#define LAB_BUTTON_GATED 0
#endif

#define CPU_NOMINAL_HZ 48000000u
#define LED_MASK (1u << 1)       /* D13 = PB01, ordinary onboard LED, active high. */
#define MIRROR_MASK (1u << 0)    /* D12 = PB00, complementary digital output. */
#define BUTTON_MASK (1u << 4)    /* D48 = PC04, input / EXTINT4. */
#define MARKER_MASK (1u << 19)   /* D34 = PA19, ISR/block marker. */
#define SAMPLE_COUNT 1000u

extern volatile uint32_t g_fault_stage;
extern volatile uint32_t g_dma_blocks, g_dma_errors, g_adc_overruns;
extern volatile uint32_t g_buffer_ready;
extern volatile uint16_t g_samples[SAMPLE_COUNT];
extern dmac_descriptor_registers_t g_descriptor;
extern volatile dmac_descriptor_registers_t g_writeback;

void wait32(volatile const uint32_t *reg, uint32_t mask, bool set, uint32_t stage);
void board_init(void);
void pin_mux(unsigned group, unsigned pin, unsigned function);
void clock_init(void);
void tc3_init(void);
void rtc_init(bool interrupt, bool event);
void rtc_set_period(uint32_t ticks);
void adc_init(bool event_driven, bool twelve_bit);
void pwm_init(void);
void dma_init(bool to_memory, bool event_output);
void events_init(void);
void eic_init(void);
void adc_check_overrun(void);
static inline void led_toggle(void) { PORT_REGS->GROUP[1].PORT_OUTTGL=LED_MASK; }
static inline void marker_high(void) { PORT_REGS->GROUP[0].PORT_OUTSET=MARKER_MASK; }
static inline void marker_low(void) { PORT_REGS->GROUP[0].PORT_OUTCLR=MARKER_MASK; }
#endif
