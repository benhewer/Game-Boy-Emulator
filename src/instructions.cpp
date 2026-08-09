#include "instructions.h"
#include "CPU.h"
#include "utils.h"

instruction_t sequence(instruction_t a, instruction_t b) {
    return [a, b](CPU& cpu) {
        a(cpu);
        b(cpu);
    };
}

void halt(CPU& cpu) {
    // TODO HALT
}

void nop(CPU& cpu) {
    // Do nothing
}

instruction_t ldRegReg(R8 dst, R8 src) {
    return [dst, src](CPU& cpu) {
        cpu.writeReg(dst, cpu.readReg(src));
    };
}

// assign load register into register instructions
void ldRegRegs(instruction_table_t& instructions) {
    // first load reg, ld b b has opcode 0x40
    uint8_t opcode = 0x40;
    for (auto dst : register8s) {
        for (auto src : register8s) {
            if (dst == R8::HL && src == R8::HL) {
                instructions[opcode] = halt;
            } else {
                instructions[opcode] = ldRegReg(dst, src);
            }
            opcode++;
        }
    }
}

instruction_t ldRegImm8(R8 dst) {
    return [dst](CPU& cpu) {
        uint8_t data = cpu.fetch();
        cpu.writeReg(dst, data);
    };
}

// assign load immediate 8-bit value into register instructions
void ldRegImm8s(instruction_table_t& instructions) {
    // first load imm8, ld b n8 has opcode 0x06
    uint8_t opcode = 0x06;
    for (auto dst : register8s) {
        instructions[opcode] = ldRegImm8(dst);
        // each ld imm8 instruction is 8 apart
        opcode += 0x08;
    }
}

instruction_t ldRegImm16(R16 dst) {
    return [dst](CPU& cpu) {
        uint8_t lsb = cpu.fetch();
        uint8_t msb = cpu.fetch();
        uint16_t data = mergeBytes(msb, lsb);
        cpu.writeReg(dst, data);
    };
}

// load immediate 16-bit value into register
void ldRegImm16s(instruction_table_t& instructions) {
    // first load imm16, ld bc n16 has opcode 0x01
    uint8_t opcode = 0x01;
    for (auto dst : register16s) {
        instructions[opcode] = ldRegImm16(dst);
        // each ld imm16 instruction is 0x10 apart
        opcode += 0x10;
    }
}

instruction_t ldIndReg(R16 dst, R8 src) {
    return [dst, src](CPU& cpu) {
        cpu.writeMemory(dst, cpu.readReg(src));
    };
}

instruction_t incReg16(R16 reg) {
    return [reg](CPU& cpu) {
        cpu.reg(reg)++;
    };
}

instruction_t decReg16(R16 reg) {
    return [reg](CPU& cpu) {
        cpu.reg(reg)--;
    };
}

// assign load a into indirect address instructions
void ldIndAs(instruction_table_t& instructions) {
    // first load a, ld [bc] a has opcode 0x02
    uint8_t opcode = 0x02;
    instructions[opcode] = ldIndReg(R16::BC, R8::A);
    // each ld a instruction is 0x10 apart
    opcode += 0x10;
    instructions[opcode] = ldIndReg(R16::DE, R8::A);
    opcode += 0x10;
    // ld [hl+] a, hl is incremented
    instructions[opcode] = sequence(
        ldIndReg(R16::HL, R8::A),
        incReg16(R16::HL)
    );
    opcode += 0x10;
    // ld [hl-] a, hl is decremented
    instructions[opcode] = sequence(
        ldIndReg(R16::HL, R8::A),
        decReg16(R16::HL)
    );
}

instruction_t ldRegInd(R8 dst, R16 src) {
    return [dst, src](CPU& cpu) {
        cpu.writeReg(dst, cpu.readMemory(src));
    };
}

