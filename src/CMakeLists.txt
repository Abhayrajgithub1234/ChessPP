set(SRCS
    main.cpp
    app.h
    app.cpp
    colors.h
    square.h
    square.cpp
    board.cpp
    board.h
    piece.h
    pawn.h
    pawn.cpp
    king.h
    king.cpp
    queen.h
    queen.cpp
    rook.h
    rook.cpp
    knight.h
    knight.cpp
    bishop.h
    bishop.cpp
    state.h
    fen.h
    fen.cpp
    
)

target_sources(chess PRIVATE ${SRCS})

