#include "board.h"

Board::Board(int width, int height, SDL_Window* window)
    : fen(Fen((const char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w',
              Fen::BLC | Fen::BSC | Fen::WLC | Fen::WSC)) {
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

    for (int i = 0; i < 4; i++) {
        R[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        KN[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        B[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        if (i < 2) {
            int index = 0 + ((i % 2) * 8 * 7);
            S[index].setPiece(&R[i]);

            index++;
            S[index].setPiece(&KN[i]);

            index++;
            S[index].setPiece(&B[i]);

        } else {
            int index = 5 + ((i % 2) * 8 * 7);
            S[index].setPiece(&B[i]);

            index++;
            S[index].setPiece(&KN[i]);

            index++;
            S[index].setPiece(&R[i]);
        }
    }

    for (int i = 0; i < 2; i++) {
        int index = 3 + ((i % 2) * 8 * 7);
        K[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        Q[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        S[index].setPiece(&Q[i]);

        index++;
        S[index].setPiece(&K[i]);
    }

    for (int i = 0; i < 8; i++) {
        int index = i + 8;
        P[i].setColor(Color::BLACK, m_renderer);
        S[index].setPiece(&P[i]);
    }

    for (int i = 8; i < 16; i++) {
        int index = i + 8 * 5;
        P[i].setColor(Color::WHITE, m_renderer);
        S[index].setPiece(&P[i]);
    }

    int j = 1;
    for (int i = 63; i >= 0; i--) {
        int SetIndex = 63 - (j * 8);
        for (; SetIndex % 8 == 0; SetIndex++) {
        }
    }
}

void Board::draw() {
    SDL_RenderClear(m_renderer);
    for (int i = 0; i < 64; i++) {
        S[i].draw();
    }
    SDL_RenderPresent(m_renderer);
}

Board::~Board() {
    SDL_DestroyRenderer(m_renderer);
    SDL_FreeSurface(m_windowSurface);
}

void Board::clearHighlighted() {
    for (int i = 0; i < 64; i++) {
        S[i].isHighlighted = false;
        S[i].isCheck = false;
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

void Board::clickedPosition(int x, int y) {
    int j = x / 75;
    int i = y / 75;
    int index = i * 8 + j;

    Square* sq = &S[index];

    if (sq->isHighlighted) {
        movePiece(sq, index);
        this->turn++;

        Fen f(this->boardState);
        
        int pos;
        if (turn % 2) {
            for (int i = 0; i < 64; i++) {
                if (this->boardState[i] == State::BKING) {
                    pos = i;
                }
            }
        } else {
            for (int i = 0; i < 64; i++) {
                if (this->boardState[i] == State::WKING) {
                    pos = i;
                }
            }
        }

        clearHighlighted();
        if (f.isCheck(pos, turn % 2 ? 'b' : 'w')) {
            S[pos].isCheck = true;
        }

                return;
    }

    clearHighlighted();

    if (!sq->empty) {
        selectedSquare = sq;
        selectedSquareIndex = index;
        sq->getPiece()->getValidMoves(boardState, index);
        for (int i = 0; i < 64; i++) {
            if (boardState[i] & State::VALID) {
                boardState[i] &= ~State::VALID;
                S[i].isHighlighted = true;
            }
        }
    } else {
        selectedSquare = nullptr;
        selectedSquareIndex = -1;
    }
}
