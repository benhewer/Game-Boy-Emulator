#include "utils.h"

uint16_t mergeBytes(uint8_t msb, uint8_t lsb) {
    return static_cast<uint16_t>(msb) << 8 | lsb;
}

uint8_t lsb(uint16_t twoBytes) {
    return 0xFF & twoBytes;
}

uint8_t msb(uint16_t twoBytes) {
    return twoBytes >> 8;
}

uint8_t readBit(uint8_t byte, size_t position) {
    return (byte >> position) & 0x01;
}

void setBit(uint8_t& byte, uint8_t bit, size_t position) {
    if (bit & 0x01 == 0x00) {
        byte &= ~(0x01 << position);
    } else {
        byte |= (0x01 << position);
    }
}