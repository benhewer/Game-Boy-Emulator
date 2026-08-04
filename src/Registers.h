#include <cstdint>

struct Registers {
    uint8_t a;
    uint8_t f;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;

    uint16_t af() const {
        return (a << 8) | f;
    }

    void af(uint16_t value) {
        a = value >> 8;
        f = value & 0xFF;
    }

    uint16_t bc() const {
        return (b << 8) | c;
    }

    void bc(uint16_t value) {
        b = value >> 8;
        c = value & 0xFF;
    }

    uint16_t de() const {
        return (d << 8) | e;
    }

    void af(uint16_t value) {
        d = value >> 8;
        e = value & 0xFF;
    }

    uint16_t hl() const {
        return (h) | l;
    }

    void af(uint16_t value) {
        h = value >> 8;
        l = value & 0xFF;
    }
};