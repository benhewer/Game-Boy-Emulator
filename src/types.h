#pragma once

#include <cstdint>
#include <functional>

class CPU;

using address_t = uint16_t;
using instruction_t = std::function<void(CPU&)>;

constexpr size_t NUM_INSTRUCTIONS = 256;
using instruction_table_t = std::array<instruction_t, NUM_INSTRUCTIONS>;
