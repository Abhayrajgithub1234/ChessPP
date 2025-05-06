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
}

void Board::draw() {
    SDL_RenderClear(m_renderer);
    for (int i = 0; i < 64; i++) {
        S[i].draw();
    }
    SDL_RenderPresent(m_renderer);
}

Board::~Board() {
    for (; promotedIndex - 1 > -1; promotedIndex--) {
        delete (promoted[promotedIndex - 1]);
    }
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

void Board::clickedPosition(int x, int y) {
    int j = x / 75;
    int i = y / 75;
    int index = i * 8 + j;

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
            int piece;
        ask:
            printf(
                "Promote the pawn to \n1.Queen\n2.Bishop\n3.Knight\n4.Rook\n");
            scanf("%d", &piece);
            switch (piece) {
                case 1:
                    boardState[index] =
                        turn % 2 ? State::BQUEEN : State::WQUEEN;
                    this->promoted[promotedIndex++] = new Queen();
                    promoted[promotedIndex - 1]->setColor(
                        turn % 2 ? Color::BLACK : Color::WHITE, m_renderer);
                    S[index].setPiece(promoted[promotedIndex - 1]);
                    break;
                case 2:
                    boardState[index] =
                        turn % 2 ? State::BBISHOP : State::WBISHOP;
                    this->promoted[promotedIndex++] = new Bishop();
                    promoted[promotedIndex - 1]->setColor(
                        turn % 2 ? Color::BLACK : Color::WHITE, m_renderer);
                    S[index].setPiece(promoted[promotedIndex - 1]);
                    break;
                case 3:
                    boardState[index] =
                        turn % 2 ? State::BKNIGHT : State::WKNIGHT;
                    this->promoted[promotedIndex++] = new Knight();
                    promoted[promotedIndex - 1]->setColor(
                        turn % 2 ? Color::BLACK : Color::WHITE, m_renderer);
                    S[index].setPiece(promoted[promotedIndex - 1]);
                    break;
                case 4:
                    boardState[index] = turn % 2 ? State::BROOK : State::WROOK;
                    this->promoted[promotedIndex++] = new Rook();
                    promoted[promotedIndex - 1]->setColor(
                        turn % 2 ? Color::BLACK : Color::WHITE, m_renderer);
                    S[index].setPiece(promoted[promotedIndex - 1]);
                    break;
                default:
                    goto ask;
            }
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
                } else {
                    printf("BLACK WINS!!\n");
                }
            }
        } else if (!haveValidMoves) {
            printf("StaleMate\n");
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
