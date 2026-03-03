cmake_minimum_required(VERSION 3.15...3.31)

project(ChessPP
    DESCRIPTION "Chess game with C++ and OOP"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Homebrew prefix for macOS (works for both Intel and Apple Silicon)
if(APPLE)
    execute_process(COMMAND brew --prefix OUTPUT_VARIABLE HOMEBREW_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
    list(APPEND CMAKE_PREFIX_PATH "${HOMEBREW_PREFIX}")
endif()

# Find SDL2 libraries installed via Homebrew
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_GFX REQUIRED SDL2_gfx)

# Define the executable (sources are added in src/CMakeLists.txt)
add_executable(chess)

# Add source directory
add_subdirectory(src)

# Copy assets to build directory
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Include SDL2 headers and link libraries
target_include_directories(chess PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE_DIRS})
target_link_directories(chess PRIVATE ${SDL2_LIBRARY_DIRS} ${SDL2_TTF_LIBRARY_DIRS} ${SDL2_GFX_LIBRARY_DIRS})
target_link_libraries(chess PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_GFX_LIBRARIES})

# Add compiler flags if needed
target_compile_options(chess PRIVATE ${SDL2_CFLAGS_OTHER} ${SDL2_TTF_CFLAGS_OTHER} ${SDL2_GFX_CFLAGS_OTHER})
