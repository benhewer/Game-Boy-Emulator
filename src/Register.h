#pragma once

#include <iostream>
#include <array>

enum class Flag {
    Z,
    N,
    H,
    C
};

enum class R8 {
    B,
    C,
    D,
    E,
    H,
    L,
    HL,
    A
};

constexpr std::array register8s {
    R8::B,
    R8::C,
    R8::D,
    R8::E,
    R8::H,
    R8::L,
    R8::HL,
    R8::A
};

enum class R16 {
    BC,
    DE,
    HL,
    AF,
    SP
};

constexpr std::array register16s {
    R16::BC,
    R16::DE,
    R16::HL,
    R16::SP
};

constexpr std::array register16s_stack {
    R16::BC,
    R16::DE,
    R16::HL,
    R16::AF
};