#pragma once

enum State {
    NONE = 0,

    WROOK,
    WKNIGHT,
    WBISHOP,
    WQUEEN,
    WKING,
    WPAWN,

    BROOK,
    BKNIGHT,
    BBISHOP,
    BQUEEN,
    BKING,
    BPAWN,

    // CHECK,
    // ENPASSANT

    VALID = 1 << 10,
    PROMOTION = 1 << 11,
    LCASTLE = 1 << 12,
    SCASTLE = 1 << 13,
    CASTLE = LCASTLE | SCASTLE
};
