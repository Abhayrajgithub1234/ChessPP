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

static bool simulateMove(int BS[], char turn, int sourcePos, int destPos,
                         int kingPos) {
    bool isValid;
    int state = BS[destPos];
    BS[destPos] = BS[sourcePos];
    BS[sourcePos] = State::NONE;
    isValid = !Fen(BS).isCheck(kingPos, turn);
    // Undo the move to reuse the virtual board BS
    BS[sourcePos] = BS[destPos];
    BS[destPos] = state;

    return isValid;
}

void Pawn::getValidMoves(int boardState[], int index) {
    int KingPos;

    if (this->color == Color::BLACK) {
        for (int i = 0; i < 64; i++) {
            if (boardState[i] == State::BKING) KingPos = i;
        }

        if (index >= 8 && index <= 15) {
            if (boardState[index + 16] == State::NONE
                && boardState[index + 8] == State::NONE
                && simulateMove(boardState, 'b', index, index + 16, KingPos)) {
                boardState[index + 16] |= State::VALID;
            }
        }

        if (boardState[index + 8] == State::NONE
            && simulateMove(boardState, 'b', index, index + 8, KingPos)) {
            boardState[index + 8] |= State::VALID;
            if (index + 8 >= 56 && index + 8 <= 63) {
                boardState[index + 8] |= State::PROMOTION;
            }
        }

        if (index % 8 == 0) {
            if (boardState[index + 9] <= State::WPAWN
                && boardState[index + 9] >= State::WROOK
                && simulateMove(boardState, 'b', index, index + 9, KingPos)) {
                if (index + 9 >= 56 && index + 9 <= 63) {
                    boardState[index + 9] |= State::PROMOTION;
                }
                boardState[index + 9] |= State::VALID;
            }
        } else if (index % 8 == 7) {
            if (boardState[index + 7] <= State::WPAWN
                && boardState[index + 7] >= State::WROOK
                && simulateMove(boardState, 'b', index, index + 7, KingPos)) {
                if (index + 7 >= 56 && index + 7 <= 63) {
                    boardState[index + 7] |= State::PROMOTION;
                }
                boardState[index + 7] |= State::VALID;
            }
        } else {
            if (boardState[index + 9] <= State::WPAWN
                && boardState[index + 9] >= State::WROOK
                && simulateMove(boardState, 'b', index, index + 9, KingPos)) {
                if (index + 9 >= 56 && index + 9 <= 63) {
                    boardState[index + 9] |= State::PROMOTION;
                }
                boardState[index + 9] |= State::VALID;
            }
            if (boardState[index + 7] <= State::WPAWN
                && boardState[index + 7] >= State::WROOK
                && simulateMove(boardState, 'b', index, index + 7, KingPos)) {
                if (index + 7 >= 56 && index + 7 <= 63) {
                    boardState[index + 7] |= State::PROMOTION;
                }
                boardState[index + 7] |= State::VALID;
            }
        }

    } else {  // WHITE pawn
        for (int i = 0; i < 64; i++) {
            if (boardState[i] == State::WKING) KingPos = i;
        }

        if (index >= 48 && index <= 55) {
            if (boardState[index - 16] == State::NONE
                && boardState[index - 8] == State::NONE
                && simulateMove(boardState, 'w', index, index - 16, KingPos)) {
                boardState[index - 16] |= State::VALID;
            }
        }

        if (boardState[index - 8] == State::NONE
            && simulateMove(boardState, 'w', index, index - 8, KingPos)) {
            if (index - 8 >= 0 && index - 8 <= 7) {
                boardState[index - 8] |= State::PROMOTION;
            }
            boardState[index - 8] |= State::VALID;
        }

        if (index % 8 == 0) {
            if (boardState[index - 7] <= State::BPAWN
                && boardState[index - 7] >= State::BROOK
                && simulateMove(boardState, 'w', index, index - 7, KingPos)) {
                if (index - 7 >= 0 && index - 7 <= 7) {
                    boardState[index - 7] |= State::PROMOTION;
                }
                boardState[index - 7] |= State::VALID;
            }
        } else if (index % 8 == 7) {
            if (boardState[index - 9] <= State::BPAWN
                && boardState[index - 9] >= State::BROOK
                && simulateMove(boardState, 'w', index, index - 9, KingPos)) {
                if (index - 9 >= 0 && index - 9 <= 7) {
                    boardState[index - 9] |= State::PROMOTION;
                }
                boardState[index - 9] |= State::VALID;
            }
        } else {
            if (boardState[index - 9] <= State::BPAWN
                && boardState[index - 9] >= State::BROOK
                && simulateMove(boardState, 'w', index, index - 9, KingPos)) {
                if (index - 9 >= 0 && index - 9 <= 7) {
                    boardState[index - 9] |= State::PROMOTION;
                }
                boardState[index - 9] |= State::VALID;
            }
            if (boardState[index - 7] <= State::BPAWN
                && boardState[index - 7] >= State::BROOK
                && simulateMove(boardState, 'w', index, index - 7, KingPos)) {
                if (index - 7 >= 0 && index - 7 <= 7) {
                    boardState[index - 7] |= State::PROMOTION;
                }
                boardState[index - 7] |= State::VALID;
            }
        }
    }
}

