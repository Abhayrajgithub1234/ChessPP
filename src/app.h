#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

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
