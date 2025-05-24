cmake_minimum_required(VERSION 3.11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Utilities
        DialogBox.h.cpp
        Game.cpp
        object.h.cpp
        Vector2D.h.cpp
        TextHandler.cpp
        TextureHandler.cpp
        AnimationHandler.cpp
        TweenService.cpp
)

target_link_libraries(Utilities raylib)

target_include_directories(Utilities PUBLIC
        "${PROJECT_BINARY_DIR}"
)