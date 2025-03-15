#pragma once
#include <SDL2/SDL.h>

#include "piece.h" 
#include "piece.h"

class King : public Piece {
    public:
        King();
        King(int xPos, int yPos, Color color);
        ~King() override;
        void draw(SDL_Renderer* m_renderer) override;
        void setColor(Color color, SDL_Renderer* m_renderer) override;
};

