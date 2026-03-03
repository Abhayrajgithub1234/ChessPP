#include "queen.h"

#include "colors.h"

Queen::Queen() : Piece(0, 0, Color::NONE) {
}

Queen::Queen(int xPos, int yPos, Color color) : Piece(xPos, yPos, color) {
    // m_imageSurface = SDL_LoadBMP(
    //     color == Color::WHITE ? "../assets/Wpawn1.bmp" :
    //     "../assets/Bpawn1.bmp");
}

Queen::~Queen() {
}

void Queen::draw(SDL_Renderer* m_renderer) {
    if (!texture) return;
    int padding = size / 12;  // Smaller padding for queen
    SDL_Rect rect;
    rect.x = xPos + padding;
    rect.y = yPos + padding;
    rect.h = size - (padding * 2);
    rect.w = size - (padding * 2);
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}

void Queen::setColor(Color color, SDL_Renderer* m_renderer) {
    this->color = color;
    SDL_Surface* m_imageSurface =
        SDL_LoadBMP((color == Color::WHITE ? "assets/Wqueen.bmp"
                                           : "assets/Bqueen.bmp"));
    if (!m_imageSurface) {
        printf("Queen image not loaded: %s\n", SDL_GetError());
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

void Queen::getValidMoves(int boardState[], int index) {
    int KingPos = -1;
    int mask = ~(State::VALID | State::PROMOTION | State::CASTLE);

    if (this->color == Color::WHITE) {
        for (int i = 0; i < 64; i++) {
            if ((boardState[i] & mask) == State::WKING) {
                KingPos = i;
                break;
            }
        }
        if (KingPos < 0) return;
        // Rook-like moves (Straight)
        // Up
        for (int i = 1; i < 8; i++) {
            int mov = index - (8 * i);
            if (mov < 0) break;
            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'w', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] >= State::BROOK
                && boardState[mov] <= State::BPAWN
                && simulateMove(boardState, 'w', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }
        // Down
        for (int i = 1; i < 8; i++) {
            int mov = index + (8 * i);
            if (mov > 63) break;
            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'w', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] >= State::BROOK
                && boardState[mov] <= State::BPAWN
                && simulateMove(boardState, 'w', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }
        // Left
        if (index % 8 != 0) {
            for (int i = 1; i < 8; i++) {
                int mov = index - i;
                if (mov < 0 || mov % 8 == 7) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 0) continue;
                }
                if (boardState[mov] >= State::BROOK
                    && boardState[mov] <= State::BPAWN
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }
        // Right
        if (index % 8 != 7) {
            for (int i = 1; i < 8; i++) {
                int mov = index + i;
                if (mov > 63 || mov % 8 == 0) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 7) continue;
                }
                if (boardState[mov] >= State::BROOK
                    && boardState[mov] <= State::BPAWN
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }

        // Bishop-like moves (Diagonals)
        if (index % 8 != 7) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (7 * i);
                if (mov < 0 || mov % 8 == 0) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 7) continue;
                }
                if (boardState[mov] >= State::BROOK
                    && boardState[mov] <= State::BPAWN
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (9 * i);
                if (mov > 63 || mov % 8 == 0) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 7) continue;
                }
                if (boardState[mov] >= State::BROOK
                    && boardState[mov] <= State::BPAWN
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }

        if (index % 8 != 0) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (9 * i);
                if (mov < 0 || mov % 8 == 7) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 0) continue;
                }
                if (boardState[mov] >= State::BROOK
                    && boardState[mov] <= State::BPAWN
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (7 * i);
                if (mov > 63 || mov % 8 == 7) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 0) continue;
                }
                if (boardState[mov] >= State::BROOK
                    && boardState[mov] <= State::BPAWN
                    && simulateMove(boardState, 'w', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }
    } else {
        for (int i = 0; i < 64; i++) {
            if ((boardState[i] & mask) == State::BKING) {
                KingPos = i;
                break;
            }
        }
        if (KingPos < 0) return;
        // Logic for BLACK Queen (mirror of above)
        // Up
        for (int i = 1; i < 8; i++) {
            int mov = index - (8 * i);
            if (mov < 0) break;
            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'b', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] >= State::WROOK
                && boardState[mov] <= State::WPAWN
                && simulateMove(boardState, 'b', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }
        // Down
        for (int i = 1; i < 8; i++) {
            int mov = index + (8 * i);
            if (mov > 63) break;
            if (boardState[mov] == State::NONE
                && simulateMove(boardState, 'b', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
                continue;
            }
            if (boardState[mov] >= State::WROOK
                && boardState[mov] <= State::WPAWN
                && simulateMove(boardState, 'b', index, mov, KingPos)) {
                boardState[mov] |= State::VALID;
            }
            break;
        }
        // Left
        if (index % 8 != 0) {
            for (int i = 1; i < 8; i++) {
                int mov = index - i;
                if (mov < 0 || mov % 8 == 7) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 0) continue;
                }
                if (boardState[mov] >= State::WROOK
                    && boardState[mov] <= State::WPAWN
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }
        // Right
        if (index % 8 != 7) {
            for (int i = 1; i < 8; i++) {
                int mov = index + i;
                if (mov > 63 || mov % 8 == 0) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 7) continue;
                }
                if (boardState[mov] >= State::WROOK
                    && boardState[mov] <= State::WPAWN
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }

        // Bishop-like moves
        if (index % 8 != 7) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (7 * i);
                if (mov < 0 || mov % 8 == 0) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 7) continue;
                }
                if (boardState[mov] >= State::WROOK
                    && boardState[mov] <= State::WPAWN
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (9 * i);
                if (mov > 63 || mov % 8 == 0) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 7) continue;
                }
                if (boardState[mov] >= State::WROOK
                    && boardState[mov] <= State::WPAWN
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }

        if (index % 8 != 0) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (9 * i);
                if (mov < 0 || mov % 8 == 7) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 0) continue;
                }
                if (boardState[mov] >= State::WROOK
                    && boardState[mov] <= State::WPAWN
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (7 * i);
                if (mov > 63 || mov % 8 == 7) break;
                if (boardState[mov] == State::NONE
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                    if (mov % 8 != 0) continue;
                }
                if (boardState[mov] >= State::WROOK
                    && boardState[mov] <= State::WPAWN
                    && simulateMove(boardState, 'b', index, mov, KingPos)) {
                    boardState[mov] |= State::VALID;
                }
                break;
            }
        }
    }
}

