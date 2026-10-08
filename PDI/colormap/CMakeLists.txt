cmake_minimum_required(VERSION 3.10)
project(ColormapPDI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Altera aqui de colormaps_ex.cpp para colormap.cpp
add_executable(colormap colormap.cpp)
target_link_libraries(colormap ${OpenCV_LIBS})