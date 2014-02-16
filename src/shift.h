#ifndef SHIFT_H
#define SHIFT_H

#include "hwconfig.h"

#ifndef SHIFT_DDR
#error Missing hardware definition for SHIFT_DDR
#endif
#ifndef SHIFT_PORT
#error Missing hardware definition for SHIFT_PORT
#endif
#ifndef SHIFT_RCK
#error Missing hardware definition for SHIFT_RCK
#endif
#ifndef SHIFT_SCK
#error Missing hardware definition for SHIFT_SCK
#endif
#ifndef SHIFT_SER
#error Missing hardware definition for SHIFT_SER
#endif

/**
 * Initializes shift register
 *
 * This function has to be called once before the other functions of this
 * module can be used. All output pins are set to 0.
 */
void shift_init(void);

/**
 * Loads 8-bit value into the register
 */
void shift_load(uint8_t value);

/**
 * Switches buffers, sets output level
 */
void shift_flip(void);

/**
 * Shifts the next bit into the register
 */
void shift_next(uint8_t value);

/**
 * Shifts the next bit into the register and switches buffers
 */
void shift_next_flip(uint8_t value);

/**
 * Loads 8-bit value into the register and switches buffers
 */
void shift_set(uint8_t value);

#endif /* SHIFT_H */
