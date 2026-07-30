
#ifndef CPU_HPP_
#define CPU_HPP_

#include "memory.hpp"
#include <cstdint>

struct CPU {


// private:
    Memory memory;

    // registers
    uint8_t v0; // v steht für variable.
    uint8_t v1;
    uint8_t v2;
    uint8_t v3;
    uint8_t v4;
    uint8_t v5;
    uint8_t v6;
    uint8_t v7;
    uint8_t v8;
    uint8_t v9;
    uint8_t vA;
    uint8_t vB;
    uint8_t vC;
    uint8_t vD;
    uint8_t vE;
    uint8_t vF; // used as flag register

    // program counter
    uint16_t pc;

    // index register
    uint16_t i;
};


#endif /* CPU_HPP_ */