// assign load indirect address into a instructions
void ldAInds(instruction_table_t& instructions) {
    // first load ind, ld a [bc] has opcode 0x0A
    uint8_t opcode = 0x0A;
    instructions[opcode] = ldRegInd(R8::A, R16::BC);
    // each ld ind instruction is 0x10 apart
    opcode += 0x10;
    instructions[opcode] = ldRegInd(R8::A, R16::DE);
    opcode += 0x10;
    // ld a [hl+], hl is incremented
    instructions[opcode] = sequence(
        ldRegInd(R8::A, R16::HL),
        incReg16(R16::HL)
    );
    opcode += 0x10;
    // ld a [hl-], hl is decremented
    instructions[opcode] = sequence(
        ldRegInd(R8::A, R16::HL),
        decReg16(R16::HL)
    );
}

instruction_t ldRegDir16(R8 dst) {
    return [dst](CPU& cpu) {
        uint8_t lsb = cpu.fetch();
        uint8_t msb = cpu.fetch();
        address_t address = mergeBytes(msb, lsb);
        cpu.writeReg(dst, cpu.readMemory(address));
    };
}

instruction_t ldDir16Reg(R8 src) {
    return [src](CPU& cpu) {
        uint8_t lsb = cpu.fetch();
        uint8_t msb = cpu.fetch();
        address_t address = mergeBytes(msb, lsb);
        cpu.writeMemory(address, cpu.readReg(src));
    };
}

instruction_t ldhRegInd(R8 dst, R8 src) {
    return [dst, src](CPU& cpu) {
        address_t address = mergeBytes(0xFF, cpu.readReg(src));
        cpu.writeReg(dst, address);
    };
}

instruction_t ldhIndReg(R8 dst, R8 src) {
    return [dst, src](CPU& cpu) {
        address_t address = mergeBytes(0xFF, cpu.readReg(dst));
        cpu.writeMemory(address, cpu.readReg(src));
    };
}

instruction_t ldhRegDir8(R8 dst) {
    return [dst](CPU& cpu) {
        uint8_t data = cpu.fetch();
        address_t address = mergeBytes(0xFF, data);
        cpu.writeReg(dst, cpu.readMemory(address));
    };
}

instruction_t ldhDir8Reg(R8 src) {
    return [src](CPU& cpu) {
        uint8_t data = cpu.fetch();
        address_t address = mergeBytes(0xFF, data);
        cpu.writeMemory(address, cpu.readReg(src));
    };
}

// 8-bit load instructions: https://gbdev.io/gb-opcodes/optables/
void ld8Instrs(instruction_table_t& instructions) {
    // top 4 rows 
    ldIndAs(instructions);
    ldRegImm8s(instructions);
    ldAInds(instructions);

    // big 4 row block - this also adds halt
    ldRegRegs(instructions);

    // 2 rows at the bottom

    // ld [a16] a
    instructions[0xEA] = ldDir16Reg(R8::A);
    // ld a [a16]
    instructions[0xFA] = ldRegDir16(R8::A);

    // ldh [c] a
    instructions[0xE2] = ldhIndReg(R8::C, R8::A);
    // ldh a [c]
    instructions[0xF2] = ldhRegInd(R8::A, R8::C);

    // ldh [a8] a
    instructions[0xE0] = ldhDir8Reg(R8::A);
    // ldh a [a8]
    instructions[0xF0] = ldhRegDir8(R8::A);
}

instruction_t ldDir16Reg(R16 src) {
    return [src](CPU& cpu) {
        uint8_t _lsb = cpu.fetch();
        uint8_t _msb = cpu.fetch();
        address_t address = mergeBytes(_msb, _lsb);
        cpu.writeMemory(address, cpu.lsbReg(src));
        cpu.writeMemory(address + 1, cpu.msbReg(src));
    };
}

instruction_t ldRegReg(R16 dst, R16 src) {
    return [dst, src](CPU& cpu) {
        cpu.writeReg(dst, cpu.readReg(src));
    };
}

instruction_t pushReg(R16 src) {
    return [src](CPU& cpu) {
        cpu.writeMemory(--cpu.sp, cpu.msbReg(src));
        cpu.writeMemory(--cpu.sp, cpu.lsbReg(src));
    };
}

void pushRegs(instruction_table_t& instructions) {
    // first push reg, push bc has opcode 0xC5
    uint8_t opcode = 0xC5;
    for (auto src : register16s) {
        instructions[opcode] = pushReg(src);
        // each push is 0x10 apart
        opcode += 0x10;
    }
}

