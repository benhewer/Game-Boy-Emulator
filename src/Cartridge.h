#pragma once

#include <cstdint>

#include "types.h"

constexpr size_t CARTRIDGE_RAM_SIZE = 0X2000;
constexpr address_t CARTRIDGE_RAM_LOWER = 0xA000;
constexpr address_t CARTRIDGE_RAM_UPPER = 0xBFFF;

constexpr size_t CARTRIDGE_ROM_SIZE = 0x8000;
constexpr address_t CARTRIDGE_ROM_LOWER = 0x0000;
constexpr address_t CARTRIDGE_ROM_UPPER = 0x7FFF;

class Cartridge {
private:
    std::array<uint8_t, CARTRIDGE_ROM_SIZE> rom;
    std::array<uint8_t, CARTRIDGE_RAM_SIZE> ram;

public:
    explicit Cartridge(const std::vector<uint8_t>&& rom);

    uint8_t& get(address_t address);
};