
#ifndef CPU_HPP_
#define CPU_HPP_

#include "memory.hpp"
#include <cstdint>

struct CPU {


// private:
    Memory memory;

    // registers v0 to vF, vF is used as flag register. v stands for variable
    std::array<uint8_t, 16> vRegisters; 

    // program counter
    uint16_t pc;

    // index register
    uint16_t i;
};


#endif /* CPU_HPP_ */