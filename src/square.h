#pragma once
#include <SDL2/SDL.h>

#include "colors.h"

class Square {
    private:
        int x, y, size;
        Color color;
        SDL_Renderer* m_renderer;
    public:
        Square(int x, int y, int size, Color color, SDL_Renderer* renderer);
        ~Square();

        void draw();
};
