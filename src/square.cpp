#include "square.h"

#include <string.h>

Square::Square() {
    this->x = 0;
    this->y = 0;
    this->size = 0;
    this->color = Color::NONE;
    this->m_renderer = nullptr;
    this->empty = true;
}

Square::Square(int x, int y, int size, Color color, SDL_Renderer* m_renderer) {
    this->x = x;
    this->y = y;
    this->size = size;
    this->color = color;
    this->m_renderer = m_renderer;
    this->empty = true;
    this->piece = nullptr;
}

Square ::~Square() {
}

void Square::draw() {
    // SDL_RenderClear(m_renderer);

    SDL_Rect rect;

    rect.w = this->size;
    rect.h = this->size;
    rect.x = this->x;
    rect.y = this->y;

    if (color == Color::WHITE) {
        SDL_SetRenderDrawColor(m_renderer, 180, 154, 154, 255);

    } else {
        SDL_SetRenderDrawColor(m_renderer, 97, 75, 75, 255);
    }

    SDL_RenderFillRect(m_renderer, &rect);


    if (!empty) piece->draw(m_renderer);
}

void Square::set_values(int x, int y, int size, Color color,
                        SDL_Renderer* renderer) {
    this->x = x;
    this->y = y;
    this->size = size;
    this->color = color;
    this->m_renderer = renderer;
}

void Square::setPiece(Piece* piece) {
    this->piece = piece;
    piece->setPosition(this->x, this->y);
    this->empty = false;
}

void Square::clearPiece() {
    this->empty = true;
}
