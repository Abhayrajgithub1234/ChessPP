#include "board.h"
#include "constants.h"

#include <algorithm>
#include <SDL2/SDL2_gfxPrimitives.h>

Board::Board(int width, int height, SDL_Window* window)
    : fen(Fen((const char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w',
              Fen::ALL)) {
    this->width = width;
    this->height = height;
    this->windowWidth = width;
    this->windowHeight = height;
    this->m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    this->m_windowSurface = SDL_GetWindowSurface(window);
    this->selectedSquare = nullptr;
    this->selectedSquareIndex = -1;
    this->fen.getBoardState(this->boardState);

    // Set the blend mode
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    // Calculate board layout (centered, with space for UI)
    updateLayout();

    // Initialize squares with proper colors
    int colorIndex = 0;
    for (int i = 0; i < ChessConstants::BOARD_SIZE; i++, colorIndex++) {
        for (int j = 0; j < ChessConstants::BOARD_SIZE; j++) {
            Color color = colorIndex++ % 2 == 0 ? Color::WHITE : Color::BLACK;
            S[i * ChessConstants::BOARD_SIZE + j].set_values(
                boardOffsetX + j * squareSize, 
                boardOffsetY + i * squareSize, 
                squareSize, color, m_renderer);
        }
    }

    initializePieces();

    // Load promotion piece textures
    const char* imgs[] = {"assets/Wrook.bmp",   "assets/Wbishop.bmp",
                          "assets/Wknight.bmp", "assets/Wqueen.bmp",
                          "assets/Brook.bmp",   "assets/Bbishop.bmp",
                          "assets/Bknight.bmp", "assets/Bqueen.bmp"};

    for (int i = 0; i < 8; ++i) {
        SDL_Surface* surface = SDL_LoadBMP(imgs[i]);
        if (!surface) {
            printf("Failed to load image: %s\n", imgs[i]);
            promotionPieces[i] = nullptr;
        } else {
            this->promotionPieces[i] = SDL_CreateTextureFromSurface(m_renderer, surface);
            SDL_FreeSurface(surface);
        }
    }

    // Setup popup rects
    int w = 75, h = 75;
    if (promotionPieces[0]) {
        SDL_QueryTexture(promotionPieces[0], NULL, NULL, &w, &h);
    }
    this->popupRect.w = (w * 4) + 10;
    this->popupRect.h = (h) + 10;
    this->popupRect.x = (windowWidth / 2) - (this->popupRect.w / 2);
    this->popupRect.y = (windowHeight / 2) - (this->popupRect.h / 2);
    for (int i = 0; i < 4; ++i)
        this->popupImgRects[i] = {popupRect.x + (i * w), popupRect.y, w, h};
}

void Board::updateLayout() {
    // Calculate available space for board (leave room for UI at bottom)
    int turnIndicatorHeight = 35;  // Space for turn text
    int uiHeight = turnIndicatorHeight + ChessConstants::BUTTON_HEIGHT + ChessConstants::BUTTON_MARGIN * 2 + 40;
    int availableHeight = windowHeight - uiHeight;
    int availableWidth = windowWidth - ChessConstants::BOARD_PADDING * 2;
    
    // Use the smaller dimension to keep board square
    int boardSize = std::min(availableWidth, availableHeight);
    this->squareSize = boardSize / ChessConstants::BOARD_SIZE;
    int actualBoardSize = squareSize * ChessConstants::BOARD_SIZE;
    
    // Center the board horizontally, position it at top with padding
    this->boardOffsetX = (windowWidth - actualBoardSize) / 2;
    this->boardOffsetY = ChessConstants::BOARD_PADDING;
    
    // Position buttons below the board (after turn indicator)
    int buttonY = boardOffsetY + actualBoardSize + turnIndicatorHeight + ChessConstants::BUTTON_MARGIN;
    int totalButtonWidth = ChessConstants::BUTTON_WIDTH * 2 + ChessConstants::BUTTON_MARGIN;
    int buttonStartX = (windowWidth - totalButtonWidth) / 2;
    
    resetButtonRect = {
        buttonStartX,
        buttonY,
        ChessConstants::BUTTON_WIDTH,
        ChessConstants::BUTTON_HEIGHT
    };
    
    newGameButtonRect = {
        buttonStartX + ChessConstants::BUTTON_WIDTH + ChessConstants::BUTTON_MARGIN,
        buttonY,
        ChessConstants::BUTTON_WIDTH,
        ChessConstants::BUTTON_HEIGHT
    };
}

void Board::initializePieces() {
    // Reset piece counters
    bpi = wpi = bri = wri = bbi = wbi = bni = wni = bqi = wqi = 0;
    
    for (int i = 0; i < 64; ++i) {
        switch (boardState[i]) {
            case State::WROOK: {
                WR[wri].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WR[wri++]);
            } break;
            case State::WKNIGHT: {
                WN[wni].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WN[wni++]);
            } break;
            case State::WBISHOP: {
                WB[wbi].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WB[wbi++]);
            } break;
            case State::WQUEEN: {
                WQ[wqi].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WQ[wqi++]);
            } break;
            case State::WKING: {
                WK.setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WK);
            } break;
            case State::WPAWN: {
                WP[wpi].setColor(Color::WHITE, m_renderer);
                S[i].setPiece(&WP[wpi++]);
            } break;
            case State::BROOK: {
                BR[bri].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BR[bri++]);
            } break;
            case State::BKNIGHT: {
                BN[bni].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BN[bni++]);
            } break;
            case State::BBISHOP: {
                BB[bbi].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BB[bbi++]);
            } break;
            case State::BQUEEN: {
                BQ[bqi].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BQ[bqi++]);
            } break;
            case State::BKING: {
                BK.setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BK);
            } break;
            case State::BPAWN: {
                BP[bpi].setColor(Color::BLACK, m_renderer);
                S[i].setPiece(&BP[bpi++]);
            } break;
            default:
                break;
        }
    }
}

