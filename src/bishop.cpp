#include "bishop.h"

#include "colors.h"

Bishop::Bishop() : Piece(0, 0, Color::NONE) {
}

Bishop::Bishop(int xPos, int yPos, Color color) : Piece(xPos, yPos, color) {
    // m_imageSurface = SDL_LoadBMP(
    //     color == Color::WHITE ? "../assets/Wpawn1.bmp" :
    //     "../assets/Bpawn1.bmp");
}

Bishop::~Bishop() {
}

void Bishop::draw(SDL_Renderer* m_renderer) {
    SDL_Rect rect;
    rect.x = xPos;
    rect.y = yPos;
    rect.h = 75;
    rect.w = 75;
    // SDL_BlitSurface(m_imageSurface, NULL, m_windowSurface, &rect);
    SDL_RenderCopy(m_renderer, texture, NULL, &rect);
}

void Bishop::setColor(Color color, SDL_Renderer* m_renderer) {
    this->color = color;
    SDL_Surface* m_imageSurface =
        SDL_LoadBMP((color == Color::WHITE ? "../assets/Wbishop.bmp"
                                           : "../assets/Bbishop.bmp"));
    if (!m_imageSurface) printf("Image not loaded!!\n");
    texture = SDL_CreateTextureFromSurface(m_renderer, m_imageSurface);
    SDL_FreeSurface(m_imageSurface);
}

void Bishop::getValidMoves(State* boardState, int index) {
    if (this->color == Color::WHITE) {
        if (index % 8 != 7) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (7 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 7) continue;
                }

                if ((boardState[mov] <= State::BPAWN
                     && boardState[mov] >= State::BROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (9 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 7) continue;
                }

                if ((boardState[mov] <= State::BPAWN
                     && boardState[mov] >= State::BROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }
        }

        if (index % 8 != 0) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (9 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 0) continue;
                }

                if ((boardState[mov] <= State::BPAWN
                     && boardState[mov] >= State::BROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (7 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 0) continue;
                }

                if ((boardState[mov] <= State::BPAWN
                     && boardState[mov] >= State::BROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }
        }
    } else {
        if (index % 8 != 7) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (7 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 7) continue;
                }

                if ((boardState[mov] <= State::WPAWN
                     && boardState[mov] >= State::WROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (9 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 7) continue;
                }

                if ((boardState[mov] <= State::WPAWN
                     && boardState[mov] >= State::WROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }
        }

        if (index % 8 != 0) {
            for (int i = 1; i < 8; i++) {
                int mov = index - (9 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 0) continue;
                }

                if ((boardState[mov] <= State::WPAWN
                     && boardState[mov] >= State::WROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }

            for (int i = 1; i < 8; i++) {
                int mov = index + (7 * i);
                if (boardState[mov] == State::NONE) {
                    boardState[mov] = State::VALID;
                    if (mov % 8 != 0) continue;
                }
                if ((boardState[mov] <= State::WPAWN
                     && boardState[mov] >= State::WROOK)) {
                    boardState[mov] = State::VALID;
                }
                break;
            }
        }
    }
}
