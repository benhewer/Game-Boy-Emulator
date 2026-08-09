#include <iostream>

#include "Cartridge.h"

Cartridge::Cartridge(const std::vector<uint8_t>&& data) {
    if (data.size() > rom.size()) {
        throw std::runtime_error("ROM is too large");
    }

    std::copy(data.begin(), data.end(), rom.begin());
}

uint8_t& Cartridge::get(address_t address) {
    if (address >= CARTRIDGE_ROM_LOWER && address <= CARTRIDGE_ROM_UPPER) {
        return rom[address - CARTRIDGE_ROM_LOWER];
    }

    if (address >= CARTRIDGE_RAM_LOWER && address <= CARTRIDGE_RAM_UPPER) {
        return ram[address - CARTRIDGE_RAM_LOWER];
    }
}