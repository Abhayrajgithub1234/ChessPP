set(SRCS
    main.cpp
    app.h
    app.cpp
    colors.h
    square.h
    square.cpp
    board.cpp
    board.h
)

target_sources(chess PRIVATE ${SRCS})

