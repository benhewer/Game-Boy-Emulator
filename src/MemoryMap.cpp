#include "MemoryMap.h"

MemoryMap::MemoryMap()
    : wram{}
{
}

uint8_t const MemoryMap::read(address_t address) {
    if (address >= WRAM_LOWER && address <= WRAM_UPPER) {
        return wram[address - WRAM_LOWER];
    }
}

void MemoryMap::write(address_t address, uint8_t data) {
    if (address >= WRAM_LOWER && address <= WRAM_UPPER) {
        wram[address - WRAM_LOWER] = data;
    }
}

