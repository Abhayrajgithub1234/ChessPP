#include "square.h"
#include "constants.h"

Square::Square() {
    this->x = 0;
    this->y = 0;
    this->size = 0;
    this->color = Color::NONE;
    this->m_renderer = nullptr;
    this->piece = nullptr;
    this->empty = true;
    this->isHighlighted = false;
    this->isCheck = false;
    this->isLastMove = false;
    this->isSelected = false;
}

Square::Square(int x, int y, int size, Color color, SDL_Renderer* m_renderer) {
    this->x = x;
    this->y = y;
    this->size = size;
    this->color = color;
    this->m_renderer = m_renderer;
    this->empty = true;
    this->piece = nullptr;
    this->isHighlighted = false;
    this->isCheck = false;
    this->isLastMove = false;
    this->isSelected = false;
}

Square ::~Square() {
}

void Square::draw() {
    if (!m_renderer) return;
    
    SDL_Rect rect;

    rect.w = this->size;
    rect.h = this->size;
    rect.x = this->x;
    rect.y = this->y;

    // Draw base square color
    if (color == Color::WHITE) {
        SDL_SetRenderDrawColor(m_renderer, 
            ChessConstants::Colors::LIGHT_R,
            ChessConstants::Colors::LIGHT_G,
            ChessConstants::Colors::LIGHT_B, 255);
    } else {
        SDL_SetRenderDrawColor(m_renderer, 
            ChessConstants::Colors::DARK_R,
            ChessConstants::Colors::DARK_G,
            ChessConstants::Colors::DARK_B, 255);
    }
    SDL_RenderFillRect(m_renderer, &rect);

    // Draw last move highlight (chess.com yellow-green tint)
    if (isLastMove) {
        // Chess.com style: blend the last move color with the square color
        int blendR, blendG, blendB;
        if (color == Color::WHITE) {
            blendR = 247; blendG = 247; blendB = 105;  // Bright yellow for light
        } else {
            blendR = 186; blendG = 202; blendB = 68;   // Green-yellow for dark
        }
        SDL_SetRenderDrawColor(m_renderer, blendR, blendG, blendB, 255);
        SDL_RenderFillRect(m_renderer, &rect);
    }

    // Draw selected square highlight (similar to last move)
    if (isSelected && !isLastMove) {
        int blendR, blendG, blendB;
        if (color == Color::WHITE) {
            blendR = 247; blendG = 247; blendB = 105;
        } else {
            blendR = 186; blendG = 202; blendB = 68;
        }
        SDL_SetRenderDrawColor(m_renderer, blendR, blendG, blendB, 255);
        SDL_RenderFillRect(m_renderer, &rect);
    }

    // Draw check highlight (radial red gradient effect)
    if (isCheck) {
        // Draw multiple rectangles with decreasing alpha for glow effect
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 
            ChessConstants::Colors::CHECK_R,
            ChessConstants::Colors::CHECK_G,
            ChessConstants::Colors::CHECK_B, 180);
        SDL_RenderFillRect(m_renderer, &rect);
    }

    // Draw move highlight (chess.com style dots and rings)
    if (isHighlighted) {
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        int centerX = rect.x + (rect.w / 2);
        int centerY = rect.y + (rect.h / 2);
        
        if (empty) {
            // Small semi-transparent dot for empty squares
            int radius = size / 6;
            // Multiple layers for smoother appearance
            for (int r = radius; r > 0; r--) {
                int alpha = (int)(40 * (1.0 - (float)r / radius) + 25);
                filledCircleRGBA(m_renderer, centerX, centerY, r, 
                                 0, 0, 0, alpha);
            }
            // Anti-aliased edge
            aacircleRGBA(m_renderer, centerX, centerY, radius, 0, 0, 0, 60);
        } else {
            // Triangle corners for capturable pieces (chess.com style)
            int cornerSize = size / 4;
            
            // Top-left corner
            filledTrigonRGBA(m_renderer, 
                rect.x, rect.y,
                rect.x + cornerSize, rect.y,
                rect.x, rect.y + cornerSize,
                0, 0, 0, 80);
            
            // Top-right corner
            filledTrigonRGBA(m_renderer,
                rect.x + rect.w, rect.y,
                rect.x + rect.w - cornerSize, rect.y,
                rect.x + rect.w, rect.y + cornerSize,
                0, 0, 0, 80);
            
            // Bottom-left corner
            filledTrigonRGBA(m_renderer,
                rect.x, rect.y + rect.h,
                rect.x + cornerSize, rect.y + rect.h,
                rect.x, rect.y + rect.h - cornerSize,
                0, 0, 0, 80);
            
            // Bottom-right corner
            filledTrigonRGBA(m_renderer,
                rect.x + rect.w, rect.y + rect.h,
                rect.x + rect.w - cornerSize, rect.y + rect.h,
                rect.x + rect.w, rect.y + rect.h - cornerSize,
                0, 0, 0, 80);
        }
    }

    if (!empty && piece) piece->draw(m_renderer);
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
    if (!piece) return;
    this->piece = piece;
    piece->setPosition(this->x, this->y, this->size);
    this->empty = false;
}

void Square::clearPiece() {
    this->empty = true;
    this->piece = nullptr;
}

Piece* Square::getPiece() {
    return this->piece;
}
