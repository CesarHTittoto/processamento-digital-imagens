#include <iostream>
#include <opencv2/opencv.hpp>

void printMenu() {
  std::cout << "\n=== MENU FILTRO DA MEDIA ===" << std::endl;
  std::cout << "Teclas disponiveis para alterar a mascara:" << std::endl;
  std::cout << "1 - Mascara da Media 3x3" << std::endl;
  std::cout << "2 - Mascara da Media 11x11" << std::endl;
  std::cout << "3 - Mascara da Media 21x21" << std::endl;
  std::cout << "ESC - Sair" << std::endl;
}

int main(int argc, char** argv) {
  cv::VideoCapture cap;

  if (argc > 1) {
    cap.open(argv[1]);
  } else {
    cap.open(0, cv::CAP_DSHOW);
  }

  if (!cap.isOpened()) {
    std::cerr << "Erro ao abrir a camera ou ficheiro de video." << std::endl;
    return -1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

  cv::Mat frame, gray, result;
  int maskSize = 3;

  printMenu();

  while (true) {
    cap >> frame;
    if (frame.empty()) break;

    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::Mat mask = cv::Mat::ones(maskSize, maskSize, CV_32F) / (float)(maskSize * maskSize);

    cv::filter2D(gray, result, CV_32F, mask);
    result.convertTo(result, CV_8U);

    std::string text = "Filtro da Media: " + std::to_string(maskSize) + "x" + std::to_string(maskSize);
    cv::putText(result, text, cv::Point(20, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255), 2);

    cv::imshow("Original (Cinza)", gray);
    cv::imshow("Filtrada (Convolucao)", result);

    char key = (char)cv::waitKey(30);
    if (key == 27) break;

    switch (key) {
      case '1':
        maskSize = 3;
        std::cout << "Mascara alterada para 3x3" << std::endl;
        break;
      case '2':
        maskSize = 11;
        std::cout << "Mascara alterada para 11x11" << std::endl;
        break;
      case '3':
        maskSize = 21;
        std::cout << "Mascara alterada para 21x21" << std::endl;
        break;
      default:
        break;
    }
  }

  return 0;
}