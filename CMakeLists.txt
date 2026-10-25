cmake_minimum_required(VERSION 3.11)
project(blank)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_find")

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

set(SRC_FILES
    src/main.cpp
    src/game.cpp
    src/player.cpp
    src/projectile.cpp
    src/util.cpp
    src/font.cpp
    src/timer.cpp
    src/music.cpp
    src/hud.cpp
    src/puzzle.cpp
    src/drag_crate_puzzle.cpp
    src/button_click_puzzle.cpp
    src/puzzle.cpp
    src/drag_crate_puzzle.cpp
    src/button_click_puzzle.cpp
    src/puzzle_spawner.cpp
)

add_executable(blank ${SRC_FILES})

target_include_directories(blank PUBLIC
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

target_link_libraries(blank
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    SDL2_image
    SDL2_mixer
    m
)

add_custom_command(TARGET blank POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:blank>/assets)
