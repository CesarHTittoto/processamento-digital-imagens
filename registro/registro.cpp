#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

// Estrutura para passar dados para a funcao callback do mouse
struct MouseData {
  cv::Mat imgOriginal;
  cv::Mat imgDisplay;
  std::vector<cv::Point2f> srcPoints;
};

// Funcao callback do mouse
void onMouse(int event, int x, int y, int flags, void* userdata) {
  MouseData* data = reinterpret_cast<MouseData*>(userdata);

  if (event == cv::EVENT_LBUTTONDOWN) {
    if (data->srcPoints.size() < 4) {
      cv::Point2f pt(x, y);
      data->srcPoints.push_back(pt);

      std::cout << "Ponto " << data->srcPoints.size() << " selecionado: (" << x << ", " << y << ")" << std::endl;

      // Desenha o ponto e a ordem do clique na imagem de exibicao
      cv::circle(data->imgDisplay, pt, 5, cv::Scalar(0, 0, 255), -1);
      cv::putText(data->imgDisplay, std::to_string(data->srcPoints.size()), 
                  cv::Point(x + 8, y + 8), cv::FONT_HERSHEY_SIMPLEX, 0.6, 
                  cv::Scalar(0, 255, 0), 2);
      
      cv::imshow("Selecione 4 Pontos", data->imgDisplay);

      // Quando os 4 pontos forem selecionados, realiza a correcao
      if (data->srcPoints.size() == 4) {
        // Calcula a largura e altura estimadas da imagem corrigida
        float largura = cv::max(
          cv::norm(data->srcPoints[0] - data->srcPoints[1]),
          cv::norm(data->srcPoints[2] - data->srcPoints[3])
        );

        float altura = cv::max(
          cv::norm(data->srcPoints[1] - data->srcPoints[2]),
          cv::norm(data->srcPoints[3] - data->srcPoints[0])
        );

        std::cout << "\nDimensoes calculadas:" << std::endl;
        std::cout << "Largura: " << largura << " px" << std::endl;
        std::cout << "Altura: " << altura << " px" << std::endl;

        // Define os 4 pontos de destino (retangulo plano)
        std::vector<cv::Point2f> dstPoints;
        dstPoints.push_back(cv::Point2f(0, 0));
        dstPoints.push_back(cv::Point2f(largura, 0));
        dstPoints.push_back(cv::Point2f(largura, altura));
        dstPoints.push_back(cv::Point2f(0, altura));

        // Calcula a matriz de transformacao de perspectiva
        cv::Mat perspectiveMatrix = cv::getPerspectiveTransform(data->srcPoints, dstPoints);

        // Aplica a correcao de perspectiva
        cv::Mat correctedImage;
        cv::warpPerspective(data->imgOriginal, correctedImage, perspectiveMatrix, cv::Size(largura, altura));

        // Exibe a imagem corrigida
        cv::imshow("Imagem Corrigida", correctedImage);
      }
    }
  }
}

int main(int argc, char** argv) {
  std::string filename = (argc > 1) ? argv[1] : "voltimetro.png";
  
  MouseData data;
  data.imgOriginal = cv::imread(filename);

  if (data.imgOriginal.empty()) {
    std::cerr << "Erro ao carregar a imagem: " << filename << std::endl;
    return -1;
  }

  data.imgDisplay = data.imgOriginal.clone();

  std::cout << "--- Instrucoes ---" << std::endl;
  std::cout << "1. Clique com o BOTAO ESQUERDO do mouse para selecionar 4 pontos na ordem:" << std::endl;
  std::cout << "   [1] Superior Esquerdo -> [2] Superior Direito -> [3] Inferior Direito -> [4] Inferior Esquerdo" << std::endl;
  std::cout << "2. Pressione 'r' para resetar os pontos." << std::endl;
  std::cout << "3. Pressione 'ESC' ou 'q' para sair." << std::endl;

  cv::namedWindow("Selecione 4 Pontos");
  cv::setMouseCallback("Selecione 4 Pontos", onMouse, &data);

  cv::imshow("Selecione 4 Pontos", data.imgDisplay);

  while (true) {
    int key = cv::waitKey(30);
    if (key == 27 || key == 'q' || key == 'Q') { // ESC ou q para sair
      break;
    } else if (key == 'r' || key == 'R') { // Resetar os pontos
      data.srcPoints.clear();
      data.imgDisplay = data.imgOriginal.clone();
      cv::imshow("Selecione 4 Pontos", data.imgDisplay);
      cv::destroyWindow("Imagem Corrigida");
      std::cout << "\nPontos resetados. Selecione 4 novos pontos." << std::endl;
    }
  }

  return 0;
}