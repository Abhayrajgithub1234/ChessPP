#include "fen.h"

Fen::Fen(const char* code, char turn, short int castling) {
    this->m_turn = turn;
    this->m_castling = castling;
    int j = 0;

    for (int i = 0; code[i]; i++) {
        switch (code[i]) {
            case 'R':
                this->boardState[j++] = State::WROOK;
                break;
            case 'r':
                this->boardState[j++] = State::BROOK;
                break;
            case 'N':
                this->boardState[j++] = State::WKNIGHT;
                break;
            case 'n':
                this->boardState[j++] = State::BKNIGHT;
                break;
            case 'B':
                this->boardState[j++] = State::WBISHOP;
                break;
            case 'b':
                this->boardState[j++] = State::BBISHOP;
                break;
            case 'Q':
                this->boardState[j++] = State::WQUEEN;
                break;
            case 'q':
                this->boardState[j++] = State::BQUEEN;
                break;
            case 'K':
                this->boardState[j++] = State::WKING;
                break;
            case 'k':
                this->boardState[j++] = State::BKING;
                break;
            case 'P':
                this->boardState[j++] = State::WPAWN;
                break;
            case 'p':
                this->boardState[j++] = State::BPAWN;
                break;
            case '/':
                break;
            default: {
                int var = code[i] - '0';
                for (int k = 0; k < var; k++) {
                    this->boardState[j++] = State::NONE;
                }
            } break;
        }
    }
}

Fen::~Fen() {
}

Fen::Fen(int boardState[]) {
    int index = 0;
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0 && i != 0) {
            this->m_code[index++] = '/';
        }
        switch (boardState[i]) {
            case State::WROOK:
                this->m_code[index++] = 'R';
                break;
            case State::WKNIGHT:
                this->m_code[index++] = 'N';
                break;
            case State::WBISHOP:
                this->m_code[index++] = 'B';
                break;
            case State::WQUEEN:
                this->m_code[index++] = 'Q';
                break;
            case State::WKING:
                this->m_code[index++] = 'K';
                break;
            case State::WPAWN:
                this->m_code[index++] = 'P';
                break;
            case State::BROOK:
                this->m_code[index++] = 'r';
                break;
            case State::BKNIGHT:
                this->m_code[index++] = 'n';
                break;
            case State::BBISHOP:
                this->m_code[index++] = 'b';
                break;
            case State::BQUEEN:
                this->m_code[index++] = 'q';
                break;
            case State::BKING:
                this->m_code[index++] = 'k';
                break;
            case State::BPAWN:
                this->m_code[index++] = 'p';
                break;
            case State::NONE: {
                int count = 0;
                for (; boardState[i] == State::NONE && (i % 8 != 7); i++) {
                    count++;
                }
                // Count the last NONE square (at index i)
                if (boardState[i] == State::NONE) count++;
                else --i;
                this->m_code[index++] = count + '0';
                break;
            }
        }
    }

    this->m_code[index] = '\0';
    for (int i = 0; i < 64; i++) {
        this->boardState[i] = boardState[i];
    }
}

void Fen::getBoardState(int boardState[64]) {
    for (int i = 0; i < 64; i++) {
        boardState[i] = this->boardState[i];
    }
}

bool Fen::isCheck(int pos, char color) {
    int row = pos / 8;
    int col = pos % 8;

    auto isFriendly = [&](int p) {
        if (color == 'w')
            return State::WROOK <= boardState[p]
                && boardState[p] <= State::WPAWN;
        else
            return State::BROOK <= boardState[p]
                && boardState[p] <= State::BPAWN;
    };

    // ----- Horizontal Right -----
    for (int c = col + 1; c < 8; ++c) {
        int i = row * 8 + c;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BROOK
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WROOK
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ----- Horizontal Left -----
    for (int c = col - 1; c >= 0; --c) {
        int i = row * 8 + c;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BROOK
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WROOK
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ----- Vertical Up -----
    for (int r = row - 1; r >= 0; --r) {
        int i = r * 8 + col;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BROOK
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WROOK
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ----- Vertical Down -----
    for (int r = row + 1; r < 8; ++r) {
        int i = r * 8 + col;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BROOK
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WROOK
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ----- Diagonal Checks (Bishop / Queen) -----

    // ↖ up-left
    for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c) {
        int i = r * 8 + c;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BBISHOP
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WBISHOP
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ↗ up-right
    for (int r = row - 1, c = col + 1; r >= 0 && c < 8; --r, ++c) {
        int i = r * 8 + c;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BBISHOP
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WBISHOP
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ↙ down-left
    for (int r = row + 1, c = col - 1; r < 8 && c >= 0; ++r, --c) {
        int i = r * 8 + c;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BBISHOP
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WBISHOP
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ↘ down-right
    for (int r = row + 1, c = col + 1; r < 8 && c < 8; ++r, ++c) {
        int i = r * 8 + c;
        if (boardState[i] == State::NONE) continue;
        if (isFriendly(i)) break;
        if ((color == 'w'
             && (boardState[i] == State::BBISHOP
                 || boardState[i] == State::BQUEEN))
            || (color == 'b'
                && (boardState[i] == State::WBISHOP
                    || boardState[i] == State::WQUEEN)))
            return true;
        break;
    }

    // ----- Pawn Threats -----
    if (color == 'w') {
        if (col > 0 && pos - 9 >= 0 && boardState[pos - 9] == State::BPAWN)
            return true;
        if (col < 7 && pos - 7 >= 0 && boardState[pos - 7] == State::BPAWN)
            return true;
    } else {
        if (col > 0 && pos + 7 < 64 && boardState[pos + 7] == State::WPAWN)
            return true;
        if (col < 7 && pos + 9 < 64 && boardState[pos + 9] == State::WPAWN)
            return true;
    }

    //---this is for knights ---
    int possibleMoves[8] = {pos - 15, pos - 6, pos + 10, pos + 17,
                            pos + 15, pos + 6, pos - 10, pos - 17};

    switch (pos % 8) {
        case 0:
            possibleMoves[4] = possibleMoves[5] = possibleMoves[6] =
                possibleMoves[7] = -1;

            break;

        case 1:
            possibleMoves[5] = possibleMoves[6] = -1;
            break;

        case 6:
            possibleMoves[1] = possibleMoves[2] = -1;
            break;

        case 7:

            possibleMoves[0] = possibleMoves[1] = possibleMoves[2] =
                possibleMoves[3] = -1;

            break;
        default:
            break;
    }
    if (color == 'w') {
        for (int i = 0; i < 8; i++) {
            int idx = possibleMoves[i];
            if (idx >= 0 && idx <= 63) {
                if (boardState[idx] == State::BKNIGHT) return true;
            }
        }
    } else {
        for (int i = 0; i < 8; i++) {
            int idx = possibleMoves[i];
            if (idx >= 0 && idx <= 63) {
                if (boardState[idx] == State::WKNIGHT) return true;
            }
        }
    }

    return false;
}