instruction_t popReg(R16 dst) {
    return [dst](CPU& cpu) {
        uint8_t lsb = cpu.readMemory(cpu.sp++);
        uint8_t msb = cpu.readMemory(cpu.sp++);
        uint16_t data = mergeBytes(msb, lsb);
        cpu.writeReg(dst, data);
    };
}

void popRegs(instruction_table_t& instructions) {
    // first pop reg, pop bc has opcode 0xC1
    uint8_t opcode = 0xC1;
    for (auto dst : register16s) {
        instructions[opcode] = popReg(dst);
        // each pop is 0x10 apart
        opcode += 0x10;
    }
}

// Load dst from adjusted src
instruction_t ldRegRegE8(R16 dst, R16 src) {
    return [dst, src](CPU& cpu) {
        uint8_t offset = cpu.fetch();
        uint8_t srcLow = cpu.readReg(src) & 0xFF;

        // check for bit carries
        cpu.setFlag(Flag::H, (srcLow & 0xF) + (offset & 0xF) > 0xF);
        cpu.setFlag(Flag::C, srcLow + offset > 0xFF);

        int8_t e8 = static_cast<int8_t>(offset);
        cpu.writeReg(dst, cpu.readReg(src) + e8);
    };
}

// 16-bit load instructions
void ld16Instrs(instruction_table_t& instructions) {
    // top 4 rows in x1 column
    ldRegImm16s(instructions);

    // ld [a16] sp
    instructions[0x08] = ldDir16Reg(R16::SP);

    // ld sp hl
    instructions[0xF9] = ldRegReg(R16::SP, R16::HL);

    // bottom 4 rows in x5 column
    pushRegs(instructions);

    // bottom 4 rows in x1 column
    popRegs(instructions);

    // ld hl sp+e8
    instructions[0xF8] = ldRegRegE8(R16::HL, R16::SP);
}

using reg_reg_op8_t = uint8_t (*)(CPU&, uint8_t, uint8_t);
using reg_op8_t = uint8_t (*)(CPU&, uint8_t);

instruction_t op8RegReg(reg_reg_op8_t op, R8 dst, R8 src) {
    return [op, dst, src](CPU& cpu) {
        cpu.writeReg(
            dst,
            op(cpu, cpu.readReg(dst), cpu.readReg(src))
        );
    };
}

instruction_t op8Reg(reg_op8_t op, R8 reg) {
    return [op, reg](CPU& cpu) {
        cpu.writeReg(
            reg,
            op(cpu, cpu.readReg(reg))
        );
    };
}

instruction_t op8RegImm(reg_reg_op8_t op, R8 dst) {
    return [op, dst](CPU& cpu) {
        cpu.writeReg(
            dst,
            op(cpu, cpu.readReg(dst), cpu.fetch())
        );
    };
}

uint8_t add8(CPU& cpu, uint8_t a, uint8_t b) {
    uint16_t result = static_cast<uint16_t>(a) + b;

    cpu.setFlag(Flag::Z, static_cast<uint8_t>(result) == 0);
    cpu.setFlag(Flag::N, false);
    cpu.setFlag(Flag::H, (a & 0x0F) + (b & 0x0F) > 0x0F);
    cpu.setFlag(Flag::C, result > 0xFF);

    return static_cast<uint8_t>(result);
}

void addRegs(instruction_table_t& instructions) {
    // first add, add a b has opcode 0x80
    uint8_t opcode = 0x80;
    for (auto reg : register8s) {
        instructions[opcode++] = op8RegReg(add8, R8::A, reg);
    }
}

uint8_t adc8(CPU& cpu, uint8_t a, uint8_t b) {
    uint8_t carry = cpu.readFlag(Flag::C);

    uint16_t result = static_cast<uint16_t>(a) + b + carry;

    cpu.setFlag(Flag::Z, static_cast<uint8_t>(result) == 0);
    cpu.setFlag(Flag::N, false);
    cpu.setFlag(Flag::H, (a & 0x0F) + (b & 0x0F) + carry > 0x0F);
    cpu.setFlag(Flag::C, result > 0xFF);

    return static_cast<uint8_t>(result);
}

