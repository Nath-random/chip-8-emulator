#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include <array>
#include <cstdint>

struct Keyboard {

    auto pressKey(uint8_t key) -> void;
    auto releaseKey(uint8_t key) -> void;

    std::array<bool, 16> keys{};
};

#endif /* KEYBOARD_HPP_ */