
/****
 *
 * enable sections of code based on which variant:
 * RP2040, RP2350 riscv, RP2350 arm
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

/* end */
