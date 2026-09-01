#include <algorithm>
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <thread>

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
        cpu.ram.at(byteCount++) = byte;
    }
}

auto Emulator::start() -> void {
    while (true) {
        std::chrono::time_point frameStart = std::chrono::steady_clock::now();

        for (size_t i = 0; i < cpf; ++i) { // default: 10 CPU Cycles per frame
            fetch();
            decode();
            execute();
            cycleCount++;
        }

        updateTimers();

        screen.renderFrame();
        frameCount++;

        if (screen.handleEvents(keyboard)) {
            return;
        }
        

        std::chrono::nanoseconds timeSinceLastFrame = std::chrono::steady_clock::now() - frameStart;
        if (timeSinceLastFrame < std::chrono::milliseconds(1000 / fps)) {
            std::chrono::nanoseconds remainingTime = std::chrono::nanoseconds(1000000000 / fps) - timeSinceLastFrame;
            std::this_thread::sleep_for(remainingTime);
        }
    }
}

auto Emulator::updateTimers() -> void {
    if (delayTimer) {
        --delayTimer;
    }
    if (soundTimer) {
        --soundTimer;
    }
}

auto Emulator::fetch() -> void {
    opCode = cpu.ram.at(cpu.pc) << 8;
    opCode += cpu.ram.at(cpu.pc + 1);

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
    } else if (opCode == 0x00EE) {
        currentOperation = &Emulator::opReturnSubroutine;
    } else if (firstNibble == 0x1) {
        currentOperation = &Emulator::opJumpAddr;
    } else if (firstNibble == 0x2) {
        currentOperation = &Emulator::opCallSubroutine;
    } else if (firstNibble == 0x3) {
        currentOperation = &Emulator::opSkipEqual;
    } else if (firstNibble == 0x4) {
        currentOperation = &Emulator::opSkipNotEqual;
    } else if (firstNibble == 0x5 && fourthNibble == 0x0) {
        currentOperation = &Emulator::opSkipRegistersEqual;
    } else if (firstNibble == 0x6) {
        currentOperation = &Emulator::opValueSet;
    } else if (firstNibble == 0x7) {
        currentOperation = &Emulator::opValueAdd;
    } else if (firstNibble == 0x8 && fourthNibble == 0x0) {
        currentOperation = &Emulator::opRegisterSet;
    } else if (firstNibble == 0x8 && fourthNibble == 0x1) {
        currentOperation = &Emulator::opOr;
    } else if (firstNibble == 0x8 && fourthNibble == 0x2) {
        currentOperation = &Emulator::opAnd;
    } else if (firstNibble == 0x8 && fourthNibble == 0x3) {
        currentOperation = &Emulator::opXor;
    } else if (firstNibble == 0x8 && fourthNibble == 0x4) {
        currentOperation = &Emulator::opRegisterAdd;
    } else if (firstNibble == 0x8 && fourthNibble == 0x5) {
        currentOperation = &Emulator::opSubtract;
    } else if (firstNibble == 0x8 && fourthNibble == 0x6) {
        currentOperation = &Emulator::opShiftRight;
    } else if (firstNibble == 0x8 && fourthNibble == 0x7) {
        currentOperation = &Emulator::opSubtractReverse;
    } else if (firstNibble == 0x8 && fourthNibble == 0xE) {
        currentOperation = &Emulator::opShiftLeft;
    } else if (firstNibble == 0x9 && fourthNibble == 0x0) {
        currentOperation = &Emulator::opSkipRegistersNotEqual;
    } else if (firstNibble == 0xA) {
        currentOperation = &Emulator::opSetIndex;
    } else if (firstNibble == 0xB) {
        currentOperation = &Emulator::opJumpOffset;
    } else if (firstNibble == 0xC) {
        currentOperation = &Emulator::opRandom;
    } else if (firstNibble == 0xD) {
        currentOperation = &Emulator::opDraw;
    } else if ((opCode & 0xF0FF) == 0xE09E) {
        currentOperation = &Emulator::opSkipKey;
    } else if ((opCode & 0xF0FF) == 0xE0A1) {
        currentOperation = &Emulator::opSkipNoKey;
    } else if ((opCode & 0xF0FF)== 0xF007) {
        currentOperation = &Emulator::opGetDelayTimer;
    } else if ((opCode & 0xF0FF) == 0xF015) {
        currentOperation = &Emulator::opSetDelayTimer;
    } else if ((opCode & 0xF0FF) == 0xF018) {
        currentOperation = &Emulator::opSetSoundTimer;
    } else if ((opCode & 0xF0FF) == 0xF01E) {
        currentOperation = &Emulator::opAddIndex;
    } else if ((opCode & 0xF0FF) == 0xF00A) {
        currentOperation = &Emulator::opGetKey;
    } else if ((opCode & 0xF0FF) == 0xF029) {
        currentOperation = &Emulator::opFontCharacter;
    } else if ((opCode & 0xF0FF) == 0xF033) {
        currentOperation = &Emulator::opDecimalConversion;
    } else if ((opCode & 0xF0FF) == 0xF055) {
        currentOperation = &Emulator::opStoreRegisters;
    } else if ((opCode & 0xF0FF) == 0xF065) {
        currentOperation = &Emulator::opLoadRegisters;
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
auto Emulator::opReturnSubroutine() -> void {
    cpu.pc = cpu.stack.top();
    cpu.stack.pop();
}
auto Emulator::opJumpAddr() -> void { // Jump to Address
    cpu.pc = lastTwelveBits;
}
auto Emulator::opCallSubroutine() -> void {
    cpu.stack.push(cpu.pc);
    cpu.pc = lastTwelveBits;
}
auto Emulator::opSkipEqual() -> void {
    if (cpu.vRegisters.at(secondNibble) == secondByte) {
        cpu.pc = (cpu.pc + 2) % 4096;
    }
}
auto Emulator::opSkipNotEqual() -> void {
    if (cpu.vRegisters.at(secondNibble) != secondByte) {
        cpu.pc = (cpu.pc + 2) % 4096;
    }
}
auto Emulator::opSkipRegistersEqual() -> void {
    if (cpu.vRegisters.at(secondNibble) == cpu.vRegisters.at(thirdNibble)) {
        cpu.pc = (cpu.pc + 2) % 4096;
    }
}
auto Emulator::opValueSet() -> void {
    cpu.vRegisters.at(secondNibble) = secondByte;
}
auto Emulator::opValueAdd() -> void {
    cpu.vRegisters.at(secondNibble) += secondByte;
}
auto Emulator::opRegisterSet() -> void {
    cpu.vRegisters.at(secondNibble) = cpu.vRegisters.at(thirdNibble);
}
auto Emulator::opOr() -> void {
    cpu.vRegisters.at(secondNibble) = cpu.vRegisters.at(secondNibble) | cpu.vRegisters.at(thirdNibble);
}
auto Emulator::opAnd() -> void {
    cpu.vRegisters.at(secondNibble) = cpu.vRegisters.at(secondNibble) & cpu.vRegisters.at(thirdNibble);
}
auto Emulator::opXor() -> void {
    cpu.vRegisters.at(secondNibble) = cpu.vRegisters.at(secondNibble) ^ cpu.vRegisters.at(thirdNibble);
}
auto Emulator::opRegisterAdd() -> void {
    cpu.vRegisters.at(secondNibble) += cpu.vRegisters.at(thirdNibble);
}
auto Emulator::opSubtract() -> void {
    cpu.vRegisters.at(secondNibble) -= cpu.vRegisters.at(thirdNibble);
}
auto Emulator::opShiftRight() -> void {
    cpu.vRegisters.at(secondNibble) = cpu.vRegisters.at(thirdNibble) >> 1;
}
auto Emulator::opSubtractReverse() -> void {
    cpu.vRegisters.at(secondNibble) = cpu.vRegisters.at(thirdNibble) - cpu.vRegisters.at(secondNibble);
}
auto Emulator::opShiftLeft() -> void {
    cpu.vRegisters.at(secondNibble) = cpu.vRegisters.at(thirdNibble) << 1;
}
auto Emulator::opSkipRegistersNotEqual() -> void {
    if (cpu.vRegisters.at(secondNibble) != cpu.vRegisters.at(thirdNibble)) {
        cpu.pc = (cpu.pc + 2) % 4096;
    }}
auto Emulator::opSetIndex() -> void {
    cpu.i = lastTwelveBits;
}
auto Emulator::opJumpOffset() -> void {
    cpu.pc = lastTwelveBits + cpu.vRegisters.at(0);
}
auto Emulator::opRandom() -> void {
    cpu.vRegisters.at(0) = 0;
    std::cout << "not implemented: " << opCode << "\n";
}
auto Emulator::opDraw() -> void {
    bool pixelTurnedOff = false;
    uint16_t spriteLineDataLocation = cpu.i;
    uint16_t screenX = cpu.vRegisters.at(secondNibble) % screen.WIDTH;
    uint16_t screenY = cpu.vRegisters.at(thirdNibble) % screen.HEIGHT;
    for (size_t line = 0; line < fourthNibble; ++line, ++spriteLineDataLocation, ++screenY) {
        if (screen.drawSpriteLine(screenX, screenY, cpu.ram.at(spriteLineDataLocation))) {
            pixelTurnedOff = true;
        }
    }

    cpu.vRegisters.at(0xF) = pixelTurnedOff;
}
auto Emulator::opSkipKey() -> void {
    if (keyboard.keys.at(secondNibble)) {
        cpu.pc = (cpu.pc + 2) % 4096;
    }
}
auto Emulator::opSkipNoKey() -> void {
    if (!keyboard.keys.at(secondNibble)) {
        cpu.pc = (cpu.pc + 2) % 4096;
    }
}
auto Emulator::opGetDelayTimer() -> void {
    cpu.vRegisters.at(secondNibble) = delayTimer;
}
auto Emulator::opSetDelayTimer() -> void {
    delayTimer = cpu.vRegisters.at(secondNibble);
}
auto Emulator::opSetSoundTimer() -> void {
    soundTimer = cpu.vRegisters.at(secondNibble);
}
auto Emulator::opAddIndex() -> void {
    cpu.i += cpu.vRegisters.at(secondNibble);
}
auto Emulator::opGetKey() -> void {
    for (size_t key = 0; key <= 0xF; ++key) {
        if (keyboard.keys.at(key)) {
            cpu.vRegisters.at(secondNibble) = key;
            return; 
        }
    }

    cpu.pc = (cpu.pc + 4096 - 2) % 4096; // do this instruction until a button is pressed
}
auto Emulator::opFontCharacter() -> void {
    cpu.i = 0x50 + 5 * cpu.vRegisters.at(secondNibble); // a character is bytes long, font is stored at 0x50 - 0x9F
}
auto Emulator::opDecimalConversion() -> void {
    uint8_t binaryNumber = cpu.vRegisters.at(secondNibble);

    uint8_t decimalDigit = binaryNumber % 10;
    cpu.ram.at(cpu.i + 2) = decimalDigit;
    decimalDigit /= 10;

    decimalDigit = binaryNumber % 10;
    cpu.ram.at(cpu.i + 1) = decimalDigit;
    decimalDigit /= 10;

    decimalDigit = binaryNumber;
    cpu.ram.at(cpu.i) = decimalDigit;
}
auto Emulator::opStoreRegisters() -> void {
    uint16_t address = cpu.i;
    for (size_t n = 0; n <= 0xF; ++n) {
        cpu.ram.at(address) = cpu.vRegisters.at(n);
        address = (address + 1) % 4096;
    }
}
auto Emulator::opLoadRegisters() -> void {
    uint16_t address = cpu.i;
    for (size_t n = 0; n <= 0xF; ++n) {
        cpu.vRegisters.at(n) = cpu.ram.at(address);
        address = (address + 1) % 4096;
    }
}
auto Emulator::opNop() -> void { // no operation
    return;
}