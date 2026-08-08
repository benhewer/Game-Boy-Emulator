#include <vector>

#include "types.h"
#include "RegisterPair.h"
#include "Register.h"
#include "MemoryMap.h"

class CPU {
private:
    RegisterPair af_storage;
    RegisterPair bc_storage;
    RegisterPair de_storage;
    RegisterPair hl_storage;

    MemoryMap mem;

    size_t cycles;

    instruction_table_t instructions;

    void addInstructions();

    uint8_t& memory(address_t address);
    uint8_t& memory(Register16 r);
    
public:
    address_t sp;
    address_t pc;

    uint16_t& af;
    uint16_t& bc;
    uint16_t& de;
    uint16_t& hl;

    uint8_t& a;
    uint8_t& f;
    uint8_t& b;
    uint8_t& c;
    uint8_t& d;
    uint8_t& e;
    uint8_t& h;
    uint8_t& l;

    CPU(MemoryMap& memory);

    uint8_t readMemory(address_t address);
    uint8_t readMemory(Register16 r);

    void writeMemory(address_t address, uint8_t data);
    void writeMemory(Register16 r, uint8_t data);

    uint8_t& reg(Register8 r);
    uint16_t& reg(Register16 r);

    uint8_t readReg(Register8 r);
    uint16_t readReg(Register16 r);
    
    void writeReg(Register8 r, uint8_t data);
    void writeReg(Register16 r, uint16_t data);

    uint8_t msbReg(Register16 r);
    uint8_t lsbReg(Register16 r);

    void setFlag(Flag flag, uint8_t value);
    void setFlags(
        uint8_t z,
        uint8_t n,
        uint8_t h,
        uint8_t c
    );

    // returns wram[pc] and increments pc
    uint8_t fetch();
};
