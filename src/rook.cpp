#include "rook.h"

#include "colors.h"

Rook::Rook() : Piece(0, 0, Color::NONE) {
}

Rook::Rook(int xPos, int yPos, Color color) : Piece(xPos, yPos, color) {
    // m_imageSurface = SDL_LoadBMP(
    //     color == Color::WHITE ? "../assets/Wpawn1.bmp" :
    //     "../assets/Bpawn1.bmp");
}

Rook::~Rook() {
}

void Rook::draw(SDL_Renderer* m_renderer) {
    if (!texture) return;
    int padding = size / 10;
    SDL_Rect rect;
    rect.x = xPos + padding;
    rect.y = yPos + padding;
    rect.h = size - (padding * 2);
    rect.w = size - (padding * 2);
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}

void Rook::setColor(Color color, SDL_Renderer* m_renderer) {
    this->color = color;
    SDL_Surface* m_imageSurface = SDL_LoadBMP((
        color == Color::WHITE ? "assets/Wrook.bmp" : "assets/Brook.bmp"));
    if (!m_imageSurface) {
        printf("Rook image not loaded: %s\n", SDL_GetError());
        return;
    }
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

void Rook::getValidMoves(int boardState[], int index) {
    int kingPos = -1;
    int mask = ~(State::VALID | State::PROMOTION | State::CASTLE);
    if (this->color == Color::WHITE) {
        for (int i = 0; i < 64; i++) {
            if ((boardState[i] & mask) == State::WKING) kingPos = i;
        }
        if (kingPos < 0) return;
        // Move Up
        for (int i = 1; i < 8; i++) {
            int mov = index - 8 * i;
            if (mov < 0) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::BPAWN
                && boardState[mov] >= State::BROOK
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }

        // Move Down
        for (int i = 1; i < 8; i++) {
            int mov = index + 8 * i;
            if (mov > 63) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::BPAWN
                && boardState[mov] >= State::BROOK
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }

        // Move Left
        for (int i = 1; i < 8; i++) {
            int mov = index - i;
            if ((mov / 8) != (index / 8)) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::BPAWN
                && boardState[mov] >= State::BROOK
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }

        // Move Right
        for (int i = 1; i < 8; i++) {
            int mov = index + i;
            if ((mov / 8) != (index / 8)) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::BPAWN
                && boardState[mov] >= State::BROOK
                && simulateMove(boardState, 'w', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }

    } else {
        // BLACK ROOK

        for (int i = 0; i < 64; i++) {
            if ((boardState[i] & mask) == State::BKING) kingPos = i;
        }
        if (kingPos < 0) return;

        // Move Up
        for (int i = 1; i < 8; i++) {
            int mov = index - 8 * i;
            if (mov < 0) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::WPAWN
                && boardState[mov] >= State::WROOK
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }

        // Move Down
        for (int i = 1; i < 8; i++) {
            int mov = index + 8 * i;
            if (mov > 63) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::WPAWN
                && boardState[mov] >= State::WROOK
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }

        // Move Left
        for (int i = 1; i < 8; i++) {
            int mov = index - i;
            if ((mov / 8) != (index / 8)) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::WPAWN
                && boardState[mov] >= State::WROOK
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }

        // Move Right
        for (int i = 1; i < 8; i++) {
            int mov = index + i;
            if ((mov / 8) != (index / 8)) break;

            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] <= State::WPAWN
                && boardState[mov] >= State::WROOK
                && simulateMove(boardState, 'b', index, mov, kingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }
    }
}

