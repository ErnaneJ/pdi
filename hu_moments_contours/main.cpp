#include <iostream>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
    return -1;
  }

  cv::Mat image, gray;
  std::ofstream file;
  cv::Moments momentos;

  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data)
  {
    std::cout << "Could not open or find the image: " << argv[1] << std::endl;
    return -1;
  }

  file.open("../assets/moments.txt");
  if (!file.is_open())
  {
    std::cerr << "Failed to open moments.txt for writing!" << std::endl;
    return -1;
  }

  cv::threshold(image, image, 1, 255, cv::THRESH_BINARY + cv::THRESH_OTSU + cv::THRESH_BINARY_INV);

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

  cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);

  int nformas = 0;

  for (size_t i = 0; i < contours.size(); i++)
  {
    if (contours[i].size() < 10)
    {
      continue;
    }
    nformas++;
    momentos = cv::moments(contours[i]);

    cv::Point2f center(momentos.m10 / momentos.m00, momentos.m01 / momentos.m00);

    double hu[7];
    cv::HuMoments(momentos, hu);

    if (hu[0] > 0)
    {
      cv::drawContours(image, contours, i, cv::Scalar(0, 0, 255), 2);
    }
    else
    {
      cv::drawContours(image, contours, i, cv::Scalar(0, 255, 0), 2);
    }

    cv::putText(image, std::to_string(i), center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 8);
    cv::putText(image, std::to_string(i), center, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

    file << i << ", ";
    for (int j = 0; j < 7; j++)
    {
      hu[j] = -1 * std::copysign(1.0, hu[j]) * std::log10(std::abs(hu[j]));
      file << hu[j] << ", ";
    }
    file << std::endl;
  }

  std::cout << "Count of objects: " << nformas << std::endl;
  file.close();

  cv::imshow("Contours Labeled", image);
  cv::imwrite("../assets/contours-labeled.png", image);
  cv::waitKey();
  return 0;
}
