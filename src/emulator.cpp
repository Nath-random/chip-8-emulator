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
        fetch();
        decode();
        execute();
    }
}

auto Emulator::fetch() -> void {
    opCode = cpu.memory.ram.at(cpu.pc) << 8;
    opCode += cpu.memory.ram.at(cpu.pc + 1);

    cpu.pc = (cpu.pc + 2) % 4096;
}

auto Emulator::decode() -> void {
    firstNibble = (opCode & 0xF000) >> 12;
    secondNibble = (opCode & 0x0F00) >> 8;
    thirdNibble = (opCode & 0x00F0) >> 4;
    fourthNibble = opCode & 0x000F;

    firstByte = (opCode & 0xFF00) >> 8;
    secondByte = opCode & 0x00FF;

    lastTwelveBits = opCode & 0x0FFF;

    if (firstNibble == 0x1) {
        currentOperation = &Emulator::opJumpAddr;
        return;
    } else if (firstNibble == 0x6) {
        currentOperation = &Emulator::opSet;
        return;
    }
    
    currentOperation = &Emulator::opNop;
}

auto Emulator::execute() -> void {
    (this->*currentOperation)();
}


// OpCodes
auto Emulator::opJumpAddr() -> void { // Jump to Address
    cpu.pc = lastTwelveBits;
}

auto Emulator::opSet() -> void {
    cpu.vRegisters.at(secondNibble) = secondByte;
}

auto Emulator::opNop() -> void { // no operation
    return;
}