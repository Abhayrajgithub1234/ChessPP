#include "app.h"
#include "constants.h"

int main() {
    App app(ChessConstants::DEFAULT_WINDOW_WIDTH, 
            ChessConstants::DEFAULT_WINDOW_HEIGHT, "ChessPP");
    app.run();
}
