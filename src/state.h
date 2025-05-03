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

    PROMOTION = 1 << 11,
    // CHECK,
    // ENPASSANT

    VALID = 1 << 10

};