void Board::draw() {
    // Draw background
    drawBackground();
    
    int actualBoardSize = squareSize * ChessConstants::BOARD_SIZE;
    
    // Draw board shadow (multiple layers for soft shadow effect)
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    for (int i = 6; i >= 1; i--) {
        SDL_Rect shadowRect = {
            boardOffsetX - 2 + i * 2, boardOffsetY - 2 + i * 2,
            actualBoardSize + 4, actualBoardSize + 4
        };
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 20 + (6 - i) * 8);
        SDL_RenderFillRect(m_renderer, &shadowRect);
    }
    
    // Draw board border
    SDL_Rect borderRect = {
        boardOffsetX - 3, boardOffsetY - 3,
        actualBoardSize + 6, actualBoardSize + 6
    };
    SDL_SetRenderDrawColor(m_renderer, 
        ChessConstants::Colors::BORDER_R,
        ChessConstants::Colors::BORDER_G,
        ChessConstants::Colors::BORDER_B, 255);
    SDL_RenderFillRect(m_renderer, &borderRect);

    // Draw squares
    for (int i = 0; i < 64; i++) S[i].draw();
    
    // Draw coordinate labels (chess.com style)
    drawBoardCoordinates();
    
    // Draw UI elements
    drawUI();

    if (this->showPopup) showOptions();

    if (end) drawGameEndScreen();

    SDL_RenderPresent(m_renderer);
}

void Board::drawBackground() {
    SDL_SetRenderDrawColor(m_renderer, 
        ChessConstants::Colors::BG_R,
        ChessConstants::Colors::BG_G,
        ChessConstants::Colors::BG_B, 255);
    SDL_RenderClear(m_renderer);
}

