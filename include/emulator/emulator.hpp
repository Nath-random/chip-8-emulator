#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include <string>

#include "cpu.hpp"
#include "screen.hpp"

struct Emulator {
    using Operation = void (Emulator::*)();

    Emulator(std::string path);

    auto loadROM() -> void;
    auto start() -> void;

    auto fetch() -> void;
    auto decode() -> void;
    auto execute() -> void;

    auto opClearScreen() -> void;
    auto opJumpAddr() -> void;
    auto opSet() -> void;
    auto opAdd() -> void;
    auto opSetIndex() -> void;
    auto opDraw() -> void;
    auto opNop() -> void;

    static constexpr uint fps = 60; // frames per second
    static constexpr uint cpf = 10; // cycles per frame

private:
    CPU cpu{};
    Screen screen{};
    std::string diskPath;

    uint32_t frameCount = 0;
    uint32_t cycleCount = 0;

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