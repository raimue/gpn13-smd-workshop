#ifndef HWCONFIG_H
#define HWCONFIG_H

#include <avr/io.h>

// Hardware definition
#define SHIFT_DDR  DDRB
#define SHIFT_PORT PORTB
#define SHIFT_RCK  (1 << PB2)
#define SHIFT_SCK  (1 << PB1)
#define SHIFT_SER  (1 << PB0)

#endif /* HWCONFIG_H */
