// rp2350_reading.inc
// Fri Nov 15 21:38:10 UTC 2024

// Read keyboard (or ascii upload, using picocom / ascii-xfr).
// Store in ram buffer, sized 4095 bytes,
// including a terminating NULL ('\0') character.

#define READING_SIZE 4096

char buffer[READING_SIZE];

bool noFlip = false;

int flipper(int flip) {
    const uint LED_PIN = 25;
    gpio_set_dir(LED_PIN, GPIO_OUT);

    if (noFlip) {
        return flip; // early!  TODO: does it matter what flip holds now?
    }

    if (flip == 0) {
        sleep_ms(1);
        gpio_put(LED_PIN, 1);
    }

    if (flip == -1) {
        sleep_ms(1);
        gpio_put(LED_PIN, 0);
    }

    int flop = flip;

    if (flop == -1) {
        flip = 0;
    }

    if (flop == 0) {
        flip = -1;
    }
    return flip;
}

#warning cool thing happening rp2350_reading.inc  LINE 44

/* blipLED() appears to mostly work if not entirely. */

/*
 the count increases by one for each ascii-xfer char seen,
   though it is unclear why there is an apparent one to one
   relationship there.
 */

void blipLED() {
    char buffer[32];
    int count = cpop();
    int flip = cpop();
    count++;

#if 0
    snprintf(buffer, sizeof(buffer), "\nsaid 36: count: %d\n", count);
    printf(buffer);
#endif

    /* fix indent here: */

    if (count == 0x3F) {
        count = 0;
        flip = flipper(flip);
    }

    cpush(flip);
    cpush(count);
}

CODE(reading) {
    int bufpos = 0;
    int count = 0;
    int flip = -1;
    uint8_t ch_read, ch_test;
    uint32_t location = (uint32_t)&buffer[0];
    --psp;
    psp[0] = (uint32_t)location; // push
    for (int i = (READING_SIZE - 1); i > 0; i--) {
        do {
            cpush(flip);
            cpush(count);
            blipLED();
            count = cpop();
            flip = cpop();
            ch_read = getchar_timeout_us(18);
        } while ((ch_read == '\0') || (ch_read == 0xff) || (ch_read == 0xfe));

        ch_test = ch_read;
        if (((ch_test > 0x1F) && (ch_test < 0x7F)) ||
            ((ch_test == '\010') || (ch_test == '\033') || (ch_test == '\t') ||
             (ch_test == '\n'))) {
            if (ch_read == '\033') {
                buffer[bufpos++] = '\0';
                noFlip = true; // bypass flag
                printf(" ESC! ");
                return;
            }
            buffer[bufpos++] = ch_read;
            putch((char)ch_read);
        }
    }
    buffer[bufpos] = '\0';
    putch('f');
    putch('f');
    putch(' ');
    putch('O');
    putch('K');
    putch(' ');
}

// END.
