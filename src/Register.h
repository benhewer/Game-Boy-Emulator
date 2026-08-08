#include <iostream>
#include <array>

enum Flag {
    Z,
    N,
    H,
    C
};

enum Register8 {
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
    Register8::B,
    Register8::C,
    Register8::D,
    Register8::E,
    Register8::H,
    Register8::L,
    Register8::HL,
    Register8::A
};

enum Register16 {
    BC,
    DE,
    HL,
    AF,
    SP
};

constexpr std::array register16s {
    Register16::BC,
    Register16::DE,
    Register16::HL,
    Register16::SP
};

constexpr std::array register16s_stack {
    Register16::BC,
    Register16::DE,
    Register16::HL,
    Register16::AF
};