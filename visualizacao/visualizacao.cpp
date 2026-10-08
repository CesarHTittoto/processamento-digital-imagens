#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: ./visualizacao <caminho_da_imagem>" << std::endl;
    return -1;
  }

  // Carrega a imagem em escala de cinza
  cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    std::cout << "Erro ao carregar a imagem: " << argv[1] << std::endl;
    return -1;
  }

  std::ofstream fmatriz("matriz.txt");
  std::ofstream fline("line.txt");

  if (!fmatriz.is_open() || !fline.is_open()) {
    std::cout << "Erro ao criar os arquivos de saída (matriz.txt / line.txt)!" << std::endl;
    return -1;
  }

  int mid_row = image.rows / 2;

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      int val = static_cast<int>(image.at<uchar>(i, j));
      
      // Escreve a matriz de intensidades
      fmatriz << val << (j == image.cols - 1 ? "" : " ");
      
      // Extrai o perfil de intensidade da linha central da imagem
      if (i == mid_row) {
        fline << j << " " << val << "\n";
      }
    }
    fmatriz << "\n";
  }

  fmatriz.close();
  fline.close();

  std::cout << "Sucesso! Arquivos matriz.txt e line.txt gerados para " << argv[1] << std::endl;

  return 0;
}