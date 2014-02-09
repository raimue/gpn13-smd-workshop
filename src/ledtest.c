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

void delay(void) {
    for (uint16_t i = 0; i < 10000; i++) {
        _delay_us(10);
    }
    /* for (volatile uint16_t i = 0; i < 30000; i++); */
}

void main(void) {
    shift_init();

    while(1) {
        uint16_t value = 0x01;
        for (uint8_t i = 0; i < 8; i++) {
            shift_load(value);
            shift_flip();
            delay();
            value <<= 1;
        }
    }
}
