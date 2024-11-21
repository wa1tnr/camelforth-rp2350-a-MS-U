// Thu Nov 21 19:54:31 UTC 2024
// was: Wed Apr 14 20:45:28 UTC 2021
// wa1tnr
// camelforth

/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/flash.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "tusb.h"
#include <stdio.h>
#include <stdlib.h>

#define FLASH_TARGET_OFFSET_B 0x1E0000

const uint8_t *flash_target_contents_b =
    (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET_B);

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1
extern void interpreter(void);
extern void _this_ws2812(void);
extern void crufty_printer(void);

extern void _pico_LED_init(void);
extern void _pico_pip(void);
extern void _pico_LED(void);

void _loop_delay_local(void) {
    if (tud_cdc_n_connected(0))
        return;
    for (volatile int i = 288; i > 0; i--) {
        for (volatile int j = 455555; j > 0; j--) {
        }
        if (tud_cdc_n_connected(0))
            return;
    }
}

void blink_loop(void) {
    _pico_pip();
    _loop_delay_local();
}

int main(void) {
    sleep_ms(1100);
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    stdio_init_all();
    sleep_ms(600);
    _pico_LED_init();
    sleep_ms(140);
    for (int i = 3; i > 0; i--)
        _pico_LED();
    while (!tud_cdc_n_connected(0)) {
        blink_loop();
    }
    for (int i = 3; i > 0; i--)
        _pico_LED();
    uart_puts(UART_ID, "\n   camelforth-rp2350-a-MS-U r0.2.0-pre-alpha\n");
    printf(            "\n   camelforth-rp2350-a-MS-U r0.2.0-pre-alpha\n");

    uart_puts(UART_ID, "        +fl_sizing +alltargets +itsybitsy +blinkwait +feather\n");
    printf(            "        +fl_sizing +alltargets +itsybitsy +blinkwait +feather\n");

    uart_puts(UART_ID, "        +no_emit +auto_load +rewind +flaccept +erase +flwrite\n");
    printf(            "        +no_emit +auto_load +rewind +flaccept +erase +flwrite\n");

    uart_puts(UART_ID, "        +rp2350 +reflash +dump +blink +UART +USB\n");
    printf(            "        +tidy +bcde +rp2350 +reflash +dump +blink +UART +USB \n");

    crufty_printer();

    _this_ws2812();

    printf("      NEOPIX activity here\n");
    printf("\n");

// kludge: bug with flash access in no_flash binary compile (see CMakeLists.txt
// for the toggle)
// #undef NO_FLASH_CMAKE
#ifdef NO_FLASH_CMAKE
    flash_range_erase(FLASH_TARGET_OFFSET_B, FLASH_SECTOR_SIZE);
    printf("   flash_range_erase is required (and completed).\n\n");
#endif

#define WANT_FORCED_ERASE_QTPY
#undef WANT_FORCED_ERASE_QTPY
#ifdef WANT_FORCED_ERASE_QTPY
    flash_range_erase(FLASH_TARGET_OFFSET_B, FLASH_SECTOR_SIZE);
    printf("   flash_range_erase is required (and completed).\n\n");
#endif // #ifdef WANT_FORCED_ERASE_QTPY
    while(1) {
        interpreter(); // camelforth
    }
}

// END.
