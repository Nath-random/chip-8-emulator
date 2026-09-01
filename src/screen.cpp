#include "screen.hpp"
#include <iostream>

Screen::Screen() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH * SCALING_FACTOR, HEIGHT * SCALING_FACTOR, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, SCALING_FACTOR, SCALING_FACTOR);
    clearScreen();
}

Screen::~Screen() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

auto Screen::clearScreen() -> void {
    for (size_t x = 0; x < WIDTH; ++x) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            pixelState.at(y).at(x) = 0;
        }
    }
}

auto Screen::drawSpriteLine(uint16_t xBegin, uint16_t y, uint8_t spriteLine) -> bool { // A line of a sprite is 8 bit wide
    if (y >= HEIGHT) {
        return false;
    }

    bool pixelTurnedOff = false;
    for (size_t x = xBegin; x < xBegin + 8 && x < WIDTH; ++x) {
        uint8_t bitMask = 0x1 << (7 + xBegin - x); // LSB is the rightmost pixel
        if (spriteLine & bitMask) {
            pixelState.at(y).at(x) = !pixelState.at(y).at(x); // toggle/XOR pixel
        }
    }
    return pixelTurnedOff;
}

auto Screen::renderFrame() -> void {
    for (size_t x = 0; x < WIDTH; ++x) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            if (pixelState.at(y).at(x)) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);
}