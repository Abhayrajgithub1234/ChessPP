#include "board.h"

#include "square.h"

Board::Board(int width, int height, SDL_Window* window) {
    this->width = width;
    this->height = height;
    this->m_renderer = SDL_CreateRenderer(window, -1, 0);
}

void Board::draw() {
    Square s1(0, 0, 64, Color::WHITE, this->m_renderer);
    s1.draw();
    Square s2(64, 0, 64, Color::BLACK, this->m_renderer);
    s2.draw();
    Square s3(128, 0, 64, Color::WHITE, this->m_renderer);
    s3.draw();
    Square s4((128 * 2), 0, 64, Color::BLACK, this->m_renderer);
    s4.draw();
    Square s5((128 * 4), 0, 64, Color::WHITE, this->m_renderer);
    s5.draw();
}

Board::~Board() {
    SDL_DestroyRenderer(m_renderer);
}
