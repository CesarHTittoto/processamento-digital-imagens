#include <iostream>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>

int main() {
  int width = 256;
  int height = 256;
  int periodos = 4;
  float amplitude = 127.0f;
  float offset = 127.0f;

  cv::Mat image(height, width, CV_32FC1);

  // Geração da senoide horizontal
  for (int j = 0; j < width; j++) {
    float val = offset + amplitude * std::sin(2.0f * M_PI * periodos * j / width);
    for (int i = 0; i < height; i++) {
      image.at<float>(i, j) = val;
    }
  }

  // 1. Gravação em formato YML (Ponto Flutuante)
  cv::FileStorage fs("senoide.yml", cv::FileStorage::WRITE);
  fs << "mat" << image;
  fs.release();

  // 2. Normalização e Gravação em formato PNG (8 bits inteiros)
  cv::Mat image8u;
  image.convertTo(image8u, CV_8U);
  cv::imwrite("senoide.png", image8u);

  // --- Leitura e Comparação ---
  cv::Mat img_yml;
  cv::FileStorage fs_read("senoide.yml", cv::FileStorage::READ);
  fs_read["mat"] >> img_yml;
  fs_read.release();

  cv::Mat img_png = cv::imread("senoide.png", cv::IMREAD_GRAYSCALE);

  int mid_row = height / 2;
  std::ofstream fdiff("diferenca_linha.txt");

  for (int j = 0; j < width; j++) {
    float val_yml = img_yml.at<float>(mid_row, j);
    float val_png = static_cast<float>(img_png.at<uchar>(mid_row, j));
    float diff = std::abs(val_yml - val_png);

    fdiff << j << " " << val_yml << " " << val_png << " " << diff << "\n";
  }

  fdiff.close();
  std::cout << "Processamento concluído. Arquivo diferenca_linha.txt gerado com sucesso!" << std::endl;

  return 0;
}