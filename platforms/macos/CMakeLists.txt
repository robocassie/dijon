cmake_minimum_required(VERSION 3.19)
project(dijon)

set (CMAKE_CXX_STANDARD 11)


add_subdirectory(cimgui)

find_package(SDL2 2.30.4 REQUIRED)
message("Found SDL2 include dirs at: " ${SDL2_INCLUDE_DIRS})

file(GLOB_RECURSE DIJON_PLATFORMSRCS "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB_RECURSE DIJON_CORESRCS "${CMAKE_SOURCE_DIR}/../../src/*.c")
file(GLOB_RECURSE ImGui_BACKENDSRCS "${CMAKE_SOURCE_DIR}/cimgui/imgui/backends/*.cpp")

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/../../include/
    ${CMAKE_SOURCE_DIR}/cimgui/
)

add_executable(dijon ${DIJON_PLATFORMSRCS} ${DIJON_CORESRCS} ${ImGui_BACKENDSRCS})

target_link_libraries(dijon ${SDL2_LIBRARIES})
target_link_libraries(dijon cimgui)