#include "king.h"

#include "colors.h"

King::King() : Piece(0, 0, Color::NONE) {
}

King::King(int xPos, int yPos, Color color) : Piece(xPos, yPos, color) {
    // m_imageSurface = SDL_LoadBMP(
    //     color == Color::WHITE ? "../assets/Wpawn1.bmp" :
    //     "../assets/Bpawn1.bmp");
}

King::~King() {
}

void King::draw(SDL_Renderer* m_renderer) {
    SDL_Rect rect;
    rect.x = xPos;
    rect.y = yPos;
    rect.h = 75;
    rect.w = 75;
    // SDL_BlitSurface(m_imageSurface, NULL, m_windowSurface, &rect);
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}

void King::setColor(Color color, SDL_Renderer* m_renderer) {
    this->color = color;
    SDL_Surface* m_imageSurface = SDL_LoadBMP((
        color == Color::WHITE ? "../assets/Wking.bmp" : "../assets/Bking.bmp"));
    if (!m_imageSurface) printf("Image not loaded!!\n");
    texture = SDL_CreateTextureFromSurface(m_renderer, m_imageSurface);
    SDL_FreeSurface(m_imageSurface);
}

void King::getValidMoves(int boardState[], int index) {
    if (this->color == Color::WHITE) {
        // Top row
        if (index >= 8) {
            if (index % 8 != 0) {
                int mov = index - 9;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::BPAWN
                        && boardState[mov] >= State::BROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
            int mov = index - 8;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::BPAWN
                    && boardState[mov] >= State::BROOK)) {
                boardState[mov] |= State::VALID;
            }
            if (index % 8 != 7) {
                int mov = index - 7;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::BPAWN
                        && boardState[mov] >= State::BROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
        }

        // Middle row (left and right)
        if (index % 8 != 0) {
            int mov = index - 1;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::BPAWN
                    && boardState[mov] >= State::BROOK)) {
                boardState[mov] |= State::VALID;
            }
        }

        if (index % 8 != 7) {
            int mov = index + 1;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::BPAWN
                    && boardState[mov] >= State::BROOK)) {
                boardState[mov] |= State::VALID;
            }
        }

        // Bottom row
        if (index < 56) {
            if (index % 8 != 0) {
                int mov = index + 7;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::BPAWN
                        && boardState[mov] >= State::BROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
            int mov = index + 8;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::BPAWN
                    && boardState[mov] >= State::BROOK)) {
                boardState[mov] |= State::VALID;
            }
            if (index % 8 != 7) {
                int mov = index + 9;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::BPAWN
                        && boardState[mov] >= State::BROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
        }
    } else {  // BLACK KING
        // Top row
        if (index >= 8) {
            if (index % 8 != 0) {
                int mov = index - 9;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::WPAWN
                        && boardState[mov] >= State::WROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
            int mov = index - 8;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::WPAWN
                    && boardState[mov] >= State::WROOK)) {
                boardState[mov] |= State::VALID;
            }
            if (index % 8 != 7) {
                int mov = index - 7;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::WPAWN
                        && boardState[mov] >= State::WROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
        }

        // Middle row
        if (index % 8 != 0) {
            int mov = index - 1;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::WPAWN
                    && boardState[mov] >= State::WROOK)) {
                boardState[mov] |= State::VALID;
            }
        }

        if (index % 8 != 7) {
            int mov = index + 1;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::WPAWN
                    && boardState[mov] >= State::WROOK)) {
                boardState[mov] |= State::VALID;
            }
        }

        // Bottom row
        if (index < 56) {
            if (index % 8 != 0) {
                int mov = index + 7;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::WPAWN
                        && boardState[mov] >= State::WROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
            int mov = index + 8;
            if (boardState[mov] == State::NONE
                || (boardState[mov] <= State::WPAWN
                    && boardState[mov] >= State::WROOK)) {
                boardState[mov] |= State::VALID;
            }
            if (index % 8 != 7) {
                int mov = index + 9;
                if (boardState[mov] == State::NONE
                    || (boardState[mov] <= State::WPAWN
                        && boardState[mov] >= State::WROOK)) {
                    boardState[mov] |= State::VALID;
                }
            }
        }
    }
}

