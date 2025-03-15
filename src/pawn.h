#pragma once
#include <SDL2/SDL.h>

#include "piece.h"

class Pawn : public Piece {
    public:
        Pawn();
        Pawn(int xPos, int yPos, Color color);
        ~Pawn() override;
        void draw(SDL_Renderer* m_renderer) override;
        void setColor(Color color, SDL_Renderer* m_renderer) override;
        void getValidMoves(State* boardState, int index) override;
};

