#include "board.h"

#include <SDL3/SDL_video.h>

Board::Board(int width, int height, SDL_Window* window)
    // : fen(Fen((const char*)"8/P7/8/8/8/8/8/k6K", 'w', Fen::NONE)) {
    : fen(Fen((const char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w',
              Fen::ALL)) {
    this->width = width;
    this->height = height;
    this->m_renderer = SDL_CreateRenderer(window, -1, 0);
    this->m_windowSurface = SDL_GetWindowSurface(window);
    this->selectedSquare = nullptr;
    this->selectedSquareIndex = -1;
    this->fen.getBoardState(this->boardState);

    int colorIndex = 0;
    for (int i = 0; i < 8; i++, colorIndex++) {
        for (int j = 0; j < 8; j++) {
            Color color = colorIndex++ % 2 == 0 ? Color::WHITE : Color::BLACK;
            S[i * 8 + j].set_values(j * 75, i * 75, 75, color, m_renderer);
        }
    }

    for (int i = 0; i < 64; ++i) {
        switch (boardState[i]) {
            case State::WROOK: {
                WR[wri].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WR[wri++]);
            } break;
            case State::WKNIGHT: {
                WN[wni].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WN[wni++]);
            } break;
            case State::WBISHOP: {
                WB[wbi].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WB[wbi++]);
            } break;
            case State::WQUEEN: {
                WQ[wqi].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WQ[wqi++]);
            } break;
            case State::WKING: {
                WK.setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WK);
            } break;
            case State::WPAWN: {
                WP[wpi].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WP[wpi++]);
            } break;
            case State::BROOK: {
                BR[bri].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BR[bri++]);
            } break;
            case State::BKNIGHT: {
                BN[bni].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BN[bni++]);
            } break;
            case State::BBISHOP: {
                BB[bbi].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BB[bbi++]);
            } break;
            case State::BQUEEN: {
                BQ[bqi].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BQ[bqi++]);
            } break;
            case State::BKING: {
                BK.setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BK);
            } break;
            case State::BPAWN: {
                BP[bpi].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BP[bpi++]);
            } break;
            default:
                break;
        }
    }

    const char* imgs[] = {"../assets/Wrook.bmp",   "../assets/Wbishop.bmp",
                          "../assets/Wknight.bmp", "../assets/Wqueen.bmp",
                          "../assets/Brook.bmp",   "../assets/Bbishop.bmp",
                          "../assets/Bknight.bmp", "../assets/Bqueen.bmp"};

    for (int i = 0; i < 8; ++i) {
        SDL_Surface* surface = SDL_LoadBMP(imgs[i]);
        this->promotionPieces[i] =
            SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_FreeSurface(surface);
    }

    int w, h;
    int ww, wh;
    SDL_GetWindowSize(window, &ww, &wh);
    SDL_QueryTexture(promotionPieces[0], NULL, NULL, &w, &h);

    this->popupRect.w = (w * 4) + 10;
    this->popupRect.h = (h) + 10;
    this->popupRect.x = (ww / 2) - (this->popupRect.w / 2);
    this->popupRect.y = (wh / 2) - (this->popupRect.h / 2);
    for (int i = 0; i < 4; ++i)
        this->popupImgRects[i] = {popupRect.x + (i * w), popupRect.y, w, h};
}

void Board::draw() {
    SDL_RenderClear(m_renderer);

    if (!end) {
        for (int i = 0; i < 64; i++) {
            S[i].draw();
        }

        if (this->showPopup) {
            showOptions();
        }
    } else {
    }

    SDL_RenderPresent(m_renderer);
}

Board::~Board() {
    for (int i = 0; i < 8; ++i) SDL_DestroyTexture(promotionPieces[i]);

    SDL_DestroyRenderer(m_renderer);
    SDL_FreeSurface(m_windowSurface);
}

void Board::clearHighlighted() {
    for (int i = 0; i < 64; i++) {
        S[i].isHighlighted = false;
    }
}

