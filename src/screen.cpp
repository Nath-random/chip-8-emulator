#include <iostream>

#include "screen.hpp"
#include "emulator.hpp"

Screen::Screen() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_CreateWindowAndRenderer(WIDTH * SCALING_FACTOR, HEIGHT * SCALING_FACTOR, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, SCALING_FACTOR, SCALING_FACTOR);
    clearScreen();

    initAudio();
}

Screen::~Screen() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

// userdata ist void-pointer, weil man irgendwas übergeben kann und in diesem Fall wird halt ein double übergeben
auto Screen::audioCallback(void* userdata, uint8_t* stream, int len) -> void {
    double* phase = reinterpret_cast<double*>(userdata);
    int16_t* buffer = reinterpret_cast<int16_t*>(stream);
    int length = len / 2; // weil wir 16 bit statt 8 machen, braucht es nur halb so viele Einträge

    constexpr int frequency = 400; // das ist Tonhöhe vom beep
    constexpr int sampleRate = 44100; // die Soundkarte aktualisiert das Sample 441000 mal pro Sekunde.
    //Ein Sample ist die Amplitude des Sounds in einem Moment. 

    constexpr int volume = 3000;

    for (int i = 0; i < length; ++i) {
        buffer[i] = (*phase < 0.0) ? volume : -volume; // Macht Rechtecksignal
        *phase += static_cast<double>(frequency) / sampleRate;
        if (*phase > 0.5) {
            *phase -= 1.0;
        }
    }
}

auto Screen::initAudio() -> void {
    SDL_AudioSpec want, have;
    SDL_zero(want);

    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 1;
    want.samples = 2048;
    want.callback = audioCallback;
    want.userdata = &audioPhase;

    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
}

auto Screen::clearScreen() -> void {
    for (size_t x = 0; x < WIDTH; ++x) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            pixelState.at(y).at(x) = 0;
        }
    }
}

auto Screen::drawSpriteLine(uint16_t xBegin, uint16_t y, uint8_t spriteLine) -> bool { // A line of a sprite is 8 bit wide
    if (y >= HEIGHT && !Emulator::spritesWrapAroundY) {
        return false;
    }

    bool pixelTurnedOff = false;
    for (size_t x = xBegin; x < xBegin + 8 && (x < WIDTH || Emulator::spritesWrapAroundX); ++x) {
        uint8_t bitMask = 0x1 << (7 + xBegin - x); // LSB is the rightmost pixel
        if (spriteLine & bitMask) {
            uint8_t wrappedX = (x + WIDTH) % WIDTH;
            uint8_t wrappedY = (y + HEIGHT) % HEIGHT;
            if (pixelState.at(wrappedY).at(wrappedX)) {
                pixelState.at(wrappedY).at(wrappedX) = false;
                pixelTurnedOff = true;
            } else {
                pixelState.at(wrappedY).at(wrappedX) = true;
            }
        }
    }
    return pixelTurnedOff;
}

auto Screen::renderFrame() -> void {
    for (size_t x = 0; x < WIDTH; ++x) {
        for (size_t y = 0; y < HEIGHT; ++y) {
            if (pixelState.at(y).at(x)) {
                SDL_SetRenderDrawColor(renderer, 150, 150, 200, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
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
                case SDL_SCANCODE_X: keyboard.pressKey(0x0); break;
                case SDL_SCANCODE_1: keyboard.pressKey(0x1); break;
                case SDL_SCANCODE_2: keyboard.pressKey(0x2); break;
                case SDL_SCANCODE_3: keyboard.pressKey(0x3); break;
                case SDL_SCANCODE_Q: keyboard.pressKey(0x4); break;
                case SDL_SCANCODE_W: keyboard.pressKey(0x5); break;
                case SDL_SCANCODE_E: keyboard.pressKey(0x6); break;
                case SDL_SCANCODE_A: keyboard.pressKey(0x7); break;
                case SDL_SCANCODE_S: keyboard.pressKey(0x8); break;
                case SDL_SCANCODE_D: keyboard.pressKey(0x9); break;
                case SDL_SCANCODE_Y: keyboard.pressKey(0xA); break;
                case SDL_SCANCODE_C: keyboard.pressKey(0xB); break;
                case SDL_SCANCODE_4: keyboard.pressKey(0xC); break;
                case SDL_SCANCODE_R: keyboard.pressKey(0xD); break;
                case SDL_SCANCODE_F: keyboard.pressKey(0xE); break;
                case SDL_SCANCODE_V: keyboard.pressKey(0xF); break;
                default: break;
            }            
        } else if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_X: keyboard.releaseKey(0x0); break;
                case SDL_SCANCODE_1: keyboard.releaseKey(0x1); break;
                case SDL_SCANCODE_2: keyboard.releaseKey(0x2); break;
                case SDL_SCANCODE_3: keyboard.releaseKey(0x3); break;
                case SDL_SCANCODE_Q: keyboard.releaseKey(0x4); break;
                case SDL_SCANCODE_W: keyboard.releaseKey(0x5); break;
                case SDL_SCANCODE_E: keyboard.releaseKey(0x6); break;
                case SDL_SCANCODE_A: keyboard.releaseKey(0x7); break;
                case SDL_SCANCODE_S: keyboard.releaseKey(0x8); break;
                case SDL_SCANCODE_D: keyboard.releaseKey(0x9); break;
                case SDL_SCANCODE_Y: keyboard.releaseKey(0xA); break;
                case SDL_SCANCODE_C: keyboard.releaseKey(0xB); break;
                case SDL_SCANCODE_4: keyboard.releaseKey(0xC); break;
                case SDL_SCANCODE_R: keyboard.releaseKey(0xD); break;
                case SDL_SCANCODE_F: keyboard.releaseKey(0xE); break;
                case SDL_SCANCODE_V: keyboard.releaseKey(0xF); break;
                default: break;
            }
        }
    }
    return terminationRequested;
}