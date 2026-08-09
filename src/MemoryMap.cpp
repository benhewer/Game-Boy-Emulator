#include <iostream>

#include "MemoryMap.h"

MemoryMap::MemoryMap(Cartridge cartridge)
    : wram{},
      cartridge{cartridge}
{
}

uint8_t& MemoryMap::get(address_t address) {
    if (address >= CARTRIDGE_ROM_LOWER && address <= CARTRIDGE_ROM_UPPER) {
        return cartridge.get(address);
    }

    if (address >= WRAM_LOWER && address <= WRAM_UPPER) {
        return wram[address - WRAM_LOWER];
    }

    if (address >= CARTRIDGE_RAM_LOWER && address <= CARTRIDGE_RAM_UPPER) {
        return cartridge.get(address);
    }

    throw std::out_of_range("Invalid memory address (for now)");
}

uint8_t const MemoryMap::read(address_t address) {
    return get(address);
}

void MemoryMap::write(address_t address, uint8_t data) {
    get(address) = data;
}