void adcRegs(instruction_table_t instructions) {
    // first adc, adc a b has opcode 0x88
    uint8_t opcode = 0x88;
    for (auto reg : register8s) {
        instructions[opcode++] = op8RegReg(adc8, R8::A, reg);
    }
}

uint8_t sub8(CPU& cpu, uint8_t a, uint8_t b)
{
    uint8_t result = a - b;

    cpu.setFlag(Flag::Z, result == 0);
    cpu.setFlag(Flag::N, true);
    cpu.setFlag(Flag::H, (a & 0x0F) < (b & 0x0F));
    cpu.setFlag(Flag::C, a < b);

    return result;
}

void subRegs(instruction_table_t& instructions) {
    // first sub, sub a b has opcode 0x90
    uint8_t opcode = 0x90;
    for (auto reg : register8s) {
        instructions[opcode++] = op8RegReg(sub8, R8::A, reg);
    }
}

uint8_t sbc8(CPU& cpu, uint8_t a, uint8_t b) {
    uint8_t carry = cpu.readFlag(Flag::C);

    uint8_t result = a - b - carry;

    cpu.setFlag(Flag::Z, result == 0);
    cpu.setFlag(Flag::N, true);
    cpu.setFlag(Flag::H, (a & 0x0F) < (b & 0x0F) + carry);
    cpu.setFlag(Flag::C, a < b + carry);

    return static_cast<uint8_t>(result);
}

void sbcRegs(instruction_table_t& instructions) {
    // first sbc, sbc a b has opcode 0x98
    uint8_t opcode = 0x98;
    for (auto reg : register8s) {
        instructions[opcode++] = op8RegReg(sbc8, R8::A, reg);
    }
}

uint8_t and8(CPU& cpu, uint8_t a, uint8_t b) {
    uint8_t result = a & b;

    cpu.setFlag(Flag::Z, result == 0);
    cpu.setFlag(Flag::N, false);
    cpu.setFlag(Flag::H, true);
    cpu.setFlag(Flag::C, false);

    return result;
}

void andRegs(instruction_table_t instructions) {
    // first and opcode
    uint8_t opcode = 0xA0;
    for (auto reg : register8s) {
        instructions[opcode++] = op8RegReg(and8, R8::A, reg);
    }
}

uint8_t xor8(CPU& cpu, uint8_t a, uint8_t b) {
    uint8_t result = a ^ b;

    cpu.setFlag(Flag::Z, result == 0);
    cpu.setFlag(Flag::N, false);
    cpu.setFlag(Flag::H, false);
    cpu.setFlag(Flag::C, false);

    return result;
}

void xorRegs(instruction_table_t instructions) {
    // first xor opcode
    uint8_t opcode = 0xA8;
    for (auto reg : register8s) {
        instructions[opcode++] = op8RegReg(xor8, R8::A, reg);
    }
}

uint8_t or8(CPU& cpu, uint8_t a, uint8_t b) {
    uint8_t result = a | b;

    cpu.setFlag(Flag::Z, result == 0);
    cpu.setFlag(Flag::N, false);
    cpu.setFlag(Flag::H, false);
    cpu.setFlag(Flag::C, false);

    return result;
}

void orRegs(instruction_table_t instructions) {
    // first or opcode
    uint8_t opcode = 0xB0;
    for (auto reg : register8s) {
        instructions[opcode++] = op8RegReg(or8, R8::A, reg);
    }
}

instruction_t cpRegReg(R8 dst, R8 src) {
    return [dst, src](CPU& cpu) {
        // just update the flags
        sub8(cpu, cpu.readReg(dst), cpu.readReg(src));
    };
}

instruction_t cpRegImm(R8 reg) {
    return [reg](CPU& cpu) {
        // just update the flags
        sub8(cpu, cpu.readReg(reg), cpu.fetch());
    };
}

void cpRegs(instruction_table_t& instructions) {
    // frst cp, cp a b has opcode 0xB8
    uint8_t opcode = 0xB8;
    for (auto reg : register8s) {
        instructions[opcode++] = cpRegReg(R8::A, reg);
    }
}

