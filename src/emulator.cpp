#include <fstream>
#include <iostream>
#include <exception>
#include "emulator.hpp"

Emulator::Emulator(std::string diskPath) : diskPath{diskPath} {
    loadROM();
}

auto Emulator::loadROM() -> void {
    std::ifstream romFile{diskPath, std::ios::binary};
    
    if (!romFile.is_open()) {
        throw std::runtime_error{"Emulator: ROM File " + diskPath + " not found!"};
    }

    size_t byteCount = 0x200; //starts at 200 because 000 to 1FF is reserved
    char byte;
    while(byteCount < 4096 && romFile.get(byte)) {
        // std::cout << +byte << " ";
        cpu.memory.ram.at(byteCount++) = byte;
    }
}

auto Emulator::start() -> void {
    while (true) {
        // std::cout << +cpu.pc;
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

    if (opCode == 0x00E0) {
        currentOperation = &Emulator::opClearScreen;
    } else if (firstNibble == 0x1) {
        currentOperation = &Emulator::opJumpAddr;
    } else if (firstNibble == 0x6) {
        currentOperation = &Emulator::opSet;
    } else if (firstNibble == 0x7) {
        currentOperation = &Emulator::opAdd;
    } else if (firstNibble == 0xA) {
        currentOperation = &Emulator::opSetIndex;
    } else if (firstNibble == 0xD) {
        currentOperation = &Emulator::opDraw;
    } else {
        std::cout << "unknown instruction" << +opCode << "\n";
        currentOperation = &Emulator::opNop;
    }
}

auto Emulator::execute() -> void {
    (this->*currentOperation)();
}


// OpCodes
auto Emulator::opClearScreen() -> void {
    screen.clearScreen();
}
auto Emulator::opJumpAddr() -> void { // Jump to Address
    cpu.pc = lastTwelveBits;
}
auto Emulator::opSet() -> void {
    cpu.vRegisters.at(secondNibble) = secondByte;
}
auto Emulator::opAdd() -> void {
    cpu.vRegisters.at(secondNibble) += secondByte;
}
auto Emulator::opSetIndex() -> void {
    cpu.i = lastTwelveBits;
}
auto Emulator::opDraw() -> void {
    bool pixelTurnedOff = false;
    uint16_t spriteLineDataLocation = cpu.i;
    uint16_t screenX = cpu.vRegisters.at(secondNibble) % screen.WIDTH;
    uint16_t screenY = cpu.vRegisters.at(thirdNibble) % screen.HEIGHT;
    for (size_t line = 0; line < fourthNibble; ++line, ++spriteLineDataLocation, ++screenY) {
        if (screen.drawSpriteLine(screenX, screenY, cpu.memory.ram.at(spriteLineDataLocation))) {
            pixelTurnedOff = true;
        }
    }

    cpu.vRegisters.at(0xF) = pixelTurnedOff;
}
auto Emulator::opNop() -> void { // no operation
    return;
}