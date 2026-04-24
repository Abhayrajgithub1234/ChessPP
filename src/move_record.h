#pragma once

#include "state.h"
#include "constants.h"

// Structure to store all information needed to undo/redo a move
struct MoveRecord {
    // Move positions
    int fromIndex;
    int toIndex;
    
    // Board state before the move
    int boardStateBefore[ChessConstants::TOTAL_SQUARES];
    
    // Piece states
    int movedPieceState;      // State of piece that moved
    int capturedPieceState;   // State of piece that was captured (NONE if no capture)
    
    // Castling state before the move
    int castlingBefore;
    
    // Turn before the move
    int turnBefore;
    
    // Special move flags
    bool wasCastleShort;
    bool wasCastleLong;
    bool wasPromotion;
    int promotedTo;           // What piece the pawn was promoted to
    
    // Last move tracking (to restore highlights)
    int prevLastMoveFrom;
    int prevLastMoveTo;
    
    // King check positions (to restore check highlights)
    int whiteKingPos;
    int blackKingPos;
    bool whiteKingWasInCheck;
    bool blackKingWasInCheck;
    
    MoveRecord() : fromIndex(-1), toIndex(-1), movedPieceState(State::NONE),
                   capturedPieceState(State::NONE), castlingBefore(0), turnBefore(0),
                   wasCastleShort(false), wasCastleLong(false), wasPromotion(false),
                   promotedTo(State::NONE), prevLastMoveFrom(-1), prevLastMoveTo(-1),
                   whiteKingPos(-1), blackKingPos(-1), whiteKingWasInCheck(false),
                   blackKingWasInCheck(false) {
        for (int i = 0; i < ChessConstants::TOTAL_SQUARES; i++) {
            boardStateBefore[i] = State::NONE;
        }
    }
};
