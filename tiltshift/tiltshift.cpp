#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

// Variaveis globais de parametros
int height_slider = 20;      // Altura da faixa em foco (h)
int height_slider_max = 100;

int decay_slider = 10;       // Forca de decaimento (delta)
int decay_slider_max = 100;

int pos_slider = 50;         // Posicao vertical do centro (y0)
int pos_slider_max = 100;

cv::Mat imageOriginal, imageBlurred, imageResult;

void processTiltShift() {
  int rows = imageOriginal.rows;
  int cols = imageOriginal.cols;

  // Mapeamento dos valores dos sliders para parametros reais
  double h = (height_slider / (double)height_slider_max) * rows;
  double y0 = (pos_slider / (double)pos_slider_max) * rows;
  double delta = decay_slider + 1.0; // Evita divisao por zero

  // Matriz de peso alfa (3 canais)
  cv::Mat alpha = cv::Mat::zeros(rows, cols, CV_32FC3);

  for (int r = 0; r < rows; r++) {
    // Equacao da mascara baseada na funcao tangente hiperbolica (sigmoide)
    double val = 0.5 * (std::tanh((r - y0 + h / 2.0) / delta) - std::tanh((r - y0 - h / 2.0) / delta));
    
    // Atribui o peso calculado para todos os canais (BGR) do pixel
    for (int c = 0; c < cols; c++) {
      alpha.at<cv::Vec3f>(r, c) = cv::Vec3f(val, val, val);
    }
  }

  cv::Mat imgOrigFloat, imgBlurFloat, resultFloat;
  imageOriginal.convertTo(imgOrigFloat, CV_32FC3);
  imageBlurred.convertTo(imgBlurFloat, CV_32FC3);

  // Blending: Result = Original * alpha + Blurred * (1 - alpha)
  cv::Mat alphaInverse = cv::Scalar(1.0, 1.0, 1.0) - alpha;
  resultFloat = imgOrigFloat.mul(alpha) + imgBlurFloat.mul(alphaInverse);

  resultFloat.convertTo(imageResult, CV_8UC3);
  cv::imshow("Efeito Tilt-Shift", imageResult);
}

void on_trackbar(int, void*) {
  processTiltShift();
}

int main(int argc, char** argv) {
  std::string filename = (argc > 1) ? argv[1] : "por-do-sol-ponta-negra.png";
  imageOriginal = cv::imread(filename);

  if (imageOriginal.empty()) {
    std::cerr << "Erro ao abrir a imagem: " << filename << std::endl;
    std::cout << "Uso: ./tiltshift <caminho_da_imagem>" << std::endl;
    return -1;
  }

  // Gera a versao borrada com filtro Gaussiano
  cv::GaussianBlur(imageOriginal, imageBlurred, cv::Size(21, 21), 0);

  cv::namedWindow("Efeito Tilt-Shift", cv::WINDOW_NORMAL);
  cv::resizeWindow("Efeito Tilt-Shift", 800, 600);

  // Criacao das 3 trackbars solicitadas
  cv::createTrackbar("Altura Foco (h)", "Efeito Tilt-Shift", &height_slider, height_slider_max, on_trackbar);
  cv::createTrackbar("Decaimento (delta)", "Efeito Tilt-Shift", &decay_slider, decay_slider_max, on_trackbar);
  cv::createTrackbar("Posicao Vert (y0)", "Efeito Tilt-Shift", &pos_slider, pos_slider_max, on_trackbar);

  // Processa a imagem inicial
  processTiltShift();

  std::cout << "\n=== CONTROLES TILT-SHIFT ===" << std::endl;
  std::cout << "Ajuste os sliders para modificar a regiao de foco." << std::endl;
  std::cout << "Pressione 's' para salvar a imagem final." << std::endl;
  std::cout << "Pressione 'ESC' para sair." << std::endl;

  while (true) {
    char k = (char)cv::waitKey(30);
    if (k == 27) break; // ESC
    if (k == 's' || k == 'S') {
      cv::imwrite("tiltshift_resultado.png", imageResult);
      std::cout << "Imagem salva com sucesso como 'tiltshift_resultado.png'!" << std::endl;
    }
  }

  // Salva automaticamente o resultado ao sair
  cv::imwrite("tiltshift_resultado.png", imageResult);
  return 0;
}