void Board::movePiece(Square* sq, int index) {
    sq->setPiece(selectedSquare->getPiece());
    boardState[index] = boardState[selectedSquareIndex];
    selectedSquare->clearPiece();
    boardState[selectedSquareIndex] = State::NONE;
    selectedSquare = nullptr;
    selectedSquareIndex = -1;
}

static bool hasValidMoves(int boardState[], char turn, Square S[]) {
    int BS[64];

    for (int i = 0; i < 64; i++) {
        BS[i] = boardState[i];
    }

    for (int i = 0; i < 64; i++) {
        if (turn == 'w') {
            if (BS[i] <= State::WPAWN && BS[i] >= State::WROOK) {
                S[i].getPiece()->getValidMoves(BS, i);
                for (int i = 0; i < 64; i++) {
                    if (BS[i] & State::VALID) {
                        return true;
                    }
                }
            }
        } else {
            if (BS[i] <= State::BPAWN && BS[i] >= State::BROOK) {
                S[i].getPiece()->getValidMoves(BS, i);
                for (int i = 0; i < 64; i++) {
                    if (BS[i] & State::VALID) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Board::showOptions() {
    int width, height;
    SDL_SetRenderDrawColor(m_renderer, 72, 245, 66, 255);

    SDL_RenderFillRect(m_renderer, &this->popupRect);

    int base = (this->popupColor == Color::BLACK) ? 4 : 0;

    for (int i = 0; i < 4; ++i) {
        SDL_RenderCopy(m_renderer, promotionPieces[base + i], NULL,
                       &this->popupImgRects[i]);
    }
}

void Board::clickedPosition(int x, int y) {
    int j = x / 75;
    int i = y / 75;
    int index = i * 8 + j;

    if (this->showPopup) {
        SDL_Point p = {x, y};
        if (SDL_PointInRect(&p, &popupRect)) {
            for (int i = 0; i < 4; ++i) {
                if (SDL_PointInRect(&p, &popupImgRects[i])) {
                    // printf("%d\n", i);
                    int Wpos;
                    int Bpos;
                    for (int i = 0; i < 64; i++) {
                        if (this->boardState[i] == State::WKING) {
                            Wpos = i;
                        }
                        if (this->boardState[i] == State::BKING) {
                            Bpos = i;
                        }
                    }

                    clearHighlighted();

                    // Assume we have no check for both the kings
                    S[Bpos].isCheck = false;
                    S[Wpos].isCheck = false;

                    this->showPopup = false;
                    switch (i) {
                        case 0:
                            if (turn % 2) {
                                boardState[promote_index] = State::BROOK;
                                BR[bri].setColor(Color::BLACK, m_renderer);
                                S[promote_index].setPiece(&BR[bri++]);
                            } else {
                                boardState[promote_index] = State::WROOK;
                                WR[wri].setColor(Color::WHITE, m_renderer);
                                S[promote_index].setPiece(&WR[wri++]);
                            }
                            break;
                        case 1:
                            if (turn % 2) {
                                boardState[promote_index] = State::BBISHOP;
                                BB[bbi].setColor(Color::BLACK, m_renderer);
                                S[promote_index].setPiece(&BB[bbi++]);
                            } else {
                                boardState[promote_index] = State::WBISHOP;
                                WB[wbi].setColor(Color::WHITE, m_renderer);
                                S[promote_index].setPiece(&WB[wbi++]);
                            }
                            break;
                        case 2:
                            if (turn % 2) {
                                boardState[promote_index] = State::BKNIGHT;
                                BN[bni].setColor(Color::BLACK, m_renderer);
                                S[promote_index].setPiece(&BN[bni++]);
                            } else {
                                boardState[promote_index] = State::WKNIGHT;
                                WN[wni].setColor(Color::WHITE, m_renderer);
                                S[promote_index].setPiece(&WN[wni++]);
                            }
                            break;
                        case 3:
                            if (turn % 2) {
                                boardState[promote_index] = State::BQUEEN;
                                BQ[bqi].setColor(Color::BLACK, m_renderer);
                                S[promote_index].setPiece(&BQ[bqi++]);
                            } else {
                                boardState[promote_index] = State::WQUEEN;
                                WQ[wqi].setColor(Color::WHITE, m_renderer);
                                S[promote_index].setPiece(&WQ[wqi++]);
                            }
                            break;
                        default:
                            break;
                    }
                    this->turn++;

                    // Copies the FE-Notation to boardState
                    Fen f(this->boardState);

                    bool haveValidMoves = hasValidMoves(
                        boardState, turn % 2 ? 'b' : 'w', this->S);

                    // Check if king has "Check"
                    if (f.isCheck(turn % 2 ? Bpos : Wpos,
                                  turn % 2 ? 'b' : 'w')) {
                        S[turn % 2 ? Bpos : Wpos].isCheck = true;
                        if (!haveValidMoves) {
                            printf("CHECK MATE\n");
                            if (turn % 2) {
                                printf("WHITE WINS!!\n");
                            } else {
                                printf("BLACK WINS!!\n");
                            }
                        }
                    } else if (!haveValidMoves) {
                        printf("StaleMate\n");
                    }
                }
            }
        }
        return;
    }

    Square* sq = &S[index];

    if (sq->isHighlighted) {
        // Calculates the position of the both of the kings
        int Wpos;
        int Bpos;
        for (int i = 0; i < 64; i++) {
            if (this->boardState[i] == State::WKING) {
                Wpos = i;
            }
            if (this->boardState[i] == State::BKING) {
                Bpos = i;
            }
        }

        clearHighlighted();

        // Assume we have no check for both the kings
        S[Bpos].isCheck = false;
        S[Wpos].isCheck = false;

        // Move the piece
        movePiece(sq, index);

        if (this->promotion) {
            this->promotion = false;
            this->popupColor = turn % 2 ? Color::BLACK : Color::WHITE;
            this->showPopup = true;
            this->promote_index = index;
            return;
        }
        this->turn++;

        // Copies the FE-Notation to boardState
        Fen f(this->boardState);

        bool haveValidMoves =
            hasValidMoves(boardState, turn % 2 ? 'b' : 'w', this->S);

        // Check if king has "Check"
        if (f.isCheck(turn % 2 ? Bpos : Wpos, turn % 2 ? 'b' : 'w')) {
            S[turn % 2 ? Bpos : Wpos].isCheck = true;
            if (!haveValidMoves) {
                printf("CHECK MATE\n");
                if (turn % 2) {
                    printf("WHITE WINS!!\n");
                    won = Color::WHITE;
                } else {
                    printf("BLACK WINS!!\n");
                    won = Color::BLACK;
                }
                end = true;
            }
        } else if (!haveValidMoves) {
            printf("StaleMate\n");
            end = true;
            won = Color::NONE;
        }

        return;
    }

    clearHighlighted();

    if (!sq->empty) {
        // To have a systematic turns
        if ((boardState[index] >= State::WROOK
             && boardState[index] <= State ::WPAWN && turn % 2 != 0)
            || (boardState[index] >= State::BROOK
                && boardState[index] <= State ::BPAWN && turn % 2 == 0))
            return;

        selectedSquare = sq;
        selectedSquareIndex = index;
        sq->getPiece()->getValidMoves(boardState, index);
        for (int i = 0; i < 64; i++) {
            if (boardState[i] & State::PROMOTION) {
                printf("%s Pawn can be promoted!!\n",
                       turn % 2 ? "BLACK" : "WHITE");
                this->promotion = true;
            }
            if (boardState[i] & State::VALID) {
                boardState[i] &= ~(State::VALID | State::PROMOTION);
                S[i].isHighlighted = true;
            }
        }
    } else {
        selectedSquare = nullptr;
        selectedSquareIndex = -1;
    }
}
