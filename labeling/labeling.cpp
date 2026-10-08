#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  std::string filename = (argc > 1) ? argv[1] : "bolhas.png";
  cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    std::cerr << "Erro ao carregar a imagem: " << filename << std::endl;
    return -1;
  }

  int width = image.cols;
  int height = image.rows;

  // 1. Eliminar bolhas que tocam as bordas
  // Linhas superior e inferior
  for (int j = 0; j < width; j++) {
    if (image.at<uchar>(0, j) == 255)
      cv::floodFill(image, cv::Point(j, 0), 0);
    if (image.at<uchar>(height - 1, j) == 255)
      cv::floodFill(image, cv::Point(j, height - 1), 0);
  }
  // Colunas esquerda e direita
  for (int i = 0; i < height; i++) {
    if (image.at<uchar>(i, 0) == 255)
      cv::floodFill(image, cv::Point(0, i), 0);
    if (image.at<uchar>(i, width - 1) == 255)
      cv::floodFill(image, cv::Point(width - 1, i), 0);
  }

  // 2. Mudar a cor do fundo externo para 1
  cv::floodFill(image, cv::Point(0, 0), 1);

  int bolhas_sem_buraco = 0;
  int bolhas_com_buraco = 0;
  int total_bolhas = 0;

  // 3. Contagem e classificação das bolhas
  // Varredura para encontrar e identificar cada bolha
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      // Encontrou um objeto branco (bolha)
      if (image.at<uchar>(i, j) == 255) {
        total_bolhas++;
        
        // Aplica floodFill na bolha para identificá-la temporariamente
        // Mantém-se o valor em 254 para rastrear e processar buracos internos
        cv::floodFill(image, cv::Point(j, i), 254);
      }
    }
  }

  // Varredura para contar os buracos internos (pixels de valor 0)
  // Como o fundo externo é 1, qualquer pixel 0 remanescente é um buraco interno
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (image.at<uchar>(i, j) == 0) {
        // Encontrou um buraco!
        // Verifica o vizinho para saber a qual bolha ele pertence e marca
        cv::floodFill(image, cv::Point(j, i), 253); 
        
        // Se o vizinho for uma bolha ativa (254), altera o rótulo para indicar que tem buraco
        if (j > 0 && image.at<uchar>(i, j - 1) == 254) {
          cv::floodFill(image, cv::Point(j - 1, i), 252);
        } else if (i > 0 && image.at<uchar>(i - 1, j) == 254) {
          cv::floodFill(image, cv::Point(j, i - 1), 252);
        }
      }
    }
  }

  // Contagem final por tipo de rótulo
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      uchar val = image.at<uchar>(i, j);
      if (val == 254) { // Bolha sem buraco
        bolhas_sem_buraco++;
        cv::floodFill(image, cv::Point(j, i), 100);
      } else if (val == 252) { // Bolha com buraco(s)
        bolhas_com_buraco++;
        cv::floodFill(image, cv::Point(j, i), 200);
      }
    }
  }

  std::cout << "--- Resultado do Processamento ---" << std::endl;
  std::cout << "Total de bolhas validas: " << (bolhas_sem_buraco + bolhas_com_buraco) << std::endl;
  std::cout << "Bolhas sem buracos: " << bolhas_sem_buraco << std::endl;
  std::cout << "Bolhas com buracos: " << bolhas_com_buraco << std::endl;

  cv::imshow("Bolhas Processadas", image);
  cv::imwrite("resultado_labeling.png", image);
  cv::waitKey(0);

  return 0;
}