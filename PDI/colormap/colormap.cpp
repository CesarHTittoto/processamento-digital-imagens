#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  // Carrega a imagem da pasta anterior
  cv::Mat gray = cv::imread("../biel.png", cv::IMREAD_GRAYSCALE);

  if (gray.empty()) {
    std::cout << "Erro ao carregar a imagem ../biel.png!" << std::endl;
    return -1;
  }

  // 1. Aplicação dos 5 Colormaps Pré-definidos do OpenCV
  cv::Mat cm_jet, cm_hot, cm_ocean, cm_summer, cm_viridis;

  cv::applyColorMap(gray, cm_jet, cv::COLORMAP_JET);
  cv::applyColorMap(gray, cm_hot, cv::COLORMAP_HOT);
  cv::applyColorMap(gray, cm_ocean, cv::COLORMAP_OCEAN);
  cv::applyColorMap(gray, cm_summer, cv::COLORMAP_SUMMER);
  cv::applyColorMap(gray, cm_viridis, cv::COLORMAP_VIRIDIS);

  // Salva as imagens geradas
  cv::imwrite("biel_jet.png", cm_jet);
  cv::imwrite("biel_hot.png", cm_hot);
  cv::imwrite("biel_ocean.png", cm_ocean);
  cv::imwrite("biel_summer.png", cm_summer);
  cv::imwrite("biel_viridis.png", cm_viridis);

  // 2. Criação do Colormap Personalizado via Look-Up Table (LUT)
  // Tabela de tamanho 256x1 do tipo CV_8UC3 (BGR)
  cv::Mat custom_lut(256, 1, CV_8UC3);

  for (int i = 0; i < 256; i++) {
    uchar b = cv::saturate_cast<uchar>(i< 128 ? i * 2 : 255 + (i - 128) * 2);   // Azul 
    uchar g = cv::saturate_cast<uchar>(i< 128 ? i * 2 : 50 + (i - 128) * 2);   // Verde 
    uchar r = cv::saturate_cast<uchar>(i);   // Vermelho 

    custom_lut.at<cv::Vec3b>(i, 0) = cv::Vec3b(b, g, r);
  }

  // Aplica a LUT personalizada
  cv::Mat cm_custom;
  cv::applyColorMap(gray, cm_custom, custom_lut);
  cv::imwrite("biel_custom.png", cm_custom);

  std::cout << "Sucesso! Todas as imagens foram geradas com sucesso." << std::endl;

  return 0;
}