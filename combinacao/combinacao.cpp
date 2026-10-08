#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  // Define o caminho do vídeo padrão ou recebe via argumento do terminal
  std::string video_path = (argc > 1) ? argv[1] : "teste_light_painting.mp4";

  cv::VideoCapture cap(video_path);
  if (!cap.isOpened()) {
    std::cerr << "Erro ao abrir o arquivo de video: " << video_path << std::endl;
    return -1;
  }

  cv::Mat frame, frame_gray, frame_resized;
  cap >> frame;
  if (frame.empty()) {
    std::cerr << "O video nao contem frames validos." << std::endl;
    return -1;
  }

  // --- REDIMENSIONAMENTO PARA 640x640 ---
  cv::resize(frame, frame_resized, cv::Size(640, 640));

  // Converte o primeiro frame redimensionado para tons de cinza
  cv::cvtColor(frame_resized, frame_gray, cv::COLOR_BGR2GRAY);

  // Inicializa os acumuladores em 640x640
  cv::Mat light_painting_max = frame_gray.clone();
  cv::Mat light_painting_or  = frame_gray.clone();

  while (true) {
    cap >> frame;
    if (frame.empty()) {
      break; // Fim do vídeo
    }

    // Redimensiona para o formato quadrado 640x640
    cv::resize(frame, frame_resized, cv::Size(640, 640));
    cv::cvtColor(frame_resized, frame_gray, cv::COLOR_BGR2GRAY);

    // Combinações acumulativas
    cv::max(light_painting_max, frame_gray, light_painting_max);
    cv::bitwise_or(light_painting_or, frame_gray, light_painting_or);
  }

  // Exibe as janelas com o resultado quadrado
  cv::imshow("Light Painting - cv::max", light_painting_max);
  cv::imshow("Light Painting - cv::bitwise_or", light_painting_or);

  // Salva os ficheiros gerados em 640x640
  cv::imwrite("resultado_light_painting_max.png", light_painting_max);
  cv::imwrite("resultado_light_painting_or.png", light_painting_or);

  std::cout << "Processamento concluido! Imagens salvas na resolucao 640x640." << std::endl;

  cv::waitKey(0);
  return 0;
}