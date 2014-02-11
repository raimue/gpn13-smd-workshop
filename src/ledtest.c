#include <avr/io.h>
#include <util/delay.h>

#define SHIFT_DDR  DDRB
#define SHIFT_PORT PORTB
#define SHIFT_RCK  (1 << PB2)
#define SHIFT_SCK  (1 << PB1)
#define SHIFT_SER  (1 << PB0)

void shift_next(uint8_t value) {
    if ((value & 1) != 0) {
        PORTB |=  SHIFT_SER;
    } else {
        PORTB &= ~SHIFT_SER;
    }

    // Shift
    PORTB |=  SHIFT_SCK;
    PORTB &= ~SHIFT_SCK;
}

void shift_load(uint8_t value) {
    for (int8_t i = 7; i >= 0; i--) {
        shift_next((value & (1 << i)) >> i);
    }
}

void shift_flip(void) {
    // Flip buffers
    PORTB |=  SHIFT_RCK;
    PORTB &= ~SHIFT_RCK;
}

void shift_next_flip(uint8_t value) {
    shift_next(value);
    shift_flip();
}

void shift_init(void) {
    SHIFT_DDR  |= SHIFT_RCK | SHIFT_SCK | SHIFT_SER;
    shift_load(0x00);
    shift_flip();
}

void delay(uint16_t value) {
    for (uint16_t i = 0; i < value; i++) {
        _delay_us(10);
    }
}

void main(void) {
    shift_init();

    /*
    // LED dimming
    while(1) {
        shift_load(0xff);
        shift_flip();
        for (volatile uint16_t i = 0; i < 1; i++);
        shift_load(0x00);
        shift_flip();
        for (volatile uint16_t i = 0; i < 500; i++);
    }
    */

    // LED sequential on/off
    while(1) {
        for (uint8_t i = 0; i < 8; i++) {
            shift_next_flip(1);
            delay(1000);
        }
        for (uint8_t i = 0; i < 8; i++) {
            shift_next_flip(0);
            delay(1000);
        }
    }
}
