#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include <string>

#include "cpu.hpp"
#include "keyboard.hpp"
#include "screen.hpp"

struct Emulator {
    using Operation = void (Emulator::*)();

    Emulator(std::string path);

    auto loadROM() -> void;
    auto start() -> void;

    auto fetch() -> void;
    auto decode() -> void;
    auto execute() -> void;

    auto updateTimers() -> void;

    auto opClearScreen() -> void; // 00E0
    auto opReturnSubroutine() -> void; // 00EE
    auto opJumpAddr() -> void; // 1NNN
    auto opCallSubroutine() -> void; // 2NNN
    auto opSkipEqual() -> void; // 3XNN
    auto opSkipNotEqual() -> void; // 4XNN
    auto opSkipRegistersEqual() -> void; // 5XY0
    auto opValueSet() -> void; // 6XNN
    auto opValueAdd() -> void; // 7XNN
    auto opRegisterSet() -> void; // 8XY0
    auto opOr() -> void; // 8XY1
    auto opAnd() -> void; // 8XY2
    auto opXor() -> void; // 8XY3
    auto opRegisterAdd() -> void; // 8XY4
    auto opSubtract() -> void; // 8XY5
    auto opShiftRight() -> void; // 8XY6
    auto opSubtractReverse() -> void; // 8XY7
    auto opShiftLeft() -> void; // 8XYE
    auto opSkipRegistersNotEqual() -> void; // 9XY0
    auto opSetIndex() -> void; // ANNN
    auto opJumpOffset() -> void; // BNNN
    auto opRandom() -> void; // CXNN
    auto opDraw() -> void; // DXYN
    auto opSkipKey() -> void; // EX9E
    auto opSkipNoKey() -> void; // EXA1
    auto opGetDelayTimer() -> void; // FX07
    auto opSetDelayTimer() -> void; // FX15
    auto opSetSoundTimer() -> void; // FX18
    auto opAddIndex() -> void; // FX1E
    auto opGetKey() -> void; // FX0A
    auto opFontCharacter() -> void; // FX29
    auto opDecimalConversion() -> void; // FX33
    auto opStoreRegisters() -> void; // FX55
    auto opLoadRegisters() -> void; // FX65
    auto opNop() -> void; // Rest

    static constexpr uint fps = 60; // frames per second
    static constexpr uint cpf = 10; // cycles per frame

    // settings
    static constexpr bool shiftChangesFlag = true;
    static constexpr bool addToIndexChangesFlag = true;
    static constexpr bool spritesWrapAroundX = true;
    static constexpr bool spritesWrapAroundY = true;
    static constexpr bool specialJumpWithOffsetBehaviour = false;

private:
    CPU cpu{};
    Screen screen{};
    Keyboard keyboard{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};

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