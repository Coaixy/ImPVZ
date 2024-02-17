cmake_minimum_required(VERSION 3.27)
project(ImPVZ)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/outpus)

FILE(GLOB_RECURSE FILE_INCLUDES include/*.h*)
include_directories(
        include/imgui
        include
)

FILE(GLOB_RECURSE FILE_SOURCE src/*.c*)

add_executable(Imgmake
        ${FILE_INCLUDES}
        ${FILE_SOURCE}
        src/application.cpp
)
target_link_directories(
        ImPVZ
        PRIVATE
        WIN32
)