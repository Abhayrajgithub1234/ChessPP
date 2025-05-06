#include "board.h"

Board::Board(int width, int height, SDL_Window* window)
    // : fen(Fen((const char*)"8/8/8/8/8/8/8/K1k4q", 'b', Fen::NONE)) {
    : fen(Fen((const char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w',
              Fen::ALL)) {
    this->width = width;
    this->height = height;
    this->m_renderer = SDL_CreateRenderer(window, -1, 0);
    this->m_windowSurface = SDL_GetWindowSurface(window);
    this->selectedSquare = nullptr;
    this->selectedSquareIndex = -1;
    this->fen.getBoardState(this->boardState);

    // Set the blend mode thing
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

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

    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int w, h;
    SDL_QueryTexture(promotionPieces[0], NULL, NULL, &w, &h);

    this->popupRect.w = (w * 4) + 10;
    this->popupRect.h = (h) + 10;
    this->popupRect.x = (windowWidth / 2) - (this->popupRect.w / 2);
    this->popupRect.y = (windowHeight / 2) - (this->popupRect.h / 2);
    for (int i = 0; i < 4; ++i)
        this->popupImgRects[i] = {popupRect.x + (i * w), popupRect.y, w, h};
}

void Board::draw() {
    SDL_RenderClear(m_renderer);

    for (int i = 0; i < 64; i++) S[i].draw();

    if (this->showPopup) showOptions();

    if (end) drawGameEndScreen();

    SDL_RenderPresent(m_renderer);
}

void Board::drawGameEndScreen() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 80);

    SDL_Rect overlay;
    overlay.w = windowWidth;
    overlay.h = windowHeight / 3;
    overlay.x = (windowWidth / 2) - (overlay.w / 2);
    overlay.y = (windowHeight / 2) - (overlay.h / 2);

    SDL_RenderFillRect(m_renderer, &overlay);

    const char* resultText = this->won == Color::BLACK ? "Black WON!"
                           : this->won == Color::WHITE ? "White WON!"
                                                       : "STALEMATE!";

    // Render text (use SDL_ttf)
    TTF_Font* font = TTF_OpenFont("../assets/RobotoMonoNerdFont-Bold.ttf", 48);
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, resultText, white);
    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(m_renderer, textSurface);

    SDL_Rect textRect = {(windowWidth - textSurface->w) / 2,
                         (windowHeight - textSurface->h) / 2, textSurface->w,
                         textSurface->h};
    SDL_RenderCopy(m_renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

Board::~Board() {
    for (int i = 0; i < 8; ++i) SDL_DestroyTexture(promotionPieces[i]);

    SDL_DestroyRenderer(m_renderer);
    SDL_FreeSurface(m_windowSurface);
}

void Board::clearHighlighted() {
    for (int i = 0; i < 64; i++) {
        S[i].isHighlighted = false;
        boardState[i] &= ~State::CASTLE;
    }
}

void Board::movePiece(Square* sq, int index) {
    switch (boardState[selectedSquareIndex] & ~State::CASTLE) {
        case State::BROOK:
            if (index == 0) this->castling &= ~Fen::BLC;
            else if (index == 7) this->castling &= ~Fen::BSC;
            break;
        case State::BKING:
            this->castling &= ~Fen::BCASTLE;
            break;
        case State::WROOK:
            if (index == 56) this->castling &= ~Fen::WLC;
            else if (index == 63) this->castling &= ~Fen::WSC;
            break;
        case State::WKING:
            this->castling &= ~Fen::WCASTLE;
            break;
        default:
            break;
    }

    if (boardState[index] & State::LCASTLE) {
        boardState[selectedSquareIndex - 1] =
            boardState[selectedSquareIndex - 4];
        boardState[selectedSquareIndex - 4] = State::NONE;
        this->S[selectedSquareIndex - 1].setPiece(
            this->S[selectedSquareIndex - 4].getPiece());
        this->S[selectedSquareIndex - 4].clearPiece();
    }

    if (boardState[index] & State::SCASTLE) {
        boardState[selectedSquareIndex + 1] =
            boardState[selectedSquareIndex + 3];
        boardState[selectedSquareIndex + 3] = State::NONE;
        this->S[selectedSquareIndex + 1].setPiece(
            this->S[selectedSquareIndex + 3].getPiece());
        this->S[selectedSquareIndex + 3].clearPiece();
    }

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

bool Board::popWindow(int x, int y) {
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
        return true;
    }

    return false;
}

void Board::clickedPosition(int x, int y) {
    int j = x / 75;
    int i = y / 75;
    int index = i * 8 + j;

    if (popWindow(x, y)) return;

    Square* sq = &S[index];

    if (sq->isHighlighted) {
        // Calculates the position of the both of the kings
        int Wpos;
        int Bpos;
        for (int i = 0; i < 64; i++) {
            if ((this->boardState[i] & ~State::CASTLE) == State::WKING) {
                Wpos = i;
            }
            if ((this->boardState[i] & ~State::CASTLE) == State::BKING) {
                Bpos = i;
            }
        }

        // Assume we have no check for both the kings
        S[Bpos].isCheck = false;
        S[Wpos].isCheck = false;

        // Move the piece
        movePiece(sq, index);

        clearHighlighted();

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

        castlingMoves(index);

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

void Board::castlingMoves(int index) {
    int BS[64];

    for (int i = 0; i < 64; i++) BS[i] = boardState[i];

    if (boardState[index] == State::BKING
        && !Fen(boardState).isCheck(index, 'b')) {
        if (this->castling & Fen::BLC) {
            bool exit = false;
            for (int i = 1; i < index; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index - 1] = State::BROOK;
                BS[index - 2] = State::BKING;
                BS[index - 4] = State::NONE;
                if (!Fen(BS).isCheck(index - 2, 'b'))
                    boardState[index - 2] |= State::VALID | State::LCASTLE;
                BS[index] = State::BKING;
                BS[index - 1] = State::NONE;
                BS[index - 2] = State::NONE;
                BS[index - 4] = State::BROOK;
            }
        }

        if (this->castling & Fen::BSC) {
            bool exit = false;
            for (int i = index + 1; i < 7; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index + 1] = State::BROOK;
                BS[index + 2] = State::BKING;
                BS[index + 3] = State::NONE;
                if (!Fen(BS).isCheck(index + 2, 'b'))
                    boardState[index + 2] |= State::VALID | State::SCASTLE;
                BS[index] = State::BKING;
                BS[index + 1] = State::NONE;
                BS[index + 2] = State::NONE;
                BS[index + 3] = State::BROOK;
            }
        }
    } else if (boardState[index] == State::WKING
               && !Fen(boardState).isCheck(index, 'w')) {
        if (this->castling & Fen::WLC) {
            bool exit = false;
            for (int i = 57; i < index; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index - 1] = State::WROOK;
                BS[index - 2] = State::WKING;
                BS[index - 4] = State::NONE;
                if (!Fen(BS).isCheck(index - 2, 'w'))
                    boardState[index - 2] |= State::VALID | State::LCASTLE;
                BS[index] = State::WKING;
                BS[index - 1] = State::NONE;
                BS[index - 2] = State::NONE;
                BS[index - 4] = State::WROOK;
            }
        }

        if (this->castling & Fen::WSC) {
            bool exit = false;
            for (int i = index + 1; i < 63; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index + 1] = State::WROOK;
                BS[index + 2] = State::WKING;
                BS[index + 3] = State::NONE;
                if (!Fen(BS).isCheck(index + 2, 'w'))
                    boardState[index + 2] |= State::VALID | State::SCASTLE;
                BS[index] = State::WKING;
                BS[index + 1] = State::NONE;
                BS[index + 2] = State::NONE;
                BS[index + 3] = State::WROOK;
            }
        }
    }
}
