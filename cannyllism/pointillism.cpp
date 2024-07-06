#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <opencv2/opencv.hpp>
#include <random>
#include <vector>

#define STEP 5
#define JITTER 3
#define RADIUS 3

int main(int argc, char **argv)
{
  std::vector<int> yRange;
  std::vector<int> xRange;

  bool grayscale = false;
  if (argc > 2 && std::strcmp(argv[2], "grayscale") == 0)
  {
    grayscale = true;
  }

  cv::Mat image, frame, points;

  image = cv::imread(argv[1], grayscale ? cv::IMREAD_GRAYSCALE : cv::IMREAD_COLOR);
  if (image.empty())
  {
    std::cerr << "Could not open or find the image: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  int width = image.cols;
  int height = image.rows;

  xRange.resize(height / STEP);
  yRange.resize(width / STEP);

  std::iota(xRange.begin(), xRange.end(), 0);
  std::iota(yRange.begin(), yRange.end(), 0);

  for (auto &x : xRange)
  {
    x = x * STEP + STEP / 2;
  }

  for (auto &y : yRange)
  {
    y = y * STEP + STEP / 2;
  }

  points = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(xRange.begin(), xRange.end(), std::default_random_engine(seed));

  for (const auto &x : xRange)
  {
    std::shuffle(yRange.begin(), yRange.end(), std::default_random_engine(seed));
    for (const auto &y : yRange)
    {
      int jitteredX = x + std::rand() % (2 * JITTER) - JITTER + 1;
      int jitteredY = y + std::rand() % (2 * JITTER) - JITTER + 1;

      if (grayscale)
      {
        uchar grayValue = image.at<uchar>(jitteredX, jitteredY);
        cv::circle(points, cv::Point(jitteredY, jitteredX), RADIUS, cv::Scalar(grayValue, grayValue, grayValue), cv::FILLED, cv::LINE_AA);
      }
      else
      {
        cv::Vec3b color = image.at<cv::Vec3b>(jitteredX, jitteredY);
        cv::circle(points, cv::Point(jitteredY, jitteredX), RADIUS, cv::Scalar(color[0], color[1], color[2]), cv::FILLED, cv::LINE_AA);
      }
    }
  }

  std::string originalPath(argv[1]);
  std::size_t extensionPos = originalPath.find_last_of(".");
  std::string basePath = (extensionPos == std::string::npos) ? originalPath : originalPath.substr(0, extensionPos);

  cv::imwrite(basePath + "_pointillism_" + (grayscale ? "grayscale" : "colored") + ".jpg", points);
  return EXIT_SUCCESS;
}
