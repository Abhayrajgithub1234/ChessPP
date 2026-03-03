#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "bishop.h"
#include "constants.h"
#include "fen.h"
#include "king.h"
#include "knight.h"
#include "pawn.h"
#include "queen.h"
#include "rook.h"
#include "square.h"
#include "state.h"

class Board {
    private:
        int width, height;
        int squareSize;  // Computed based on window size
        int boardOffsetX, boardOffsetY;  // Offset for centering board
        SDL_Renderer* m_renderer;
        SDL_Surface* m_windowSurface;
        Square S[ChessConstants::TOTAL_SQUARES];

        Pawn BP[ChessConstants::MAX_PAWNS], WP[ChessConstants::MAX_PAWNS];
        King BK, WK;
        Queen BQ[ChessConstants::MAX_QUEENS], WQ[ChessConstants::MAX_QUEENS];
        Rook BR[ChessConstants::MAX_ROOKS], WR[ChessConstants::MAX_ROOKS];
        Knight BN[ChessConstants::MAX_KNIGHTS], WN[ChessConstants::MAX_KNIGHTS];
        Bishop BB[ChessConstants::MAX_BISHOPS], WB[ChessConstants::MAX_BISHOPS];
        int bpi = 0, wpi = 0, bri = 0, wri = 0, bbi = 0, wbi = 0, bni = 0,
            wni = 0, bqi = 0, wqi = 0;

        int boardState[ChessConstants::TOTAL_SQUARES];
        Fen fen;
        Square* selectedSquare;
        int selectedSquareIndex;

        int turn = 0;

        bool promotion = false;

        SDL_Rect popupRect;
        Color popupColor;
        bool showPopup = false;
        int promote_index;
        SDL_Texture* promotionPieces[8];
        SDL_Rect popupImgRects[4];

        bool end = false;
        Color won;

        int windowWidth, windowHeight;
        int castling = Fen::ALL;
        
        // UI Elements
        SDL_Rect resetButtonRect;
        SDL_Rect newGameButtonRect;
        bool resetHovered = false;
        bool newGameHovered = false;
        
        // Last move tracking (for highlighting)
        int lastMoveFrom = -1;
        int lastMoveTo = -1;

    private:
        void clearHighlighted();
        void movePiece(Square* sq, int index);
        void showOptions();
        void drawGameEndScreen();
        bool popWindow(int x, int y);
        void castlingMoves(int index);
        void drawBackground();
        void drawUI();
        void drawButton(SDL_Rect& rect, const char* text, bool hovered);
        void drawTurnIndicator();
        void drawBoardCoordinates();
        void initializePieces();
        void updateLayout();

    public:
        Board(int width, int height, SDL_Window* window);
        ~Board();
        void draw();
        void clickedPosition(int x, int y);
        void handleResize(int newWidth, int newHeight);
        void reset();
        bool checkButtonClick(int x, int y);
};
