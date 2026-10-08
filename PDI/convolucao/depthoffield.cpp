#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  std::string videoPath = (argc > 1) ? argv[1] : "vasos720p.mp4";
  cv::VideoCapture cap(videoPath);

  if (!cap.isOpened()) {
    std::cerr << "Erro ao abrir o video: " << videoPath << std::endl;
    return -1;
  }

  cv::Mat frame, gray, laplacian, laplacianAbs, laplacianDisplay;
  cv::Mat maxLaplacian;  // Matriz de float para guardar a amplitude exata do Laplaciano
  cv::Mat focusImage;    // Imagem de saida (foco estendido em cores)

  // Mascara Laplaciana 3x3 com valor central 8 (detecta bordas com maior ganho)
  float laplacianMask[3][3] = {
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
  };
  cv::Mat mask = cv::Mat(3, 3, CV_32F, laplacianMask);

  bool initialized = false;
  int frameCount = 0;

  std::cout << "Processando frames do video..." << std::endl;

  while (true) {
    // 1. Capture um frame da cena do video
    cap >> frame;
    if (frame.empty()) break;

    frameCount++;

    // 2. Converta o frame para tons de cinza
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    // 4. Aplique um filtro laplaciano de tamanho 3x3 na imagem cinzenta
    cv::filter2D(gray, laplacian, CV_32F, mask);
    
    // Calcula o modulo da resposta para avaliar a nitidez/borda
    laplacianAbs = cv::abs(laplacian);

    // 3. Crie uma matriz para guardar os maximos dos laplacianos (no 1º frame)
    if (!initialized) {
      maxLaplacian = laplacianAbs.clone();
      focusImage = frame.clone();
      initialized = true;
      continue;
    }

    // 5. Compare a resposta laplaciana com a matriz de maximos e atualize
    for (int r = 0; r < frame.rows; r++) {
      for (int c = 0; c < frame.cols; c++) {
        float currentVal = laplacianAbs.at<float>(r, c);
        float maxVal = maxLaplacian.at<float>(r, c);

        if (currentVal > maxVal) {
          maxLaplacian.at<float>(r, c) = currentVal;
          focusImage.at<cv::Vec3b>(r, c) = frame.at<cv::Vec3b>(r, c);
        }
      }
    }

    // Normaliza o Laplaciano do frame atual apenas para exibicao visual (0 a 255)
    cv::normalize(laplacianAbs, laplacianDisplay, 0, 255, cv::NORM_MINMAX, CV_8U);

    // 6. Exiba a imagem de saida e as janelas intermediarias
    cv::imshow("Frame Original do Video", frame);
    cv::imshow("Imagem em Tons de Cinza", gray);
    cv::imshow("Resposta Laplaciana (Frame Atual)", laplacianDisplay);
    cv::imshow("Foco Estendido (Saida Final Colorida)", focusImage);

    if (cv::waitKey(1) == 27) break; // ESC para cancelar
  }

  std::cout << "Processamento concluido (" << frameCount << " frames analisados)." << std::endl;

  // Normaliza a matriz acumulada dos maximos laplacianos para salvar em formato de imagem (CV_8U)
  cv::Mat maxLaplacianDisplay;
  cv::normalize(maxLaplacian, maxLaplacianDisplay, 0, 255, cv::NORM_MINMAX, CV_8U);

  // Salva automaticamente ambas as imagens de resultado
  cv::imwrite("resultado_foco_estendido.png", focusImage);
  cv::imwrite("resultado_laplaciano_maximo.png", maxLaplacianDisplay);

  std::cout << "Imagens salvas automaticamente:" << std::endl;
  std::cout << " - resultado_foco_estendido.png" << std::endl;
  std::cout << " - resultado_laplaciano_maximo.png" << std::endl;

  // Exibe o mapa de maximos laplacianos no final
  cv::imshow("Mapa Acumulado de Laplacianos Maximos", maxLaplacianDisplay);
  cv::imshow("Foco Estendido (Saida Final Colorida)", focusImage);
  cv::waitKey(0);

  return 0;
}