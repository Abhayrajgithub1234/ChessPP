#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

class App {
    public:
        int width, height;
        App(unsigned int width, unsigned int height, const char *name);
        ~App();
        void run();

    private:
        SDL_Window *m_window;
        SDL_Event m_window_event;
        bool running;
};
