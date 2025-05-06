#pragma once
#include <SDL2/SDL.h>

#include "piece.h"

class Rook : public Piece {
    public:
        Rook();
        Rook(int xPos, int yPos, Color color);
        ~Rook() override;
        void draw(SDL_Renderer* m_renderer) override;
        void setColor(Color color, SDL_Renderer* m_renderer) override;
        void getValidMoves(int boardState[], int index) override;
};
