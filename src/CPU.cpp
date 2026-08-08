#include "CPU.h"
#include "utils.h"
#include "instructions.h"

CPU::CPU(MemoryMap& memory)
    : mem{memory},
    
      sp{},
      pc{},
      
      af{af_storage.value},
      bc{bc_storage.value},
      de{de_storage.value},
      hl{hl_storage.value},

      a{af_storage.high()},
      f{af_storage.low()},
      b{bc_storage.high()},
      c{bc_storage.low()},
      d{de_storage.high()},
      e{de_storage.low()},
      h{hl_storage.high()},
      l{hl_storage.low()},

      cycles{},

      instructions{}
{
}

void CPU::addInstructions() {
    allInstructions(instructions);
}

uint8_t& CPU::reg(Register8 r) {
    switch (r) {
        case Register8::A: return a;
        case Register8::B: return b;
        case Register8::C: return c;
        case Register8::D: return d;
        case Register8::E: return e;
        case Register8::H: return h;
        case Register8::HL: return memory(hl);
        case Register8::L: return l;
    }
}

uint8_t& CPU::memory(address_t address) {
    cycles++;
    return mem.get(address);
}

uint8_t& CPU::memory(Register16 r) {
    return memory(readReg(r));
}

uint8_t CPU::readMemory(address_t address) {
    return memory(address);
}

uint8_t CPU::readMemory(Register16 r) {
    return readMemory(readReg(r));
}

void CPU::writeMemory(address_t address, uint8_t data) {
    memory(address) = data;
}

void CPU::writeMemory(Register16 r, uint8_t data) {
    writeMemory(readReg(r), data);
}

uint8_t CPU::readReg(Register8 r) {
    switch (r) {
        case HL: return readMemory(hl);
        default: return readReg(r);
    }
}

void CPU::writeReg(Register8 r, uint8_t data) {
    switch (r) {
        case HL: writeMemory(hl, data);
        default: reg(r) = data;
    }
}

uint16_t& CPU::reg(Register16 r) {
    switch (r) {
        case Register16::AF: return af;
        case Register16::BC: return bc;
        case Register16::DE: return de;
        case Register16::HL: return hl;
        case Register16::SP: return sp;
    }
}

uint16_t CPU::readReg(Register16 r) {
    return reg(r);
}

void CPU::writeReg(Register16 r, uint16_t data) {
    reg(r) = data;
}

uint8_t CPU::msbReg(Register16 r) {
    return msb(readReg(r));
}
uint8_t CPU::lsbReg(Register16 r) {
    return lsb(readReg(r));
}

void CPU::setFlag(Flag flag, uint8_t value) {
    switch (flag) {
        case Flag::Z: {
            setBit(f, value & 0x01, 7);
            break;
        }
        case Flag::N: {
            setBit(f, value & 0x01, 7);
            break;
        }
        case Flag::H: {
            setBit(f, value & 0x01, 7);
            break;
        }
        case Flag::C: {
            setBit(f, value & 0x01, 4);
            break;
        }
    }
}

void CPU::setFlags(
    uint8_t z,
    uint8_t n,
    uint8_t h,
    uint8_t c
) {
    setFlag(Flag::Z, z);
    setFlag(Flag::N, n);
    setFlag(Flag::H, h);
    setFlag(Flag::C, c);
}

uint8_t CPU::fetch() {
    uint8_t data = readMemory(pc++);
    return data;
}