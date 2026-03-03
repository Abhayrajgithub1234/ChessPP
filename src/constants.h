#pragma once

namespace ChessConstants {
    // Board dimensions
    constexpr int BOARD_SIZE = 8;
    constexpr int TOTAL_SQUARES = BOARD_SIZE * BOARD_SIZE;
    
    // Square size (will be calculated based on window size)
    constexpr int DEFAULT_SQUARE_SIZE = 75;
    
    // Default window dimensions
    constexpr int DEFAULT_WINDOW_WIDTH = 700;
    constexpr int DEFAULT_WINDOW_HEIGHT = 650;
    
    // Minimum window dimensions
    constexpr int MIN_WINDOW_WIDTH = 500;
    constexpr int MIN_WINDOW_HEIGHT = 500;
    
    // Piece array sizes (base pieces + max promotions possible)
    constexpr int MAX_PAWNS = 8;
    constexpr int MAX_ROOKS = 10;     // 2 original + 8 possible promotions
    constexpr int MAX_KNIGHTS = 10;   // 2 original + 8 possible promotions
    constexpr int MAX_BISHOPS = 10;   // 2 original + 8 possible promotions
    constexpr int MAX_QUEENS = 9;     // 1 original + 8 possible promotions
    
    // Highlight circle radius
    constexpr int HIGHLIGHT_RADIUS = 20;
    
    // Font size for end game text
    constexpr int END_GAME_FONT_SIZE = 48;
    constexpr int BUTTON_FONT_SIZE = 20;
    constexpr int TURN_FONT_SIZE = 24;
    
    // Asset paths
    constexpr const char* FONT_PATH = "assets/RobotoMonoNerdFont-Bold.ttf";
    constexpr const char* ASSET_DIR = "assets/";
    
    // Animation settings
    constexpr int ANIMATION_DURATION_MS = 150;  // Duration of piece movement animation
    constexpr int ANIMATION_FPS = 60;           // Animation frames per second
    
    // UI Colors (RGB) - Chess.com style
    namespace Colors {
        // Background (chess.com dark brown)
        constexpr int BG_R = 49, BG_G = 46, BG_B = 43;
        
        // Light squares (cream - chess.com style)
        constexpr int LIGHT_R = 238, LIGHT_G = 238, LIGHT_B = 210;
        
        // Dark squares (green - chess.com style)
        constexpr int DARK_R = 118, DARK_G = 150, DARK_B = 86;
        
        // Last move highlight (yellow-green tint)
        constexpr int LAST_MOVE_R = 186, LAST_MOVE_G = 202, LAST_MOVE_B = 68;
        
        // Highlight for possible moves (grey dots)
        constexpr int HIGHLIGHT_R = 0, HIGHLIGHT_G = 0, HIGHLIGHT_B = 0;
        
        // Check (red gradient)
        constexpr int CHECK_R = 235, CHECK_G = 97, CHECK_B = 80;
        
        // Selected square (slight tint)
        constexpr int SELECTED_R = 186, SELECTED_G = 202, SELECTED_B = 68;
        
        // Button colors (chess.com green theme)
        constexpr int BTN_R = 129, BTN_G = 182, BTN_B = 76;
        constexpr int BTN_HOVER_R = 149, BTN_HOVER_G = 202, BTN_HOVER_B = 96;
        constexpr int BTN_TEXT_R = 255, BTN_TEXT_G = 255, BTN_TEXT_B = 255;
        
        // Board border (dark)
        constexpr int BORDER_R = 30, BORDER_G = 27, BORDER_B = 24;
        
        // Shadow color
        constexpr int SHADOW_R = 20, SHADOW_G = 18, SHADOW_B = 16;
    }
    
    // Button dimensions
    constexpr int BUTTON_WIDTH = 100;
    constexpr int BUTTON_HEIGHT = 40;
    constexpr int BUTTON_MARGIN = 15;
    constexpr int BOARD_PADDING = 20;
}
