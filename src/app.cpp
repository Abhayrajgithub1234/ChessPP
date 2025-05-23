#include "app.h"

#include "board.h"
#include "square.h"

App::App(unsigned int width, unsigned int height, const char* name) {
    this->width = width;
    this->height = height;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) printf("Failed to init SDL\n");

    if (TTF_Init() < 0) printf("Failed to initialize the TTF\n");

    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height, 0);
}

App::~App() {
    TTF_Quit();
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void App::run() {
    Board board(width, height, m_window);

    while (running) {
        while (SDL_PollEvent(&m_window_event) > 0) {
            switch (m_window_event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    board.clickedPosition(m_window_event.button.x,
                                          m_window_event.button.y);
                    break;
            }
        }
        board.draw();
        // SDL_UpdateWindowSurface(m_window);
    }
}
