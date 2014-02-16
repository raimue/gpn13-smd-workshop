#include "shift.h"

#include <avr/io.h>

void shift_init(void) {
    SHIFT_DDR  |= SHIFT_RCK | SHIFT_SCK | SHIFT_SER;
    shift_load(0x00);
    shift_flip();
}

void shift_load(uint8_t value) {
    for (int8_t i = 7; i >= 0; i--) {
        shift_next((value & (1 << i)) >> i);
    }
}

void shift_flip(void) {
    SHIFT_PORT |=  SHIFT_RCK;
    SHIFT_PORT &= ~SHIFT_RCK;
}

void shift_next(uint8_t value) {
    // Set next bit
    if ((value & 1) != 0) {
        SHIFT_PORT |=  SHIFT_SER;
    } else {
        SHIFT_PORT &= ~SHIFT_SER;
    }

    // Shift
    SHIFT_PORT |=  SHIFT_SCK;
    SHIFT_PORT &= ~SHIFT_SCK;
}

void shift_next_flip(uint8_t value) {
    shift_next(value);
    shift_flip();
}
