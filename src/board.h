#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "bishop.h"
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
        SDL_Renderer* m_renderer;
        SDL_Surface* m_windowSurface;
        Square S[64];

        Pawn BP[8], WP[8];
        King BK, WK;
        Queen BQ[1 + 8], WQ[1 + 8];
        Rook BR[2 + 8], WR[2 + 8];
        Knight BN[2 + 8], WN[2 + 8];
        Bishop BB[2 + 8], WB[2 + 8];
        int bpi = 0, wpi = 0, bri = 0, wri = 0, bbi = 0, wbi = 0, bni = 0,
            wni = 0, bqi = 0, wqi = 0;

        int boardState[64];
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

    private:
        void clearHighlighted();
        void movePiece(Square* sq, int index);
        void showOptions();
        void drawGameEndScreen();
        bool popWindow(int x, int y);
        void castlingMoves(int index);

    public:
        Board(int width, int height, SDL_Window* window);
        ~Board();
        void draw();
        void clickedPosition(int x, int y);
};
