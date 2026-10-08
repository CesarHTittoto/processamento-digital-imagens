#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: ./trocaregioes <caminho_da_imagem>" << std::endl;
    return -1;
  }

  // Carrega a imagem
  cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    std::cout << "Erro ao carregar a imagem." << std::endl;
    return -1;
  }

  int width = image.cols;
  int height = image.rows;
  int half_width = width / 2;
  int half_height = height / 2;

  // Criar matriz de saída com o mesmo tamanho e tipo
  cv::Mat result = cv::Mat::zeros(image.size(), image.type());

  // Definir as ROIs (Regiões de Interesse) da imagem original
  cv::Mat qA(image, cv::Rect(0, 0, half_width, half_height));
  cv::Mat qB(image, cv::Rect(half_width, 0, half_width, half_height));
  cv::Mat qC(image, cv::Rect(0, half_height, half_width, half_height));
  cv::Mat qD(image, cv::Rect(half_width, half_height, half_width, half_height));

  // Copiar os quadrantes invertidos na diagonal para a imagem final
  qA.copyTo(result(cv::Rect(half_width, half_height, half_width, half_height))); // A -> D
  qD.copyTo(result(cv::Rect(0, 0, half_width, half_height)));                     // D -> A
  qB.copyTo(result(cv::Rect(0, half_height, half_width, half_height)));           // B -> C
  qC.copyTo(result(cv::Rect(half_width, 0, half_width, half_height)));           // C -> B

  // Salvar o resultado para a documentacao
  cv::imwrite("resultado_trocaregioes.png", result);
  std::cout << "Resultado guardado como 'resultado_trocaregioes.png'!" << std::endl;

  // Exibir o resultado
  cv::imshow("Troca de Quadrantes", result);
  cv::waitKey(0);

  return 0;
}