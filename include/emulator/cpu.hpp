
#ifndef CPU_HPP_
#define CPU_HPP_

#include "emulator/memory.hpp"

struct CPU {


// private:
    Memory memory;
    std::byte r1;
    std::byte r2;
};


#endif /* CPU_HPP_ */