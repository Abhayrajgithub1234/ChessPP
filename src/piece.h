#pragma once

#include <SDL2/SDL.h>

#include "colors.h"
#include "fen.h"
#include "state.h"
#include "constants.h"

class Piece {
    protected:
        int xPos;
        int yPos;
        int size;
        Color color;
        SDL_Texture* texture;

    public:
        virtual void draw(SDL_Renderer* m_renderer) = 0;

        void setPosition(int xPos, int yPos, int size = ChessConstants::DEFAULT_SQUARE_SIZE) {
            this->xPos = xPos;
            this->yPos = yPos;
            this->size = size;
        }

        virtual void setColor(Color color, SDL_Renderer* m_renderer) = 0;

        Piece(int xPos, int yPos, Color color) {
            this->xPos = xPos;
            this->yPos = yPos;
            this->color = color;
            this->size = ChessConstants::DEFAULT_SQUARE_SIZE;
            this->texture = nullptr;
        }

        virtual void getValidMoves(int boardState[], int index) = 0;

        virtual ~Piece() {

        };
};
