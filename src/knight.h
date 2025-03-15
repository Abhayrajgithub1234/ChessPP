#pragma once
#include <SDL2/SDL.h>

#include "piece.h"

class Knight : public Piece {
    public:
        Knight();
        Knight(int xPos, int yPos, Color color);
        ~Knight() override;
        void draw(SDL_Renderer* m_renderer) override;
        void setColor(Color color, SDL_Renderer* m_renderer) override;
        void getValidMoves(State* boardState, int index) override;
};

