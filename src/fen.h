#pragma once
#include "state.h"

class Fen {
    private:
        char m_turn;
        short int m_castling;
        char m_code[73];
        int boardState[64];

    public:
        Fen(const char* code, char turn, short int castling);
        Fen(int boardState[]);
        void getBoardState(int boardState[64]);
        bool isCheck(int pos, char color);
        ~Fen();

        enum castling { BLC = 1, BSC = 2, WLC = 4, WSC = 8 };
};

