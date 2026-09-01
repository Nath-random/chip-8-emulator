#include <SDL.h>

#include "emulator.hpp"
#include "screen.hpp"

auto main() -> int {
    Emulator emulator{"/home/nath/Documents/workarea/chip-8-emulator/roms/br8kout.ch8"};
    emulator.start();

    return 0;
}