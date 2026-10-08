cmake_minimum_required(VERSION 3.25.0)
project(cap14_convolucao VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Localiza a biblioteca OpenCV instalada no sistema
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 1. Programa original ajustado para testar os efeitos de convolucao
add_executable(convolucao convolucao.cpp)
target_link_libraries(convolucao ${OpenCV_LIBS})

# 2. Exercício do Filtro da Média com diferentes tamanhos de máscara (3x3, 11x11, 21x21)
add_executable(convolucao2 convolucao2.cpp)
target_link_libraries(convolucao2 ${OpenCV_LIBS})

# 3. Exercício de correção de profundidade de campo (Foco Estendido)
add_executable(depthoffield depthoffield.cpp)
target_link_libraries(depthoffield ${OpenCV_LIBS})