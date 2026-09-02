#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include <SDL.h>

#include <array>

#include "keyboard.hpp"

struct Screen {
    Screen();
    ~Screen();

    Screen(const Screen&) = delete;
    auto operator=(const Screen&) -> Screen& = delete;

    auto static audioCallback(void* userdata, uint8_t* stream, int len) -> void;
    auto initAudio() -> void;

    auto clearScreen() -> void;
    auto drawSpriteLine(uint16_t xBegin, uint16_t y, uint8_t spriteLine) -> bool;
    auto renderFrame() -> void;

    auto handleEvents(Keyboard &keyboard) -> bool;

    static constexpr u_int WIDTH = 64;
    static constexpr u_int HEIGHT = 32;
    static constexpr u_int SCALING_FACTOR = 16;
    
    SDL_AudioDeviceID audioDevice;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    double audioPhase = 0.0;

    std::array<std::array<bool, WIDTH>, HEIGHT> pixelState; // array of lines(zeilen) of pixels
};

#endif /* SCREEN_HPP_ */