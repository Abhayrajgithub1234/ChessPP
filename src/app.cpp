#include "app.h"

App::App(unsigned int width, unsigned int height, const char* name) {
    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height, 0);
}

App::~App() {
    SDL_DestroyWindow(m_window);
}

void App::run() {
    while (running) {
        while (SDL_PollEvent(&m_window_event) > 0) {
            switch (m_window_event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }

        SDL_UpdateWindowSurface(m_window);
    }
}
