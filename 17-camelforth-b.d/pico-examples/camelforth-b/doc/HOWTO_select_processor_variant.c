
/****
 *
 * enable sections of code based on which variant:
 * RP2040, RP2350 riscv, RP2350 arm
 *
 */

/*
 * modeled on:
 *
 * pico-sdk/src/rp2_common/hardware_irq/irq.c
 *
 */

int theFunctionChoices(bool enabled) {
#if defined(__riscv)
    if (enabled) {
        return 5;
    }
#elif PICO_RP2040
    if (enabled) {
        return 0;
    }
#else
    if (enabled) {
        return 3; // it is RP2350 in ARM mode
    }
#endif
    return -1;
}

/* enId, */


/*
 *
 * pico-sdk/src/rp2_common/hardware_irq/irq.c
 * LINES 78 thru 107
 *
 */

static inline void irq_set_mask_n_enabled_internal(uint n, uint32_t mask, bool enabled) {
    invalid_params_if(HARDWARE_IRQ, n * 32u >= ((NUM_IRQS + 31u) & ~31u));
#if defined(__riscv)
    if (enabled) {
        hazard3_irqarray_clear(RVCSR_MEIFA_OFFSET, 2 * n, mask & 0xffffu);
        hazard3_irqarray_clear(RVCSR_MEIFA_OFFSET, 2 * n + 1, mask >> 16);
        hazard3_irqarray_set(RVCSR_MEIEA_OFFSET, 2 * n, mask & 0xffffu);
        hazard3_irqarray_set(RVCSR_MEIEA_OFFSET, 2 * n + 1, mask >> 16);
    } else {
        hazard3_irqarray_clear(RVCSR_MEIEA_OFFSET, 2 * n, mask & 0xffffu);
        hazard3_irqarray_clear(RVCSR_MEIEA_OFFSET, 2 * n + 1, mask >> 16);
    }
#elif PICO_RP2040
    ((void)n);
    if (enabled) {
        nvic_hw->icpr = mask;
        nvic_hw->iser = mask;
    } else {
        nvic_hw->icer = mask;
    }
#else
    // >32 IRQs (well this works for the bottom 32 which is all that is passed in
    if (enabled) {
        nvic_hw->icpr[n] = mask;
        nvic_hw->iser[n] = mask;
    } else {
        nvic_hw->icer[n] = mask;
    }
#endif
}

/* end. */
