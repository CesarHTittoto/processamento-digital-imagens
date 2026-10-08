#include <iostream>
#include <opencv2/opencv.hpp>

void printmask(const cv::Mat &m) {
  for (int i = 0; i < m.rows; i++) {
    for (int j = 0; j < m.cols; j++) {
      std::cout << m.at<float>(i, j) << (j == m.cols - 1 ? "" : ", ");
    }
    std::cout << std::endl;
  }
  std::cout << "------------------" << std::endl;
}

int main(int argc, char** argv) {
  cv::VideoCapture cap;

  if (argc > 1) {
    cap.open(argv[1]);
  } else {
    cap.open(0, cv::CAP_DSHOW); // Abre a webcam padrao
  }

  if (!cap.isOpened()) {
    std::cerr << "Erro ao abrir a camera ou o arquivo de video." << std::endl;
    return -1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

  // Definicao dos coeficientes das mascaras 3x3
  float media[]      = {0.1111f, 0.1111f, 0.1111f, 
                        0.1111f, 0.1111f, 0.1111f, 
                        0.1111f, 0.1111f, 0.1111f};

  float gauss[]      = {0.0625f, 0.1250f, 0.0625f, 
                        0.1250f, 0.2500f, 0.1250f, 
                        0.0625f, 0.1250f, 0.0625f};

  float horizontal[] = {-1.0f, 0.0f, 1.0f, 
                        -2.0f, 0.0f, 2.0f, 
                        -1.0f, 0.0f, 1.0f};

  float vertical[]   = {-1.0f, -2.0f, -1.0f, 
                         0.0f,  0.0f,  0.0f, 
                         1.0f,  2.0f,  1.0f};

  float laplacian[]  = { 0.0f, -1.0f,  0.0f, 
                        -1.0f,  4.0f, -1.0f, 
                         0.0f, -1.0f,  0.0f};

  float boost[]      = { 0.0f, -1.0f,  0.0f, 
                        -1.0f,  5.2f, -1.0f, 
                         0.0f, -1.0f,  0.0f};

  cv::Mat frame, framegray, frame32f, frameFiltered, result;
  
  // Mascara inicial: Filtro da Media
  cv::Mat mask = cv::Mat(3, 3, CV_32F, media).clone();
  std::string currentEffect = "Filtro da Media (m)";

  int absolut = 1; // 1 = LIGADO, 0 = DESLIGADO

  std::cout << "=== TECLAS DE ATALHO PARA EFEITOS ===" << std::endl;
  std::cout << "a - Alternar Valor Absoluto (ON/OFF)" << std::endl;
  std::cout << "m - Filtro da Media" << std::endl;
  std::cout << "g - Filtro Gaussiano" << std::endl;
  std::cout << "h - Detector de bordas horizontais" << std::endl;
  std::cout << "v - Detector de bordas verticais" << std::endl;
  std::cout << "l - Filtro Laplaciano" << std::endl;
  std::cout << "b - Filtro de enfase (Boost)" << std::endl;
  std::cout << "ESC - Sair" << std::endl;
  std::cout << "=====================================" << std::endl;

  cv::namedWindow("original", cv::WINDOW_NORMAL);
  cv::namedWindow("filtroespacial", cv::WINDOW_NORMAL);

  while (true) {
    cap >> frame;
    if (frame.empty()) break;

    // Converte para escala de cinza e aplica espelhamento horizontal (efeito espelho)
    cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);
    cv::flip(framegray, framegray, 1);
    cv::imshow("original", framegray);

    // Converte a imagem para float de 32 bits para evitar overflow na convolucao
    framegray.convertTo(frame32f, CV_32F);

    // Aplica o filtro espacial no dominio do espaco via convolucao
    cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

    // Se a opcao 'absolut' estiver ativa, calcula o valor absoluto dos pixels
    if (absolut) {
      frameFiltered = cv::abs(frameFiltered);
    }

    // Converte de volta para imagem de 8 bits (0 a 255)
    frameFiltered.convertTo(result, CV_8U);

    // Adiciona informacao visual na tela sobre o efeito atual
    std::string textStatus = currentEffect + " | Absoluto: " + (absolut ? "ON" : "OFF");
    cv::putText(result, textStatus, cv::Point(15, 30), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(500), 2);

    cv::imshow("filtroespacial", result);

    char key = (char)cv::waitKey(10);
    if (key == 27) break; // ESC encerra o programa

    switch (key) {
      case 'a':
      case 'A':
        absolut = !absolut;
        std::cout << "Valor Absoluto " << (absolut ? "ATIVADO" : "DESATIVADO") << std::endl;
        break;

      case 'm':
      case 'M':
        mask = cv::Mat(3, 3, CV_32F, media).clone();
        currentEffect = "Filtro da Media (m)";
        std::cout << "\nMascara Ativa: " << currentEffect << std::endl;
        printmask(mask);
        break;

      case 'g':
      case 'G':
        mask = cv::Mat(3, 3, CV_32F, gauss).clone();
        currentEffect = "Filtro Gaussiano (g)";
        std::cout << "\nMascara Ativa: " << currentEffect << std::endl;
        printmask(mask);
        break;

      case 'h':
      case 'H':
        mask = cv::Mat(3, 3, CV_32F, horizontal).clone();
        currentEffect = "Bordas Horizontais (h)";
        std::cout << "\nMascara Ativa: " << currentEffect << std::endl;
        printmask(mask);
        break;

      case 'v':
      case 'V':
        mask = cv::Mat(3, 3, CV_32F, vertical).clone();
        currentEffect = "Bordas Verticais (v)";
        std::cout << "\nMascara Ativa: " << currentEffect << std::endl;
        printmask(mask);
        break;

      case 'l':
      case 'L':
        mask = cv::Mat(3, 3, CV_32F, laplacian).clone();
        currentEffect = "Filtro Laplaciano (l)";
        std::cout << "\nMascara Ativa: " << currentEffect << std::endl;
        printmask(mask);
        break;

      case 'b':
      case 'B':
        mask = cv::Mat(3, 3, CV_32F, boost).clone();
        currentEffect = "Filtro Boost (b)";
        std::cout << "\nMascara Ativa: " << currentEffect << std::endl;
        printmask(mask);
        break;

      default:
        break;
    }
  }

  return 0;
}