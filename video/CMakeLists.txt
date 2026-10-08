cmake_minimum_required(VERSION 3.10)
project(video)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Força o suporte a threads POSIX do MinGW/GCC no nível do compilador
add_compile_options(-pthread)
add_definitions(-D_REENTRANT)

find_package(OpenCV REQUIRED)

add_executable(video video.cpp)
target_link_libraries(video PRIVATE ${OpenCV_LIBS} -pthread)

add_executable(videocinza videocinza.cpp)
target_link_libraries(videocinza PRIVATE ${OpenCV_LIBS} -pthread)

add_executable(videonegativo videonegativo.cpp)
target_link_libraries(videonegativo PRIVATE ${OpenCV_LIBS} -pthread)