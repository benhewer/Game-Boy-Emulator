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

      ticks{},

      instructions{}
{
    addInstructions();
}

void CPU::addInstructions() {
    allInstructions(instructions);
}

uint8_t& CPU::reg(R8 r) {
    switch (r) {
        case R8::A: return a;
        case R8::B: return b;
        case R8::C: return c;
        case R8::D: return d;
        case R8::E: return e;
        case R8::H: return h;
        case R8::HL: return memory(hl);
        case R8::L: return l;
    }

    throw std::out_of_range("Register8 doesn't exist");
}

uint8_t& CPU::memory(address_t address) {
    // TODO: eventually, could call some cycle function here
    //       to make other things happen to simulate waiting

    ticks += TICKS_PER_MEM_ACCESS;
    return mem.get(address);
}

uint8_t& CPU::memory(R16 r) {
    return memory(readReg(r));
}

uint8_t CPU::readMemory(address_t address) {
    return memory(address);
}

uint8_t CPU::readMemory(R16 r) {
    return readMemory(readReg(r));
}

void CPU::writeMemory(address_t address, uint8_t data) {
    memory(address) = data;
}

void CPU::writeMemory(R16 r, uint8_t data) {
    writeMemory(readReg(r), data);
}

uint8_t CPU::readReg(R8 r) {
    switch (r) {
        case R8::HL: return readMemory(hl);
        default: return readReg(r);
    }
}

void CPU::writeReg(R8 r, uint8_t data) {
    switch (r) {
        case R8::HL: writeMemory(hl, data);
        default: reg(r) = data;
    }
}

uint16_t& CPU::reg(R16 r) {
    switch (r) {
        case R16::AF: return af;
        case R16::BC: return bc;
        case R16::DE: return de;
        case R16::HL: return hl;
        case R16::SP: return sp;
    }

    throw std::out_of_range("Register16 doesn't exist");
}

uint16_t CPU::readReg(R16 r) {
    return reg(r);
}

void CPU::writeReg(R16 r, uint16_t data) {
    reg(r) = data;
}

uint8_t CPU::msbReg(R16 r) {
    return msb(readReg(r));
}
uint8_t CPU::lsbReg(R16 r) {
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

// TODO fetch execute overlap
//      (for when my cycles model is more accurate)
void CPU::tick() {
    ticks++;
    if (ticks < TICKS_PER_MCYCLE) {
        return;
    }
    ticks = 0;

    uint8_t opcode = fetch();
    instruction_t instruction = instructions[opcode];

    if (!instruction) {
        std::cerr << "Unimplemented opcode: "
                  << std::hex << static_cast<int>(opcode)
                  << std::endl;
    }

    instruction(*this);
}