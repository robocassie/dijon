cmake_minimum_required(VERSION 3.19)

project(IcyBoy)

find_package(SDL2 2.30.4 REQUIRED)
message("Found SDL2 include dirs at: " ${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE IcyBoy_PLATFORMSRCS "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE IcyBoy_CORESRCS "${CMAKE_SOURCE_DIR}/../../src/*.c")

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/../../include/
)

add_executable(icyboy ${IcyBoy_PLATFORMSRCS} ${IcyBoy_CORESRCS})

target_link_libraries(icyboy ${SDL2_LIBRARIES})