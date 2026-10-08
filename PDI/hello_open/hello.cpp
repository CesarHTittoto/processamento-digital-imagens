#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: ./hello <caminho_da_imagem>" << std::endl;
    return -1;
  }

  std::cout << "Tentando abrir a imagem: " << argv[1] << std::endl;

  cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    std::cout << "Erro: Nao foi possivel carregar o arquivo: " << argv[1] << std::endl;
    return -1;
  }

  std::cout << "Imagem carregada com sucesso! Tamanho: " 
            << image.rows << "x" << image.cols << std::endl;

  cv::imshow("Hello OpenCV", image);
  std::cout << "Pressione qualquer tecla sobre a janela da imagem para fechar..." << std::endl;
  cv::waitKey(0);

  return 0;
}