#pragma once

#include <cstddef>
#include <array>

#include "types.h"
#include "Cartridge.h"

class MemoryMap {
private:
    static constexpr size_t WRAM_SIZE = 0x2000;
    static constexpr address_t WRAM_LOWER = 0xC000;
    static constexpr address_t WRAM_UPPER = 0xDFFF; 

    std::array<uint8_t, WRAM_SIZE> wram;

    Cartridge cartridge;

public:
    explicit MemoryMap(Cartridge cartridge);

    uint8_t& get(address_t address);
    uint8_t const read(address_t address);
    void write(address_t address, uint8_t data);
};