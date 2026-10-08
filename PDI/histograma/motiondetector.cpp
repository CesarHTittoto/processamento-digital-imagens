#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::VideoCapture cap;

  if (argc > 1) {
    cap.open(argv[1]);
  } else {
    cap.open(0);
  }

  if (!cap.isOpened()) {
    std::cerr << "Erro ao abrir a fonte de video/camera." << std::endl;
    return -1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

  cv::Mat frame, gray;
  cv::Mat histAtual, histAnterior;
  int nbins = 64;
  float range[] = {0, 256};
  const float* histrange = {range};

  // Limiar de sensibilidade para detecção de movimento
  // Com HISTCMP_CORREL (valores de 0.0 a 1.0): se for menor que ~0.98 indica mudança.
  // Com HISTCMP_CHISQR: quanto MAIOR o valor, maior a diferença.
  double threshold_chisqr = 50.0; // Limiar usando Chi-Quadrado

  bool firstFrame = true;

  while (true) {
    cap >> frame;
    if (frame.empty()) break;

    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Calcula histograma do frame atual
    cv::calcHist(&gray, 1, 0, cv::Mat(), histAtual, 1, &nbins, &histrange, true, false);
    cv::normalize(histAtual, histAtual, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    if (!firstFrame) {
      // Comparação de histogramas usando Chi-Quadrado (cv::HISTCMP_CHISQR)
      double diff = cv::compareHist(histAtual, histAnterior, cv::HISTCMP_CHISQR);

      // Sinaliza se ultrapassar o limiar estabelecido
      if (diff > threshold_chisqr) {
        cv::putText(frame, "ALARM: MOVIMENTO DETECTADO!", cv::Point(20, 50),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
        
        // Bordas vermelhas indicando alarme
        cv::rectangle(frame, cv::Point(0, 0), cv::Point(frame.cols - 1, frame.rows - 1),
                      cv::Scalar(0, 0, 255), 5);
      }

      std::cout << "Diferenca de Histograma (Chi-Sqr): " << diff << std::endl;
    } else {
      firstFrame = false;
    }

    // Guarda o histograma para comparar no proximo ciclo
    histAtual.clone().copyTo(histAnterior);

    cv::imshow("Detector de Movimento", frame);

    if (cv::waitKey(30) == 27) break; // ESC para sair
  }

  return 0;
}