#pragma once
#include <SDL2/SDL.h>

#include "piece.h"

class Bishop : public Piece {
    public:
        Bishop();
        Bishop(int xPos, int yPos, Color color);
        ~Bishop() override;
        void draw(SDL_Renderer* m_renderer) override;
        void setColor(Color color, SDL_Renderer* m_renderer) override;
        void getValidMoves(int boardState[], int index) override;
};

