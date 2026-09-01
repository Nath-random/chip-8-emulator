#include "keyboard.hpp"

auto Keyboard::pressKey(uint8_t key) -> void {
    keys.at(key) = true;
}

auto Keyboard::releaseKey(uint8_t key) -> void {
    keys.at(key) = false;
}