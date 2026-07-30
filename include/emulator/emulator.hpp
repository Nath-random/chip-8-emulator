#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include "cpu.hpp"
#include <string>

struct Emulator {
    using Operation = void (Emulator::*)(uint16_t opCode);

    Emulator(std::string path);

    auto loadROM() -> void;
    auto start() -> void;

    auto fetch() -> uint16_t;
    auto decode(uint16_t opCode) -> Operation;
    auto execute(Operation operation, uint16_t opCode) -> void;

    auto jumpAddr(uint16_t opCode) -> void;
    auto nop(uint16_t _) -> void;

private:
    std::string path;
    CPU cpu{};
};

#endif /* EMULATOR_HPP_ */