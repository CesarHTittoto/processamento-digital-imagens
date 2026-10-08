#include <iostream>
#include <cmath>
#include <string>
#include <opencv2/opencv.hpp>

// Funcao auxiliar para calcular e aplicar a mascara de tilt-shift sobre um frame
cv::Mat applyTiltShift(const cv::Mat &frameOrig, double h_ratio, double delta_val, double y0_ratio) {
  int rows = frameOrig.rows;
  int cols = frameOrig.cols;

  // Converte proporcoes (0.0 a 1.0) para dimensoes reais em pixels
  double h = h_ratio * rows;
  double y0 = y0_ratio * rows;
  double delta = delta_val;

  // Imagem borrada via filtro Gaussiano
  cv::Mat frameBlurred;
  cv::GaussianBlur(frameOrig, frameBlurred, cv::Size(21, 21), 0);

  // Construcao da mascara sigmoide alfa
  cv::Mat alpha = cv::Mat::zeros(rows, cols, CV_32FC3);
  for (int r = 0; r < rows; r++) {
    double val = 0.5 * (std::tanh((r - y0 + h / 2.0) / delta) - std::tanh((r - y0 - h / 2.0) / delta));
    for (int c = 0; c < cols; c++) {
      alpha.at<cv::Vec3f>(r, c) = cv::Vec3f(val, val, val);
    }
  }

  cv::Mat origFloat, blurFloat, resultFloat, resultByte;
  frameOrig.convertTo(origFloat, CV_32FC3);
  frameBlurred.convertTo(blurFloat, CV_32FC3);

  // Blending: Result = Original * alpha + Blurred * (1 - alpha)
  cv::Mat alphaInverse = cv::Scalar(1.0, 1.0, 1.0) - alpha;
  resultFloat = origFloat.mul(alpha) + blurFloat.mul(alphaInverse);

  resultFloat.convertTo(resultByte, CV_8UC3);
  return resultByte;
}

int main(int argc, char** argv) {
  std::string inputVideo = (argc > 1) ? argv[1] : "Natal.mp4";
  std::string outputVideo = "tiltshift_stopmotion.mp4";

  // Parametros do Tilt-Shift (podem ser ajustados conforme a cena)
  double h_ratio = 0.40;    // Altura da faixa em foco (40% da altura da imagem)
  double delta_val = 15.0;  // Forca de decaimento da transicao
  double y0_ratio = 0.50;   // Posicao vertical do foco (centro da imagem)

  // Fator de Stop Motion: processa 1 a cada N quadros (ex: 3 descarta 2 de cada 3 frames)
  int frameSkip = 4; 

  cv::VideoCapture cap(inputVideo);
  if (!cap.isOpened()) {
    std::cerr << "Erro ao abrir o video de entrada: " << inputVideo << std::endl;
    std::cout << "Uso: ./tiltshiftvideo <arquivo_de_video.mp4>" << std::endl;
    return -1;
  }

  int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
  int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
  double origFps = cap.get(cv::CAP_PROP_FPS);
  int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));

  if (origFps <= 0) origFps = 30.0;

  // Configura o VideoWriter (utilizando codec mp4v)
  int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
  cv::VideoWriter writer(outputVideo, fourcc, origFps, cv::Size(width, height));

  if (!writer.isOpened()) {
    std::cerr << "Erro ao inicializar o VideoWriter para: " << outputVideo << std::endl;
    return -1;
  }

  std::cout << "=== PROCESSANDO VÍDEO TILT-SHIFT + STOP MOTION ===" << std::endl;
  std::cout << "Arquivo de Entrada: " << inputVideo << " (" << width << "x" << height << " @ " << origFps << " FPS)" << std::endl;
  std::cout << "Total de Quadros: " << totalFrames << std::endl;
  std::cout << "Fator de Stop Motion (Skip): 1 a cada " << frameSkip << " frames" << std::endl;
  std::cout << "Arquivo de Saida: " << outputVideo << std::endl;
  std::cout << "--------------------------------------------------" << std::endl;

  cv::Mat frame, processedFrame;
  int frameCount = 0;
  int writtenFrames = 0;

  while (true) {
    cap >> frame;
    if (frame.empty()) break;

    // Amostragem de quadros para o efeito Stop Motion
    if (frameCount % frameSkip == 0) {
      processedFrame = applyTiltShift(frame, h_ratio, delta_val, y0_ratio);

      // Grava o quadro processado repetindo-o para manter a taxa de FPS original e o ritmo de stop motion
      for (int i = 0; i < frameSkip; i++) {
        writer.write(processedFrame);
        writtenFrames++;
      }

      cv::imshow("Processando Tilt-Shift (Stop Motion)", processedFrame);
      if (cv::waitKey(1) == 27) break; // ESC encerra
    }

    frameCount++;
    if (frameCount % 30 == 0 || frameCount == totalFrames) {
      std::cout << "Progresso: " << frameCount << " / " << totalFrames << " frames analisados." << std::endl;
    }
  }

  cap.release();
  writer.release();
  cv::destroyAllWindows();

  std::cout << "\nProcessamento concluido com sucesso!" << std::endl;
  std::cout << "Video final salvo em: " << outputVideo << std::endl;

  return 0;
}