#include <vector>

#include "types.h"
#include "Registers.h"

class CPU {
private:
    address_t sp;
    address_t pc;

    Registers registers;

public:
    CPU();
};
