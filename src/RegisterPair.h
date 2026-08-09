#pragma once

#include <cstdint>

struct RegisterPair {
    uint16_t value{};

    uint8_t& high() {
        return reinterpret_cast<uint8_t*>(&value)[1];
    }

    uint8_t& low() {
        return reinterpret_cast<uint8_t*>(&value)[0];
    }
};