#include "pawn.h"

#include "colors.h"

Pawn::Pawn() : Piece(0, 0, Color::NONE) {
}

Pawn::Pawn(int xPos, int yPos, Color color) : Piece(xPos, yPos, color) {
}

Pawn::~Pawn() {
}

void Pawn::draw(SDL_Renderer* m_renderer) {
    SDL_Rect rect;
    rect.x = xPos;
    rect.y = yPos;
    rect.h = 75;
    rect.w = 75;
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}

void Pawn::setColor(Color color, SDL_Renderer* m_renderer) {
    this->color = color;
    SDL_Surface* m_imageSurface = SDL_LoadBMP((
        color == Color::WHITE ? "../assets/Wpawn.bmp" : "../assets/Bpawn.bmp"));
    if (!m_imageSurface) printf("Image not loaded!!\n");
    texture = SDL_CreateTextureFromSurface(m_renderer, m_imageSurface);
    SDL_FreeSurface(m_imageSurface);
}

void Pawn::getValidMoves(int boardState[], int index) {
    if (this->color == Color::BLACK) {
        if (index >= 8 && index <= 15) {
            if (boardState[index + 16] == State::NONE
                && boardState[index + 8] == State::NONE)
                boardState[index + 16] |= State::VALID;
        }
        if (boardState[index + 8] == State::NONE) {
            boardState[index + 8] |= State::VALID;
        }
        if (index % 8 == 0) {
            if (boardState[index + 9] <= State::WPAWN
                && boardState[index + 9] >= State::WROOK) {
                boardState[index + 9] |= State::VALID;
            }
        } else if (index % 8 == 7) {
            if (boardState[index + 7] <= State::WPAWN
                && boardState[index + 7] >= State::WROOK) {
                boardState[index + 7] |= State::VALID;
            }
        } else {
            if (boardState[index + 9] <= State::WPAWN
                && boardState[index + 9] >= State::WROOK) {
                boardState[index + 9] |= State::VALID;
            }
            if (boardState[index + 7] <= State::WPAWN
                && boardState[index + 7] >= State::WROOK) {
                boardState[index + 7] |= State::VALID;
            }
        }

    } else {
        if (index >= 48 && index <= 55) {
            if (boardState[index - 16] == State::NONE
                && boardState[index - 8] == State::NONE) {
                boardState[index - 16] |= State::VALID;
            }
        }
        if (boardState[index - 8] == State::NONE) {
            boardState[index - 8] |= State::VALID;
        }

        if (index % 8 == 0) {
            if (boardState[index - 7] <= State::BPAWN
                && boardState[index - 7] >= State::BROOK) {
                boardState[index - 7] |= State::VALID;
            }
        } else if (index % 8 == 7) {
            if (boardState[index - 9] <= State::BPAWN
                && boardState[index - 9] >= State::BROOK) {
                boardState[index - 9] |= State::VALID;
            }
        } else {
            if (boardState[index - 9] <= State::BPAWN
                && boardState[index - 9] >= State::BROOK) {
                boardState[index - 9] |= State::VALID;
            }
            if (boardState[index - 7] <= State::BPAWN
                && boardState[index - 7] >= State::BROOK) {
                boardState[index - 7] |= State::VALID;
            }
        }
    }
}

