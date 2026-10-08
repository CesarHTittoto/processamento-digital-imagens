#define _REENTRANT
#include <mutex>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Uso: ./videocinza <caminho_do_video>" << std::endl;
    return -1;
  }

  cv::VideoCapture cap(argv[1]);
  if (!cap.isOpened()) {
    std::cout << "Erro ao abrir o vídeo!" << std::endl;
    return -1;
  }

  double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  double fps = cap.get(cv::CAP_PROP_FPS);

  // Garantia de FPS válido
  if (fps <= 0) fps = 30.0;

  std::cout << "Largura: " << width << "\n";
  std::cout << "Altura : " << height << "\n";
  std::cout << "FPS    : " << fps << "\n";

  cv::Size frameSize(static_cast<int>(width), static_cast<int>(height));

  // isColor = false é obrigatório para 1 canal (escala de cinza)
  int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
  cv::VideoWriter out("output_cinza.mp4", fourcc, fps, frameSize, false);

  if (!out.isOpened()) {
    std::cout << "Erro ao criar o arquivo de saída de vídeo!" << std::endl;
    return -1;
  }

  cv::namedWindow("Vídeo em Tons de Cinza", cv::WINDOW_AUTOSIZE);

  cv::Mat frame, frame_gray;
  int counter = 0;

  while (cap.read(frame)) {
    if (frame.empty()) break;

    // Converte para escala de cinza
    cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

    // Escreve no arquivo de saída
    out << frame_gray;

    // Exibe a janela com o vídeo
    cv::imshow("Vídeo em Tons de Cinza", frame_gray);
    counter++;

    // Sai apenas se a tecla ESC (ASCII 27) for pressionada
    char key = static_cast<char>(cv::waitKey(30));
    if (key == 27) {
      std::cout << "Execução interrompida pelo usuário." << std::endl;
      break;
    }
  }

  std::cout << "Número de frames processados com sucesso: " << counter << "\n";
  
  // Liberação dos recursos de vídeo
  cap.release();
  out.release();
  cv::destroyAllWindows();

  return 0;
}