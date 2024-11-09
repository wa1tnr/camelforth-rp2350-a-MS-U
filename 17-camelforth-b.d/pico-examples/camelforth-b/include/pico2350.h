// #include "pico/stdlib.h"
// Sat  9 Nov 21:59:48 UTC 2024
#warning included pico2350.h Sat 9 Nov 2024

#ifdef _BOARDS_PICO2_H

    #ifndef LED_PIN_MASTER
/* TODO verify pin 25 LED presence */
        #define LED_PIN_MASTER 25;
        #define NPX_PIN 19 // arbitrary external WS2812
        #define NPX_PWR_PIN -1
        #define PIP_LENGTH 4777 // untested
        #define KB_PIP_LN  2444
        #define BLINK_MS_CF 60
    #endif

    #ifndef CF_PICO_PLATFORM
        #define CF_PICO_PLATFORM "Raspberry Pi Pico RP2350 "
/* TODO flash limits 9 Nov 2024 */
        #define FLASH_LIMIT 0x1FFFFF
        #define FLASH_MASK  0x1FF000
    #endif

#endif

