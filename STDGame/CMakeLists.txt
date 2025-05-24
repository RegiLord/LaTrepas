cmake_minimum_required(VERSION 3.11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(STDGame
        QuickTime.cpp
        Player.cpp
)

target_link_libraries(STDGame raylib)
target_link_libraries(STDGame GUI)
target_link_libraries(STDGame Utilities)

target_include_directories(GUI PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/GUI"
        "${PROJECT_SOURCE_DIR}/Utilities"
        "${PROJECT_SOURCE_DIR}"
)