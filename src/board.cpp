#include "board.h"

Board::Board(int width, int height, SDL_Window* window) {
    this->width = width;
    this->height = height;
    this->m_renderer = SDL_CreateRenderer(window, -1, 0);
    this->m_windowSurface = SDL_GetWindowSurface(window);

    int colorIndex = 0;
    for (int i = 0; i < 8; i++, colorIndex++) {
        for (int j = 0; j < 8; j++) {
            Color color = colorIndex++ % 2 == 0 ? Color::WHITE : Color::BLACK;
            S[i * 8 + j].set_values(j * 75, i * 75, 75, color, m_renderer);
        }
    }

    // for (int i = 0; i < 8; i++) {
    //     switch (i) {
    //         case 0:
    //             R[0].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&R[0]);
    //             break;
    //         case 1:
    //             KN[0].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&KN[0]);
    //             break;
    //         case 2:
    //             B[0].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&B[0]);
    //             break;
    //         case 3:
    //             Q[0].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&Q[0]);
    //             break;
    //         case 4:
    //             K[0].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&K[0]);
    //             break;
    //         case 5:
    //             B[1].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&B[1]);
    //             break;
    //         case 6:
    //             KN[1].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&KN[1]);
    //             break;
    //         case 7:
    //             R[1].setColor(Color::BLACK, m_renderer);
    //             S[i].setPiece(&R[1]);
    //             break;
    //     }
    // }
    // int j = 1;
    // for (int i = 56; i < 64; i++) {
    //     switch (i) {
    //         case 56:
    //             R[2].setColor(Color::WHITE, m_renderer);
    //             S[i].setPiece(&R[2]);
    //             R[3].setColor(Color::WHITE, m_renderer);
    //             S[64-j++].setPiece(&R[3]);
    //             break;
    //         case 57:
    //             KN[2].setColor(Color::WHITE, m_renderer);
    //             S[i].setPiece(&KN[2]);
    //             KN[3].setColor(Color::WHITE, m_renderer);
    //             S[64-j++].setPiece(&KN[3]);
    //             break;
    //         case 58:
    //             B[2].setColor(Color::WHITE, m_renderer);
    //             S[i].setPiece(&B[2]);
    //             B[3].setColor(Color::WHITE, m_renderer);
    //             S[64-j++].setPiece(&B[3]);
    //             break;
    //         case 59:
    //             Q[2].setColor(Color::WHITE, m_renderer);
    //             S[i].setPiece(&Q[2]);
    //             break;
    //         case 60:
    //             K[2].setColor(Color::WHITE, m_renderer);
    //             S[i].setPiece(&K[2]);
    //             break;
    //     }
    // }

    for (int i = 0; i < 4; i++) {
        R[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        KN[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        B[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        if (i < 2) {
            int j = 0;
            S[j + ((i % 2) * 8 * 7)].setPiece(&R[i]);
            j++;
            S[j + ((i % 2) * 8 * 7)].setPiece(&KN[i]);
            j++;
            S[j + ((i % 2) * 8 * 7)].setPiece(&B[i]);
        } else {
            int j = 5;
            S[j + ((i % 2) * 8 * 7)].setPiece(&B[i]);
            j++;
            S[j + ((i % 2) * 8 * 7)].setPiece(&KN[i]);
            j++;
            S[j + ((i % 2) * 8 * 7)].setPiece(&R[i]);
        }
    }

    for (int i = 0; i < 2; i++) {
        int j = 3;
        K[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        Q[i].setColor(i % 2 == 0 ? Color::BLACK : Color::WHITE, m_renderer);
        S[j + ((i % 2) * 8 * 7)].setPiece(&Q[i]);
        j++;
        S[j + ((i % 2) * 8 * 7)].setPiece(&K[i]);
    }

    for (int i = 0; i < 8; i++) {
        P[i].setColor(Color::BLACK, m_renderer);
        S[i + 8].setPiece(&P[i]);
    }

    for (int i = 8; i < 16; i++) {
        P[i].setColor(Color::WHITE, m_renderer);
        S[i + 8 * 5].setPiece(&P[i]);
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
