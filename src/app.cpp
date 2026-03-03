#include "app.h"

#include "board.h"
#include "square.h"
#include "constants.h"

App::App(unsigned int width, unsigned int height, const char* name) {
    this->width = width;
    this->height = height;
    running = true;  // Initialize running to true
    m_window = nullptr;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Failed to init SDL: %s\n", SDL_GetError());
        running = false;
        return;
    }
    
    // Enable linear texture filtering for smoother piece scaling
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");  // "2" = best quality (anisotropic)

    if (TTF_Init() < 0) {
        printf("Failed to initialize the TTF: %s\n", TTF_GetError());
        running = false;
        return;
    }

    m_window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height,
                                SDL_WINDOW_RESIZABLE);
    
    if (!m_window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        running = false;
        return;
    }
    
    // Set minimum window size
    SDL_SetWindowMinimumSize(m_window, ChessConstants::MIN_WINDOW_WIDTH, 
                             ChessConstants::MIN_WINDOW_HEIGHT);
    
    printf("SDL initialized successfully. Window created.\n");
}

App::~App() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
    }
    TTF_Quit();
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
                case SDL_WINDOWEVENT:
                    if (m_window_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        width = m_window_event.window.data1;
                        height = m_window_event.window.data2;
                        board.handleResize(width, height);
                    }
                    break;
            }
        }
        board.draw();
        // SDL_UpdateWindowSurface(m_window);
    }
}
