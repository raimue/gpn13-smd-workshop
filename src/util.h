#ifndef UTIL_H
#define UTIL_H

/**
 * Reverses bits of an 8-bit value
 */
inline uint8_t reverse8(uint8_t x) {
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
    x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
    return x;
}

#endif /* UTIL_H */
