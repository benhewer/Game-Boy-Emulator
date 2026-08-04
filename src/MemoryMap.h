#include <cstddef>
#include <array>

#include "types.h"

constexpr size_t WRAM_SIZE = 0x2000;
constexpr address_t WRAM_LOWER = 0xC000;
constexpr address_t WRAM_UPPER = 0xDFFF; 

class MemoryMap {
private:
    std::array<uint8_t, WRAM_SIZE> wram;

public:
    MemoryMap();

    uint8_t const read(address_t address);
    void write(address_t address, uint8_t data);
};