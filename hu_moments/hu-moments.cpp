#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
  double huMoments[7];

  // Load the image in grayscale mode
  cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    std::cerr << "Failed to open " << argv[1] << std::endl;
    return -1;
  }

  // Calculate moments
  cv::Moments moments = cv::moments(image, false);
  
  // Calculate Hu moments
  cv::HuMoments(moments, huMoments);

  // Print Hu moments
  for (int i = 0; i < 7; ++i) {
    std::cout << huMoments[i] << ", ";
  }
  std::cout << std::endl;

  // Apply logarithmic transformation
  for (int i = 0; i < 7; ++i) {
    huMoments[i] = -1 * std::copysign(1.0, huMoments[i]) * std::log10(std::abs(huMoments[i]));
  }

  // Print transformed Hu moments
  for (int i = 0; i < 7; ++i) {
    std::cout << huMoments[i] << ", ";
  }
  std::cout << std::endl;

  return 0;
}