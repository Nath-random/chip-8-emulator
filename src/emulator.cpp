#include <fstream>
#include <iostream>
#include <exception>
#include "emulator.hpp"

Emulator::Emulator(std::string path) : path{path} {
    loadROM();
}

auto Emulator::loadROM() -> void {
    std::ifstream romFile{path, std::ios::binary};
    
    if (!romFile.is_open()) {
        throw std::runtime_error{"Emulator: ROM File " + path + " not found!"};
    }

    size_t byteCount = 0;
    char byte;
    while(byteCount < 4096 && romFile.get(byte)) {
        std::cout << +byte << " ";
        cpu.memory.ram.at(byteCount++) = byte;
    }
}

auto Emulator::start() -> void {

    while (true) {
        std::cout << +cpu.pc;
        uint16_t opCode = fetch();
        Operation operation = decode(opCode);
        execute(operation, opCode);
    }
}

auto Emulator::fetch() -> uint16_t {
    uint16_t opCode = cpu.memory.ram.at(cpu.pc) << 8;
    opCode += cpu.memory.ram.at(cpu.pc + 1);

    cpu.pc = (cpu.pc + 2) % 4096;
    return opCode;
}

auto Emulator::decode(uint16_t opCode) -> Operation {
    if ((opCode & 0xF000) == 0x1000) {
        return &Emulator::jumpAddr;
    }
    
    return &Emulator::nop;
}

auto Emulator::execute(Operation operation, uint16_t opCode) -> void {
    (this->*operation)(opCode);
}


// OpCodes
auto Emulator::jumpAddr(uint16_t opCode) -> void { // Jump to Address
    uint16_t jumpAddress = opCode & 0x0FFF;
    cpu.pc = jumpAddress;
}

auto Emulator::nop(uint16_t _) -> void { // no operation
    return;
}