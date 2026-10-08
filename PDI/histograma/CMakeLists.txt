cmake_minimum_required(VERSION 3.25.0)
project(cap12_histograma VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Executável original do tutorial
add_executable(histogram histogram.cpp)
target_link_libraries(histogram ${OpenCV_LIBS})

# Exercício 1
add_executable(equalize equalize.cpp)
target_link_libraries(equalize ${OpenCV_LIBS})

# Exercício 2
add_executable(motiondetector motiondetector.cpp)
target_link_libraries(motiondetector ${OpenCV_LIBS})