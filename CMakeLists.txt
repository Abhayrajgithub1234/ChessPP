cmake_minimum_required(VERSION 3.15...3.31)

project(ChessPP DESCRIPTION "Chess game with c++ along OOP" LANGUAGES CXX)


add_executable(chess)

target_link_libraries(chess PRIVATE SDL2)

add_subdirectory(src)