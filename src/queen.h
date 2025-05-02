#pragma once
#include <SDL2/SDL.h>

#include "piece.h"

class Queen : public Piece {
    public:
        Queen();
        Queen(int xPos, int yPos, Color color);
        ~Queen() override;
        void draw(SDL_Renderer* m_renderer) override;
        void setColor(Color color, SDL_Renderer* m_renderer) override;
        void getValidMoves(int boardState[], int index) override;
};

