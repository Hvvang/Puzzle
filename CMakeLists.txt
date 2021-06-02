cmake_minimum_required(VERSION 3.17)

project(Puzzle VERSION 1.0 LANGUAGES CXX)

set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(Engine)
add_subdirectory(game)