void Board::drawUI() {
    // Draw turn indicator
    drawTurnIndicator();
    
    // Draw buttons
    drawButton(resetButtonRect, "Reset", resetHovered);
    drawButton(newGameButtonRect, "New Game", newGameHovered);
}

void Board::drawButton(SDL_Rect& rect, const char* text, bool hovered) {
    // Draw rounded button background (chess.com style)
    int radius = 5;
    Uint8 r, g, b;
    
    if (hovered) {
        r = ChessConstants::Colors::BTN_HOVER_R;
        g = ChessConstants::Colors::BTN_HOVER_G;
        b = ChessConstants::Colors::BTN_HOVER_B;
    } else {
        r = ChessConstants::Colors::BTN_R;
        g = ChessConstants::Colors::BTN_G;
        b = ChessConstants::Colors::BTN_B;
    }
    
    // Draw rounded rectangle using SDL2_gfx
    roundedBoxRGBA(m_renderer, 
                   rect.x, rect.y, 
                   rect.x + rect.w, rect.y + rect.h, 
                   radius, r, g, b, 255);
    
    // Draw button text
    TTF_Font* font = TTF_OpenFont(ChessConstants::FONT_PATH, 
                                  ChessConstants::BUTTON_FONT_SIZE);
    if (!font) return;
    
    SDL_Color textColor = {
        ChessConstants::Colors::BTN_TEXT_R,
        ChessConstants::Colors::BTN_TEXT_G,
        ChessConstants::Colors::BTN_TEXT_B, 255
    };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, textColor);
    if (!textSurface) {
        TTF_CloseFont(font);
        return;
    }
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    if (textTexture) {
        SDL_Rect textRect = {
            rect.x + (rect.w - textSurface->w) / 2,
            rect.y + (rect.h - textSurface->h) / 2,
            textSurface->w,
            textSurface->h
        };
        SDL_RenderCopy(m_renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }
    
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

void Board::drawTurnIndicator() {
    TTF_Font* font = TTF_OpenFont(ChessConstants::FONT_PATH, 
                                  ChessConstants::TURN_FONT_SIZE);
    if (!font) return;
    
    const char* turnText = (turn % 2 == 0) ? "White's Turn" : "Black's Turn";
    SDL_Color textColor = {220, 220, 220, 255};
    
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, turnText, textColor);
    if (!textSurface) {
        TTF_CloseFont(font);
        return;
    }
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    if (textTexture) {
        int actualBoardSize = squareSize * ChessConstants::BOARD_SIZE;
        SDL_Rect textRect = {
            (windowWidth - textSurface->w) / 2,
            boardOffsetY + actualBoardSize + 12,
            textSurface->w,
            textSurface->h
        };
        SDL_RenderCopy(m_renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }
    
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

void Board::drawBoardCoordinates() {
    int fontSize = std::max(10, squareSize / 5);
    TTF_Font* font = TTF_OpenFont(ChessConstants::FONT_PATH, fontSize);
    if (!font) return;
    
    // Draw file letters (a-h) on bottom row
    for (int col = 0; col < 8; col++) {
        int row = 7;  // Bottom row
        int sqIndex = row * 8 + col;
        Color sqColor = (row + col) % 2 == 0 ? Color::WHITE : Color::BLACK;
        
        SDL_Color textColor;
        if (sqColor == Color::WHITE) {
            textColor = {(Uint8)ChessConstants::Colors::DARK_R, 
                         (Uint8)ChessConstants::Colors::DARK_G, 
                         (Uint8)ChessConstants::Colors::DARK_B, 255};
        } else {
            textColor = {(Uint8)ChessConstants::Colors::LIGHT_R, 
                         (Uint8)ChessConstants::Colors::LIGHT_G, 
                         (Uint8)ChessConstants::Colors::LIGHT_B, 255};
        }
        
        char fileChar[2] = {(char)('a' + col), '\0'};
        SDL_Surface* surface = TTF_RenderText_Blended(font, fileChar, textColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
            if (texture) {
                int x = boardOffsetX + col * squareSize;
                int y = boardOffsetY + row * squareSize;
                SDL_Rect textRect = {
                    x + squareSize - surface->w - 3,
                    y + squareSize - surface->h - 2,
                    surface->w,
                    surface->h
                };
                SDL_RenderCopy(m_renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
    
    // Draw rank numbers (8-1) on leftmost column
    for (int row = 0; row < 8; row++) {
        int col = 0;  // Leftmost column
        Color sqColor = (row + col) % 2 == 0 ? Color::WHITE : Color::BLACK;
        
        SDL_Color textColor;
        if (sqColor == Color::WHITE) {
            textColor = {(Uint8)ChessConstants::Colors::DARK_R, 
                         (Uint8)ChessConstants::Colors::DARK_G, 
                         (Uint8)ChessConstants::Colors::DARK_B, 255};
        } else {
            textColor = {(Uint8)ChessConstants::Colors::LIGHT_R, 
                         (Uint8)ChessConstants::Colors::LIGHT_G, 
                         (Uint8)ChessConstants::Colors::LIGHT_B, 255};
        }
        
        char rankChar[2] = {(char)('8' - row), '\0'};
        SDL_Surface* surface = TTF_RenderText_Blended(font, rankChar, textColor);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
            if (texture) {
                int x = boardOffsetX + col * squareSize;
                int y = boardOffsetY + row * squareSize;
                SDL_Rect textRect = {
                    x + 3,
                    y + 2,
                    surface->w,
                    surface->h
                };
                SDL_RenderCopy(m_renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
        }
    }
    
    TTF_CloseFont(font);
}

void Board::reset() {
    // Reset game state
    this->fen = Fen((const char*)"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", 'w', Fen::ALL);
    this->fen.getBoardState(this->boardState);
    
    // Clear all squares
    for (int i = 0; i < 64; i++) {
        S[i].clearPiece();
        S[i].isHighlighted = false;
        S[i].isCheck = false;
        S[i].isLastMove = false;
        S[i].isSelected = false;
    }
    
    // Reset game variables
    this->turn = 0;
    this->end = false;
    this->promotion = false;
    this->showPopup = false;
    this->selectedSquare = nullptr;
    this->selectedSquareIndex = -1;
    this->castling = Fen::ALL;
    this->lastMoveFrom = -1;
    this->lastMoveTo = -1;
    
    // Reinitialize pieces
    initializePieces();
}

bool Board::checkButtonClick(int x, int y) {
    SDL_Point p = {x, y};
    
    if (SDL_PointInRect(&p, &resetButtonRect)) {
        reset();
        return true;
    }
    
    if (SDL_PointInRect(&p, &newGameButtonRect)) {
        reset();
        return true;
    }
    
    return false;
}

void Board::drawGameEndScreen() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 80);

    SDL_Rect overlay;
    overlay.w = windowWidth;
    overlay.h = windowHeight / 3;
    overlay.x = (windowWidth / 2) - (overlay.w / 2);
    overlay.y = (windowHeight / 2) - (overlay.h / 2);

    SDL_RenderFillRect(m_renderer, &overlay);

    const char* resultText = this->won == Color::BLACK ? "Black WON!"
                           : this->won == Color::WHITE ? "White WON!"
                                                       : "STALEMATE!";

    // Render text (use SDL_ttf)
    TTF_Font* font = TTF_OpenFont(ChessConstants::FONT_PATH, 
                                  ChessConstants::END_GAME_FONT_SIZE);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }
    
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, resultText, white);
    if (!textSurface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }
    
    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(m_renderer, textSurface);
    if (!textTexture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect = {(windowWidth - textSurface->w) / 2,
                         (windowHeight - textSurface->h) / 2, textSurface->w,
                         textSurface->h};
    SDL_RenderCopy(m_renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

Board::~Board() {
    for (int i = 0; i < 8; ++i) SDL_DestroyTexture(promotionPieces[i]);

    SDL_DestroyRenderer(m_renderer);
    SDL_FreeSurface(m_windowSurface);
}

void Board::clearHighlighted() {
    for (int i = 0; i < 64; i++) {
        S[i].isHighlighted = false;
        S[i].isSelected = false;
        boardState[i] &= ~State::CASTLE;
    }
}

void Board::movePiece(Square* sq, int index) {
    // Clear previous last move highlights
    if (lastMoveFrom >= 0 && lastMoveFrom < 64) S[lastMoveFrom].isLastMove = false;
    if (lastMoveTo >= 0 && lastMoveTo < 64) S[lastMoveTo].isLastMove = false;
    
    // Set new last move
    lastMoveFrom = selectedSquareIndex;
    lastMoveTo = index;
    S[lastMoveFrom].isLastMove = true;
    S[lastMoveTo].isLastMove = true;
    
    int mask = ~(State::VALID | State::PROMOTION | State::CASTLE);
    switch (boardState[selectedSquareIndex] & mask) {
        case State::BROOK:
            if (index == 0) this->castling &= ~Fen::BLC;
            else if (index == 7) this->castling &= ~Fen::BSC;
            break;
        case State::BKING:
            this->castling &= ~Fen::BCASTLE;
            break;
        case State::WROOK:
            if (index == 56) this->castling &= ~Fen::WLC;
            else if (index == 63) this->castling &= ~Fen::WSC;
            break;
        case State::WKING:
            this->castling &= ~Fen::WCASTLE;
            break;
        default:
            break;
    }

    if (boardState[index] & State::LCASTLE) {
        boardState[selectedSquareIndex - 1] =
            boardState[selectedSquareIndex - 4];
        boardState[selectedSquareIndex - 4] = State::NONE;
        this->S[selectedSquareIndex - 1].setPiece(
            this->S[selectedSquareIndex - 4].getPiece());
        this->S[selectedSquareIndex - 4].clearPiece();
    }

    if (boardState[index] & State::SCASTLE) {
        boardState[selectedSquareIndex + 1] =
            boardState[selectedSquareIndex + 3];
        boardState[selectedSquareIndex + 3] = State::NONE;
        this->S[selectedSquareIndex + 1].setPiece(
            this->S[selectedSquareIndex + 3].getPiece());
        this->S[selectedSquareIndex + 3].clearPiece();
    }

    sq->setPiece(selectedSquare->getPiece());
    boardState[index] = boardState[selectedSquareIndex];
    selectedSquare->clearPiece();
    boardState[selectedSquareIndex] = State::NONE;
    selectedSquare = nullptr;
    selectedSquareIndex = -1;
}

static bool hasValidMoves(int boardState[], char turn, Square S[]) {
    int BS[ChessConstants::TOTAL_SQUARES];
    int mask = ~(State::VALID | State::PROMOTION | State::CASTLE);

    for (int i = 0; i < ChessConstants::TOTAL_SQUARES; i++) {
        BS[i] = boardState[i];
    }

    for (int i = 0; i < ChessConstants::TOTAL_SQUARES; i++) {
        int pieceType = BS[i] & mask;
        if (turn == 'w') {
            if (pieceType <= State::WPAWN && pieceType >= State::WROOK) {
                Piece* p = S[i].getPiece();
                if (!p) continue;
                p->getValidMoves(BS, i);
                for (int j = 0; j < ChessConstants::TOTAL_SQUARES; j++) {
                    if (BS[j] & State::VALID) {
                        return true;
                    }
                }
            }
        } else {
            if (pieceType <= State::BPAWN && pieceType >= State::BROOK) {
                Piece* p = S[i].getPiece();
                if (!p) continue;
                p->getValidMoves(BS, i);
                for (int j = 0; j < ChessConstants::TOTAL_SQUARES; j++) {
                    if (BS[j] & State::VALID) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Board::showOptions() {
    SDL_SetRenderDrawColor(m_renderer, 72, 245, 66, 255);

    SDL_RenderFillRect(m_renderer, &this->popupRect);

    int base = (this->popupColor == Color::BLACK) ? 4 : 0;

    for (int i = 0; i < 4; ++i) {
        SDL_RenderCopy(m_renderer, promotionPieces[base + i], NULL,
                       &this->popupImgRects[i]);
    }
}

bool Board::popWindow(int x, int y) {
    if (this->showPopup) {
        SDL_Point p = {x, y};
        if (SDL_PointInRect(&p, &popupRect)) {
            for (int i = 0; i < 4; ++i) {
                if (SDL_PointInRect(&p, &popupImgRects[i])) {
                    // printf("%d\n", i);
                    int Wpos = -1;
                    int Bpos = -1;
                    int mask = ~(State::VALID | State::PROMOTION | State::CASTLE);
                    for (int j = 0; j < 64; j++) {
                        if ((this->boardState[j] & mask) == State::WKING) {
                            Wpos = j;
                        }
                        if ((this->boardState[j] & mask) == State::BKING) {
                            Bpos = j;
                        }
                    }
                    if (Wpos < 0 || Bpos < 0) return false;

                    clearHighlighted();

                    // Assume we have no check for both the kings
                    S[Bpos].isCheck = false;
                    S[Wpos].isCheck = false;

                    this->showPopup = false;
                    switch (i) {
                        case 0:
                            if (turn % 2) {
                                if (bri < ChessConstants::MAX_ROOKS) {
                                    boardState[promote_index] = State::BROOK;
                                    BR[bri].setColor(Color::BLACK, m_renderer);
                                    S[promote_index].setPiece(&BR[bri++]);
                                }
                            } else {
                                if (wri < ChessConstants::MAX_ROOKS) {
                                    boardState[promote_index] = State::WROOK;
                                    WR[wri].setColor(Color::WHITE, m_renderer);
                                    S[promote_index].setPiece(&WR[wri++]);
                                }
                            }
                            break;
                        case 1:
                            if (turn % 2) {
                                if (bbi < ChessConstants::MAX_BISHOPS) {
                                    boardState[promote_index] = State::BBISHOP;
                                    BB[bbi].setColor(Color::BLACK, m_renderer);
                                    S[promote_index].setPiece(&BB[bbi++]);
                                }
                            } else {
                                if (wbi < ChessConstants::MAX_BISHOPS) {
                                    boardState[promote_index] = State::WBISHOP;
                                    WB[wbi].setColor(Color::WHITE, m_renderer);
                                    S[promote_index].setPiece(&WB[wbi++]);
                                }
                            }
                            break;
                        case 2:
                            if (turn % 2) {
                                if (bni < ChessConstants::MAX_KNIGHTS) {
                                    boardState[promote_index] = State::BKNIGHT;
                                    BN[bni].setColor(Color::BLACK, m_renderer);
                                    S[promote_index].setPiece(&BN[bni++]);
                                }
                            } else {
                                if (wni < ChessConstants::MAX_KNIGHTS) {
                                    boardState[promote_index] = State::WKNIGHT;
                                    WN[wni].setColor(Color::WHITE, m_renderer);
                                    S[promote_index].setPiece(&WN[wni++]);
                                }
                            }
                            break;
                        case 3:
                            if (turn % 2) {
                                if (bqi < ChessConstants::MAX_QUEENS) {
                                    boardState[promote_index] = State::BQUEEN;
                                    BQ[bqi].setColor(Color::BLACK, m_renderer);
                                    S[promote_index].setPiece(&BQ[bqi++]);
                                }
                            } else {
                                if (wqi < ChessConstants::MAX_QUEENS) {
                                    boardState[promote_index] = State::WQUEEN;
                                    WQ[wqi].setColor(Color::WHITE, m_renderer);
                                    S[promote_index].setPiece(&WQ[wqi++]);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    this->turn++;

                    // Copies the FE-Notation to boardState
                    Fen f(this->boardState);

                    bool haveValidMoves = hasValidMoves(
                        boardState, turn % 2 ? 'b' : 'w', this->S);

                    // Check if king has "Check"
                    if (f.isCheck(turn % 2 ? Bpos : Wpos,
                                  turn % 2 ? 'b' : 'w')) {
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
                }
            }
        }
        return true;
    }

    return false;
}

void Board::clickedPosition(int x, int y) {
    // Check button clicks first
    if (checkButtonClick(x, y)) return;
    
    // Check if click is within board bounds
    int actualBoardSize = squareSize * ChessConstants::BOARD_SIZE;
    if (x < boardOffsetX || x >= boardOffsetX + actualBoardSize ||
        y < boardOffsetY || y >= boardOffsetY + actualBoardSize) {
        return;  // Click outside the board
    }
    
    // Calculate board position accounting for offset
    int j = (x - boardOffsetX) / squareSize;
    int i = (y - boardOffsetY) / squareSize;
    int index = i * ChessConstants::BOARD_SIZE + j;
    
    // Validate index
    if (index < 0 || index >= 64) return;

    if (popWindow(x, y)) return;

    Square* sq = &S[index];

    if (sq->isHighlighted) {
        // Calculates the position of the both of the kings
        int Wpos = -1;
        int Bpos = -1;
        int mask = ~(State::VALID | State::PROMOTION | State::CASTLE);
        for (int k = 0; k < 64; k++) {
            if ((this->boardState[k] & mask) == State::WKING) {
                Wpos = k;
            }
            if ((this->boardState[k] & mask) == State::BKING) {
                Bpos = k;
            }
        }
        if (Wpos < 0 || Bpos < 0) return;

        // Assume we have no check for both the kings
        S[Bpos].isCheck = false;
        S[Wpos].isCheck = false;

        // Move the piece
        movePiece(sq, index);

        clearHighlighted();

        if (this->promotion) {
            this->promotion = false;
            this->popupColor = turn % 2 ? Color::BLACK : Color::WHITE;
            this->showPopup = true;
            this->promote_index = index;
            return;
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
                    won = Color::WHITE;
                } else {
                    printf("BLACK WINS!!\n");
                    won = Color::BLACK;
                }
                end = true;
            }
        } else if (!haveValidMoves) {
            printf("StaleMate\n");
            end = true;
            won = Color::NONE;
        }

        return;
    }

    clearHighlighted();

    if (!sq->empty) {
        // To have a systematic turns
        int mask = ~(State::VALID | State::PROMOTION | State::CASTLE);
        int pieceType = boardState[index] & mask;
        if ((pieceType >= State::WROOK && pieceType <= State::WPAWN && turn % 2 != 0)
            || (pieceType >= State::BROOK && pieceType <= State::BPAWN && turn % 2 == 0))
            return;

        Piece* p = sq->getPiece();
        if (!p) return;

        selectedSquare = sq;
        selectedSquareIndex = index;
        sq->isSelected = true;  // Highlight selected square
        p->getValidMoves(boardState, index);

        castlingMoves(index);

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

void Board::castlingMoves(int index) {
    int BS[64];

    for (int i = 0; i < 64; i++) BS[i] = boardState[i];

    if (boardState[index] == State::BKING
        && !Fen(boardState).isCheck(index, 'b')) {
        if (this->castling & Fen::BLC) {
            bool exit = false;
            for (int i = 1; i < index; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index - 1] = State::BROOK;
                BS[index - 2] = State::BKING;
                BS[index - 4] = State::NONE;
                if (!Fen(BS).isCheck(index - 2, 'b'))
                    boardState[index - 2] |= State::VALID | State::LCASTLE;
                BS[index] = State::BKING;
                BS[index - 1] = State::NONE;
                BS[index - 2] = State::NONE;
                BS[index - 4] = State::BROOK;
            }
        }

        if (this->castling & Fen::BSC) {
            bool exit = false;
            for (int i = index + 1; i < 7; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index + 1] = State::BROOK;
                BS[index + 2] = State::BKING;
                BS[index + 3] = State::NONE;
                if (!Fen(BS).isCheck(index + 2, 'b'))
                    boardState[index + 2] |= State::VALID | State::SCASTLE;
                BS[index] = State::BKING;
                BS[index + 1] = State::NONE;
                BS[index + 2] = State::NONE;
                BS[index + 3] = State::BROOK;
            }
        }
    } else if (boardState[index] == State::WKING
               && !Fen(boardState).isCheck(index, 'w')) {
        if (this->castling & Fen::WLC) {
            bool exit = false;
            for (int i = 57; i < index; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index - 1] = State::WROOK;
                BS[index - 2] = State::WKING;
                BS[index - 4] = State::NONE;
                if (!Fen(BS).isCheck(index - 2, 'w'))
                    boardState[index - 2] |= State::VALID | State::LCASTLE;
                BS[index] = State::WKING;
                BS[index - 1] = State::NONE;
                BS[index - 2] = State::NONE;
                BS[index - 4] = State::WROOK;
            }
        }

        if (this->castling & Fen::WSC) {
            bool exit = false;
            for (int i = index + 1; i < 63; ++i) {
                if ((boardState[i] & ~State::VALID) != State::NONE) {
                    exit = true;
                    break;
                }
            }

            if (!exit) {
                BS[index] = State::NONE;
                BS[index + 1] = State::WROOK;
                BS[index + 2] = State::WKING;
                BS[index + 3] = State::NONE;
                if (!Fen(BS).isCheck(index + 2, 'w'))
                    boardState[index + 2] |= State::VALID | State::SCASTLE;
                BS[index] = State::WKING;
                BS[index + 1] = State::NONE;
                BS[index + 2] = State::NONE;
                BS[index + 3] = State::WROOK;
            }
        }
    }
}

void Board::handleResize(int newWidth, int newHeight) {
    this->width = newWidth;
    this->height = newHeight;
    this->windowWidth = newWidth;
    this->windowHeight = newHeight;
    
    // Recalculate layout (board offset, button positions)
    updateLayout();
    
    // Update all squares with new positions and sizes
    for (int i = 0; i < ChessConstants::BOARD_SIZE; i++) {
        for (int j = 0; j < ChessConstants::BOARD_SIZE; j++) {
            int idx = i * ChessConstants::BOARD_SIZE + j;
            S[idx].set_values(
                boardOffsetX + j * squareSize, 
                boardOffsetY + i * squareSize, 
                squareSize, 
                (i + j) % 2 == 0 ? Color::WHITE : Color::BLACK, 
                m_renderer);
            // Update piece position if square has a piece
            if (!S[idx].empty) {
                S[idx].getPiece()->setPosition(
                    boardOffsetX + j * squareSize, 
                    boardOffsetY + i * squareSize, 
                    squareSize);
            }
        }
    }
    
    // Update popup rect positions
    int w = 75, h = 75;
    if (promotionPieces[0]) {
        SDL_QueryTexture(promotionPieces[0], NULL, NULL, &w, &h);
    }
    this->popupRect.w = (w * 4) + 10;
    this->popupRect.h = (h) + 10;
    this->popupRect.x = (windowWidth / 2) - (this->popupRect.w / 2);
    this->popupRect.y = (windowHeight / 2) - (this->popupRect.h / 2);
    for (int i = 0; i < 4; ++i) {
        this->popupImgRects[i] = {popupRect.x + (i * w), popupRect.y, w, h};
    }
}
