cmake_minimum_required(VERSION 3.10)
project(VisualizacaoPDI)

# Define o padrão do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Localiza o pacote do OpenCV
find_package(OpenCV REQUIRED)

# Inclui os diretórios de cabeçalho do OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Define o executável a partir do arquivo C++
add_executable(visualizacao visualizacao.cpp)

# Vincula as bibliotecas do OpenCV ao executável
target_link_libraries(visualizacao ${OpenCV_LIBS})