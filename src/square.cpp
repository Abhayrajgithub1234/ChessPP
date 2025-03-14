#include "square.h"

#include <string.h>

Square::Square(int x, int y, int size, Color color, SDL_Renderer* renderer) {
    this->x = x;
    this->y = y;
    this->size = size;
    this->color = color;
    this->m_renderer = renderer;
}

Square ::~Square() {
}

void Square::draw() {
    SDL_RenderClear(m_renderer);

    SDL_Rect rect;

    rect.w = this->size;
    rect.h = this->size;
    rect.x = this->x;
    rect.y = this->y;

    if (color == Color::WHITE) {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

    } else {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    }

    SDL_RenderFillRect(m_renderer, &rect);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderPresent(m_renderer);
}
