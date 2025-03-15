#pragma once

#include <SDL2/SDL.h>

#include "colors.h"

class Piece {
    protected:
        int xPos;
        int yPos;
        Color color;
        SDL_Texture* texture;

    public:
        virtual void draw(SDL_Renderer* m_renderer) = 0;

        void setPosition(int xPos, int yPos) {
            this->xPos = xPos;
            this->yPos = yPos;
        }

        virtual void setColor(Color color, SDL_Renderer* m_renderer) = 0;

        Piece(int xPos, int yPos, Color color) {
            this->xPos = xPos;
            this->yPos = yPos;
            this->color = color;
        }

        virtual ~Piece() {

        };
};
