cmake_minimum_required(VERSION 3.11)
project(blank)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_find")

find_package(SDL2 CONFIG REQUIRED)

include_directories(
		${SDL2_INCLUDE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

set(SRC_FILES
    src/main.cpp
    src/game.c
    src/player.c
    src/projectile.c
    src/util.c
)

add_executable(blank ${SRC_FILES})

target_include_directories(blank PUBLIC
    ${SDL2_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)


target_link_libraries(blank LINK_PUBLIC ${SDL2_LIBRARY} m)

target_link_libraries(blank
		PRIVATE
		$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
		$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
