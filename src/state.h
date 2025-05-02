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

    // PROMOTION,
    // CHECK,
    // ENPASSANT

    VALID = 1 << 10

};