uint8_t inc8(CPU& cpu, uint8_t a) {
    uint8_t result = a + 1;

    cpu.setFlag(Flag::Z, result == 0);
    cpu.setFlag(Flag::N, false);
    cpu.setFlag(Flag::H, (a & 0x0F) + 1 > 0x0F);

    return result;
}

void incRegs(instruction_table_t instructions) {
    // first inc, inc b has opcode 0x04
    uint8_t opcode = 0x04;
    for (auto reg : register8s) {
        instructions[opcode] = op8Reg(inc8, reg);
        opcode += 8;
    }
}

uint8_t dec8(CPU& cpu, uint8_t a) {
    uint8_t result = a - 1;

    cpu.setFlag(Flag::Z, result == 0);
    cpu.setFlag(Flag::N, true);
    cpu.setFlag(Flag::H, (a & 0x0F) < 1);

    return result;
}

void incRegs(instruction_table_t instructions) {
    // first dec, dec b has opcode 0x05
    uint8_t opcode = 0x05;
    for (auto reg : register8s) {
        instructions[opcode] = op8Reg(dec8, reg);
        opcode += 8;
    }
}

instruction_t ccf() {
    return [](CPU& cpu) {
        cpu.setFlag(Flag::N, false);
        cpu.setFlag(Flag::H, false);
        cpu.setFlag(Flag::C, ~cpu.readFlag(Flag::C));
    };
}

instruction_t scf() {
    return [](CPU& cpu) {
        cpu.setFlag(Flag::N, false);
        cpu.setFlag(Flag::H, false);
        cpu.setFlag(Flag::C, true);
    };
}

// Called after an arithmetic instruction whose inputs were in BCD
// e.g. 0x42 to represent 42. Adjusts the result to also be in BCD
instruction_t daa() {
    return [](CPU& cpu) {
        uint8_t adjustment = 0;

        // if a subtraction previously occured
        if (cpu.readFlag(Flag::N)) {
            if (cpu.readFlag(Flag::H)) {
                adjustment += 0x06;
            }
            if (cpu.readFlag(Flag::C)) {
                adjustment += 0x60;
            }
            cpu.a -= adjustment;

        // if an addition previously occured
        } else {
            if (cpu.readFlag(Flag::H) || cpu.a & 0x0F > 0x09) {
                adjustment += 0x06;
            }
            if (cpu.readFlag(Flag::C) || cpu.a > 0x99) {
                adjustment += 0x60;
            }
            cpu.a += adjustment;
        }
    };
}

instruction_t cpl() {
    return [](CPU& cpu) {
        cpu.a = ~cpu.a;
        cpu.setFlag(Flag::N, true);
        cpu.setFlag(Flag::H, true);
    };
}

// 8-bit arithmetic / logical instructions
void arithmeticLogical8s(instruction_table_t& instructions) {
    addRegs(instructions);
    // add a n8
    instructions[0xC6] = op8RegImm(add8, R8::A);

    adcRegs(instructions);
    // adc a n8
    instructions[0xCE] = op8RegImm(adc8, R8::A);

    subRegs(instructions);
    // sub a n8
    instructions[0xD6] = op8RegImm(sub8, R8::A);

    sbcRegs(instructions);
    // sbc a n8
    instructions[0xDE] = op8RegImm(sbc8, R8::A);

    andRegs(instructions);
    // and a n8
    instructions[0xE6] = op8RegImm(and8, R8::A);

    xorRegs(instructions);
    // xor a n8
    instructions[0xEE] = op8RegImm(xor8, R8::A);

    orRegs(instructions);
    // or a n8
    instructions[0xF6] = op8RegImm(or8, R8::A);

    cpRegs(instructions);
    // cp a n8
    instructions[0xFE] = cpRegImm(R8::A);

    instructions[0x3F] = ccf();
    instructions[0x37] = scf();
    instructions[0x27] = daa();
    instructions[0x2F] = cpl();
}

void allInstructions(instruction_table_t& instructions) {
    instructions[0x00] = nop;

    ld8Instrs(instructions);
    ld16Instrs(instructions);

    arithmeticLogical8s(instructions);
}