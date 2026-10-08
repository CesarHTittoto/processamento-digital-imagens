#include <iostream>
#include <opencv2/opencv.hpp>

void drawHistogram(const cv::Mat& hist, cv::Mat& histImg, cv::Scalar color) {
  histImg.setTo(cv::Scalar(0, 0, 0));
  int nbins = hist.rows;
  int histh = histImg.rows;

  cv::Mat histNorm;
  cv::normalize(hist, histNorm, 0, histh, cv::NORM_MINMAX, -1, cv::Mat());

  for (int i = 0; i < nbins; i++) {
    cv::line(histImg,
             cv::Point(i, histh),
             cv::Point(i, histh - cvRound(histNorm.at<float>(i))),
             color, 1, 8, 0);
  }
}

int main(int argc, char** argv) {
  cv::VideoCapture cap;

  // Se passado parâmetro abre vídeo, senão abre a webcam (índice 0)
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

  cv::Mat frame, gray, equalized;
  cv::Mat histOriginal, histEqualized;
  int nbins = 64;
  float range[] = {0, 256};
  const float* histrange = {range};

  int histw = nbins;
  int histh = nbins / 2;
  cv::Mat histImgOrig(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Mat histImgEq(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));

  while (true) {
    cap >> frame;
    if (frame.empty()) break;

    // Converte para tons de cinza
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // Equalização de Histograma
    cv::equalizeHist(gray, equalized);

    // Cálculo dos histogramas
    cv::calcHist(&gray, 1, 0, cv::Mat(), histOriginal, 1, &nbins, &histrange, true, false);
    cv::calcHist(&equalized, 1, 0, cv::Mat(), histEqualized, 1, &nbins, &histrange, true, false);

    // Desenho dos histogramas
    drawHistogram(histOriginal, histImgOrig, cv::Scalar(0, 255, 255));
    drawHistogram(histEqualized, histImgEq, cv::Scalar(0, 255, 0));

    // Converte cinza para BGR apenas para concatenar/exibir os histogramas sobrepostos
    cv::Mat displayOrig, displayEq;
    cv::cvtColor(gray, displayOrig, cv::COLOR_GRAY2BGR);
    cv::cvtColor(equalized, displayEq, cv::COLOR_GRAY2BGR);

    histImgOrig.copyTo(displayOrig(cv::Rect(0, 0, histw, histh)));
    histImgEq.copyTo(displayEq(cv::Rect(0, 0, histw, histh)));

    cv::imshow("Original (Cinza)", displayOrig);
    cv::imshow("Equalizada", displayEq);

    if (cv::waitKey(30) == 27) break; // ESC para sair
  }

  return 0;
}