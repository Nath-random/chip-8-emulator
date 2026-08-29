#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include "cpu.hpp"
#include <string>

struct Emulator {
    using Operation = void (Emulator::*)();

    Emulator(std::string path);

    auto loadROM() -> void;
    auto start() -> void;

    auto fetch() -> void;
    auto decode() -> void;
    auto execute() -> void;

    auto opJumpAddr() -> void;
    auto opSet() -> void;
    auto opNop() -> void;

private:
    std::string path;
    CPU cpu{};

    //values of current instruction
    uint16_t opCode{0};

    uint8_t firstNibble{0}; //the first half-byte of the opcode
    uint8_t secondNibble{0};
    uint8_t thirdNibble{0};
    uint8_t fourthNibble{0};

    uint8_t firstByte{0};
    uint8_t secondByte{0};

    uint16_t lastTwelveBits{0}; // same as "lastThreeNibbles", "leastSignificant12Bits"

    Operation currentOperation = &Emulator::opNop;
};

#endif /* EMULATOR_HPP_ */