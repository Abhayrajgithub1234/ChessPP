#include "board.h"

Board::Board(int width, int height, SDL_Window* window) {
    this->width = width;
    this->height = height;
    this->m_renderer = SDL_CreateRenderer(window, -1, 0);
    this->m_windowSurface = SDL_GetWindowSurface(window);
    this->selectedSquare = nullptr;
    this->selectedSquareIndex = -1;

    int colorIndex = 0;
    for (int i = 0; i < 8; i++, colorIndex++) {
        for (int j = 0; j < 8; j++) {
            Color color = colorIndex++ % 2 == 0 ? Color::WHITE : Color::BLACK;
            S[i * 8 + j].set_values(j * 75, i * 75, 75, color, m_renderer);
            boardState[i * 8 + j] = State::NONE;
        }
    }

    for (int i = 0; i < 4; i++) {
        R[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        KN[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        B[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        if (i < 2) {
            int index = 0 + ((i % 2) * 8 * 7);
            S[index].setPiece(&R[i]);
            boardState[index] = i % 2 == 0 ? State::BROOK : State::WROOK;
            index++;
            S[index].setPiece(&KN[i]);
            boardState[index] = i % 2 == 0 ? State::BKNIGHT : State::WKNIGHT;
            index++;
            S[index].setPiece(&B[i]);
            boardState[index] = i % 2 == 0 ? State::BBISHOP : State::WBISHOP;
        } else {
            int index = 5 + ((i % 2) * 8 * 7);
            S[index].setPiece(&B[i]);
            boardState[index] = i % 2 == 0 ? State::BBISHOP : State::WBISHOP;
            index++;
            S[index].setPiece(&KN[i]);
            boardState[index] = i % 2 == 0 ? State::BKNIGHT : State::WKNIGHT;
            index++;
            S[index].setPiece(&R[i]);
            boardState[index] = i % 2 == 0 ? State::BROOK : State::WROOK;
        }
    }

    for (int i = 0; i < 2; i++) {
        int index = 3 + ((i % 2) * 8 * 7);
        K[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        Q[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        S[index].setPiece(&Q[i]);
        boardState[index] = i % 2 == 0 ? State::BQUEEN : State::WQUEEN;
        index++;
        S[index].setPiece(&K[i]);
        boardState[index] = i % 2 == 0 ? State::BKING : State::WKING;
    }

    for (int i = 0; i < 8; i++) {
        int index = i + 8;
        P[i].setColor(Color::BLACK, m_renderer);
        S[index].setPiece(&P[i]);
        boardState[index] = State::BPAWN;
    }

    for (int i = 8; i < 16; i++) {
        int index = i + 8 * 5;
        P[i].setColor(Color::WHITE, m_renderer);
        S[index].setPiece(&P[i]);
        boardState[index] = State::WPAWN;
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
        clearHighlighted();
        return;
    }

    clearHighlighted();

    if (!sq->empty) {
        State copyBoardState[64];

        for (int i = 0; i < 64; i++) {
            copyBoardState[i] = boardState[i];
        }

        selectedSquare = sq;
        selectedSquareIndex = index;
        sq->getPiece()->getValidMoves(copyBoardState, index);
        for (int i = 0; i < 64; i++) {
            if (copyBoardState[i] == State::VALID) {
                S[i].isHighlighted = true;
            }
        }
    } else {
        selectedSquare = nullptr;
        selectedSquareIndex = -1;
    }
}
