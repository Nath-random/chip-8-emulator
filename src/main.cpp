#include <SDL.h>
#include "emulator.hpp"

auto main() -> int {
    Emulator emulator{"/home/nath/Documents/workarea/chip-8-emulator/roms/input.ch8"};
    emulator.start();

    return 0;
}