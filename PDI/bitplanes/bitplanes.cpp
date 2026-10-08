#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: " << argv[0] << " <desafio-esteganografia.png>" << std::endl;
    return -1;
  }

  // Carrega a imagem codificada passada por linha de comando
  cv::Mat imagemEsteganografada = cv::imread(argv[1], cv::IMREAD_COLOR);

  if (imagemEsteganografada.empty()) {
    std::cout << "Erro ao carregar a imagem: " << argv[1] << std::endl;
    return -1;
  }

  cv::Mat imagemRecuperada = imagemEsteganografada.clone();
  cv::Vec3b valEsteg, valRec;

  // Numero de bits menos significativos utilizados na codificacao
  int nbits = 3; 

  for (int i = 0; i < imagemEsteganografada.rows; i++) {
    for (int j = 0; j < imagemEsteganografada.cols; j++) {
      valEsteg = imagemEsteganografada.at<cv::Vec3b>(i, j);

      // Isolamento dos bits menos significativos e deslocamento para os bits mais significativos
      for (int c = 0; c < 3; c++) {
        // Limpa os bits mais significativos mantendo apenas os 'nbits' menos significativos
        // Em seguida, desloca para a esquerda (8 - nbits) posicoes
        valRec[c] = (valEsteg[c] << (8 - nbits)) & 0xFF;
      }

      imagemRecuperada.at<cv::Vec3b>(i, j) = valRec;
    }
  }

  // Exibe e salva a imagem recuperada
  cv::imshow("Imagem Recuperada", imagemRecuperada);
  cv::imwrite("imagem_recuperada.png", imagemRecuperada);

  std::cout << "Imagem escondida recuperada e salva com sucesso em 'imagem_recuperada.png'!" << std::endl;
  cv::waitKey(0);

  return 0;
}