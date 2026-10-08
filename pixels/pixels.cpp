#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: ./pixels <caminho_da_imagem>" << std::endl;
    return -1;
  }

  // Carrega a imagem em escala de cinza
  cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    std::cout << "Erro ao abrir a imagem!" << std::endl;
    return -1;
  }

  int x1, y1, x2, y2;
  std::cout << "Dimensões da imagem: " << image.cols << "x" << image.rows << std::endl;
  std::cout << "Digite as coordenadas de P1 (x y): ";
  std::cin >> x1 >> y1;
  std::cout << "Digite as coordenadas de P2 (x y): ";
  std::cin >> x2 >> y2;

  // Garante a ordenação correta das coordenadas
  int x_min = std::max(0, std::min(x1, x2));
  int x_max = std::min(image.cols - 1, std::max(x1, x2));
  int y_min = std::max(0, std::min(y1, y2));
  int y_max = std::min(image.rows - 1, std::max(y1, y2));

  // Aplica o efeito negativo na região retangular
  for (int i = y_min; i <= y_max; i++) {
    for (int j = x_min; j <= x_max; j++) {
      image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
    }
  }

  // Salva a imagem gerada no disco
  cv::imwrite("resultado_negativo.png", image);
  std::cout << "Imagem processada salva como 'resultado_negativo.png'!" << std::endl;

  // Exibe o resultado na tela
  cv::imshow("Pixels - Regiao Negativa", image);
  cv::waitKey(0);

  return 0;
}