#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat image, display_img, selection_mask, mask;
bool drawing = false;
int brush_type = 0; // 1: adicionar (botão esquerdo), -1: remover (botão direito)
int brush_size = 8;

cv::Scalar average(128, 128, 128);
cv::Scalar stdev(0, 0, 0);

int rangeR = 0;
int rangeG = 0;
int rangeB = 0;

void update_segmentation() {
  if (cv::countNonZero(selection_mask) > 0) {
    cv::meanStdDev(image, average, stdev, selection_mask);

    rangeB = cv::saturate_cast<int>(3 * stdev[0]);
    rangeG = cv::saturate_cast<int>(3 * stdev[1]);
    rangeR = cv::saturate_cast<int>(3 * stdev[2]);

    cv::setTrackbarPos("R-range", "Image", rangeR);
    cv::setTrackbarPos("G-range", "Image", rangeG);
    cv::setTrackbarPos("B-range", "Image", rangeB);
  }

  cv::Scalar lower_bound(
      std::max(0.0, average[0] - rangeB),
      std::max(0.0, average[1] - rangeG),
      std::max(0.0, average[2] - rangeR));

  cv::Scalar upper_bound(
      std::min(255.0, average[0] + rangeB),
      std::min(255.0, average[1] + rangeG),
      std::min(255.0, average[2] + rangeR));

  cv::inRange(image, lower_bound, upper_bound, mask);
}

void on_trackbar_R(int value, void*) { rangeR = value; update_segmentation(); }
void on_trackbar_G(int value, void*) { rangeG = value; update_segmentation(); }
void on_trackbar_B(int value, void*) { rangeB = value; update_segmentation(); }

static void onMouse(int event, int x, int y, int flags, void*) {
  if (event == cv::EVENT_LBUTTONDOWN) {
    drawing = true;
    brush_type = 1;
  } else if (event == cv::EVENT_RBUTTONDOWN) {
    drawing = true;
    brush_type = -1;
  } else if (event == cv::EVENT_LBUTTONUP || event == cv::EVENT_RBUTTONUP) {
    drawing = false;
    brush_type = 0;
  }

  if (drawing) {
    uchar color = (brush_type == 1) ? 255 : 0;
    cv::circle(selection_mask, cv::Point(x, y), brush_size, cv::Scalar(color), -1);
    update_segmentation();
  }
}

int main(int argc, const char** argv) {
  std::string filename = (argc > 1) ? argv[1] : "../PlayStation_Move.png";
  image = cv::imread(filename, cv::IMREAD_COLOR);

  if (image.empty()) {
    std::cerr << "Erro ao abrir a imagem: " << filename << std::endl;
    return -1;
  }

  selection_mask = cv::Mat::zeros(image.size(), CV_8UC1);
  mask = cv::Mat::zeros(image.size(), CV_8UC1);

  cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("Pintura da Selecao", cv::WINDOW_AUTOSIZE);
  cv::namedWindow("Segmentacao (inRange)", cv::WINDOW_AUTOSIZE);

  cv::createTrackbar("R-range", "Image", &rangeR, 255, on_trackbar_R);
  cv::createTrackbar("G-range", "Image", &rangeG, 255, on_trackbar_G);
  cv::createTrackbar("B-range", "Image", &rangeB, 255, on_trackbar_B);
  cv::setMouseCallback("Image", onMouse, nullptr);

  std::cout << "=== Controle de Pintura Interativa ===\n"
            << " - Arraste com o BOTAO ESQUERDO para PINTAR (adicionar pixels)\n"
            << " - Arraste com o BOTAO DIREITO para APAGAR (remover pixels)\n"
            << " - Pressione 'c' para LIMPAR a selecao\n"
            << " - Pressione ESC para SAIR\n";

  for (;;) {
    image.copyTo(display_img);
    
    // Destaca a região pintada com uma sobreposição semitransparente em azul/ciano
    cv::Mat overlay;
    image.copyTo(overlay);
    overlay.setTo(cv::Scalar(255, 128, 0), selection_mask);
    cv::addWeighted(overlay, 0.5, display_img, 0.5, 0, display_img);

    cv::imshow("Image", display_img);
    cv::imshow("Pintura da Selecao", selection_mask);
    cv::imshow("Segmentacao (inRange)", mask);

    char c = (char)cv::waitKey(20);
    if (c == 27) break; // ESC
    if (c == 'c' || c == 'C') {
      selection_mask = cv::Mat::zeros(image.size(), CV_8UC1);
      average = cv::Scalar(128, 128, 128);
      stdev = cv::Scalar(0, 0, 0);
      rangeR = rangeG = rangeB = 0;
      cv::setTrackbarPos("R-range", "Image", 0);
      cv::setTrackbarPos("G-range", "Image", 0);
      cv::setTrackbarPos("B-range", "Image", 0);
      mask = cv::Mat::zeros(image.size(), CV_8UC1);
    }
  }

  cv::imwrite("PlayStation_Move_pintura_selecao.png", selection_mask);
  cv::imwrite("PlayStation_Move_segmentada.png", mask);

  return 0;
}