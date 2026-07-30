#include <iostream>

#include "emulator/cpu.hpp"
#include <iostream>

auto run(CPU &cpu) -> void {
    cpu.r1 = std::byte{5};
    
    std::cout << std::to_integer<int>(cpu.r1) << "\n";
}

auto main() -> int {
    CPU cpu;
    run(cpu);

    return 0;
}

