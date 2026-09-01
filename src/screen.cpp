#include <iostream>

#include "screen.hpp"

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
            if (pixelState.at(y).at(x)) {
                pixelState.at(y).at(x) = false;
                pixelTurnedOff = true;
            } else {
                pixelState.at(y).at(x) = true;
            }
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

auto Screen::handleEvents(Keyboard &keyboard) -> bool {
    SDL_Event event;
    bool terminationRequested = false;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            terminationRequested = true;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_0: keyboard.pressKey(0x0); break;
                case SDL_SCANCODE_1: keyboard.pressKey(0x1); break;
                case SDL_SCANCODE_2: keyboard.pressKey(0x2); break;
                case SDL_SCANCODE_3: keyboard.pressKey(0x3); break;
                case SDL_SCANCODE_4: keyboard.pressKey(0x4); break;
                case SDL_SCANCODE_5: keyboard.pressKey(0x5); break;
                case SDL_SCANCODE_6: keyboard.pressKey(0x6); break;
                case SDL_SCANCODE_7: keyboard.pressKey(0x7); break;
                case SDL_SCANCODE_8: keyboard.pressKey(0x8); break;
                case SDL_SCANCODE_9: keyboard.pressKey(0x9); break;
                case SDL_SCANCODE_A: keyboard.pressKey(0xA); break;
                case SDL_SCANCODE_B: keyboard.pressKey(0xB); break;
                case SDL_SCANCODE_C: keyboard.pressKey(0xC); break;
                case SDL_SCANCODE_D: keyboard.pressKey(0xD); break;
                case SDL_SCANCODE_E: keyboard.pressKey(0xE); break;
                case SDL_SCANCODE_F: keyboard.pressKey(0xF); break;
                default: break;
            }            
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_0: keyboard.releaseKey(0x0); break;
                case SDL_SCANCODE_1: keyboard.releaseKey(0x1); break;
                case SDL_SCANCODE_2: keyboard.releaseKey(0x2); break;
                case SDL_SCANCODE_3: keyboard.releaseKey(0x3); break;
                case SDL_SCANCODE_4: keyboard.releaseKey(0x4); break;
                case SDL_SCANCODE_5: keyboard.releaseKey(0x5); break;
                case SDL_SCANCODE_6: keyboard.releaseKey(0x6); break;
                case SDL_SCANCODE_7: keyboard.releaseKey(0x7); break;
                case SDL_SCANCODE_8: keyboard.releaseKey(0x8); break;
                case SDL_SCANCODE_9: keyboard.releaseKey(0x9); break;
                case SDL_SCANCODE_A: keyboard.releaseKey(0xA); break;
                case SDL_SCANCODE_B: keyboard.releaseKey(0xB); break;
                case SDL_SCANCODE_C: keyboard.releaseKey(0xC); break;
                case SDL_SCANCODE_D: keyboard.releaseKey(0xD); break;
                case SDL_SCANCODE_E: keyboard.releaseKey(0xE); break;
                case SDL_SCANCODE_F: keyboard.releaseKey(0xF); break;
                default: break;
            }
        }
    }
    return terminationRequested;
}