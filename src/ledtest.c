#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
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

void timer_init(void) {
    /* prescaler 1/64 */
    TCCR0B &= ~(1 << CS02);
    TCCR0B |=  (1 << CS01);
    TCCR0B |=  (1 << CS00);

    /* compare mode: count up, interrupt on A */
    TCCR0A &= ~((1 << WGM01) | (1 << WGM00));
    TCCR0B &= ~(1 << WGM03);
    TCCR0B |=  (1 << WGM02);

    /* reset counter */
    OCR0A = 15625; /* 1 s */
    OCR0A =  3125; /* 200 ms */
    TCNT0 = 0;

    /* enable interrupt on overflow */
    TIMSK0 |= (1 << OCIE0A);
}

inline uint8_t reverse(uint8_t x) {
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
    x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
    return x;
}

ISR(TIM0_COMPA_vect) {
    static uint8_t x = 0;
    shift_load(reverse(x));
    shift_flip();
    x++;
}

void main(void) {
    shift_init();
    timer_init();

    sei();

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

    /*
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
    */

    // Do nothing, just sleep and wait for interrupts
    while(1) {
        sleep_enable();
        cli();
        while (1) {
            sei();
            sleep_cpu();
            cli();
        }
        sei();
        sleep_disable();
    }
}
