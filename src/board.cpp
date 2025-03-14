#include "board.h"

#include "square.h"

Board::Board(int width, int height, SDL_Window* window) {
    this->width = width;
    this->height = height;
    this->m_renderer = SDL_CreateRenderer(window, -1, 0);
}

void Board::draw() {
    Square S[64];

    int colorIndex = 0;

    for (int i = 0; i < 8; i++, colorIndex++) {
        for (int j = 0; j < 8; j++) {
            Color color = colorIndex++ % 2 == 0 ? Color::WHITE : Color::BLACK;
            S[i * 8 + j].set_values(j * 75, i * 75, 75, color, m_renderer);
        }
    }

    for (int i = 0; i < 64; i++) {
        S[i].draw();
    }
}

Board::~Board() {
    SDL_DestroyRenderer(m_renderer);
}
