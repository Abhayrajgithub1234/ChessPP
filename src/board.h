#pragma once

#include <SDL2/SDL.h>

class Board {
    private:
        int width, height;
        SDL_Renderer* m_renderer;

    public:
        Board(int width, int height, SDL_Window* window);
        ~Board();
        void draw();
};
