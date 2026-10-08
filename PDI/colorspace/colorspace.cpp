#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char** argv) {
  std::string filename = (argc > 1) ? argv[1] : "../frutas.png";

  // Carrega a imagem colorida (BGR)
  cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);

  if (image.empty()) {
    std::cerr << "Erro ao abrir a imagem: " << filename << std::endl;
    return -1;
  }

  // 1. Exibição da imagem original e decomposição em canais (BGR e HSV)
  std::vector<cv::Mat> planes;
  cv::Mat rgb_planes, hsv_planes, hsv_image;

  // Decomposição BGR
  cv::split(image, planes); // planes[0]=B, planes[1]=G, planes[2]=R
  cv::hconcat(planes[0], planes[1], rgb_planes);
  cv::hconcat(rgb_planes, planes[2], rgb_planes);
  cv::imwrite("frutas_bgr_planes.png", rgb_planes);

  // Conversão para HSV_FULL e decomposição
  cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV_FULL);
  cv::split(hsv_image, planes); // planes[0]=H, planes[1]=S, planes[2]=V
  cv::hconcat(planes[0], planes[1], hsv_planes);
  cv::hconcat(hsv_planes, planes[2], hsv_planes);
  cv::imwrite("frutas_hsv_planes.png", hsv_planes);

  // 2. Preparação de imagem em escala de cinza para aplicação de Colormaps
  cv::Mat gray;
  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

  // 3. Aplicação de 5 Colormaps Pré-definidos do OpenCV
  cv::Mat cm_autumn, cm_bone, cm_cool, cm_spring, cm_plasma;

  cv::applyColorMap(gray, cm_autumn, cv::COLORMAP_AUTUMN);
  cv::applyColorMap(gray, cm_bone, cv::COLORMAP_BONE);
  cv::applyColorMap(gray, cm_cool, cv::COLORMAP_COOL);
  cv::applyColorMap(gray, cm_spring, cv::COLORMAP_SPRING);
  cv::applyColorMap(gray, cm_plasma, cv::COLORMAP_PLASMA);

  cv::imwrite("frutas_autumn.png", cm_autumn);
  cv::imwrite("frutas_bone.png", cm_bone);
  cv::imwrite("frutas_cool.png", cm_cool);
  cv::imwrite("frutas_spring.png", cm_spring);
  cv::imwrite("frutas_plasma.png", cm_plasma);

  // 4. Criação de Colormap Personalizado via Look-Up Table (LUT)
  // Tabela de dimensão 256x1 do tipo CV_8UC3 (Canais BGR)
  cv::Mat custom_lut(256, 1, CV_8UC3);

  for (int i = 0; i < 256; i++) {
    // Mapeamento BGR customizado: gradiente de azul (tons escuros) -> verde (médios) -> vermelho (claros)
    uchar b = cv::saturate_cast<uchar>(255 - i);
    uchar g = cv::saturate_cast<uchar>(i < 128 ? i * 2 : 255 - (i - 128) * 2);
    uchar r = cv::saturate_cast<uchar>(i);

    custom_lut.at<cv::Vec3b>(i, 0) = cv::Vec3b(b, g, r);
  }

  cv::Mat cm_custom;
  cv::applyColorMap(gray, cm_custom, custom_lut);
  cv::imwrite("frutas_custom.png", cm_custom);

  std::cout << "Sucesso! Imagens salvas no diretório de execução." << std::endl;

  // Exibição interativa
  cv::namedWindow("Imagem Original", cv::WINDOW_NORMAL);
  cv::imshow("Imagem Original", image);

  cv::namedWindow("Canais BGR (B | G | R)", cv::WINDOW_NORMAL);
  cv::imshow("Canais BGR (B | G | R)", rgb_planes);

  cv::namedWindow("Canais HSV (H | S | V)", cv::WINDOW_NORMAL);
  cv::imshow("Canais HSV (H | S | V)", hsv_planes);

  cv::namedWindow("Colormap Customizado (LUT)", cv::WINDOW_NORMAL);
  cv::imshow("Colormap Customizado (LUT)", cm_custom);

  cv::waitKey(0);
  return 0;
}