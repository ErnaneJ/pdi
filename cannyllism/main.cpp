#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

std::vector<int> yRange;
std::vector<int> xRange;

cv::Mat image, border, points;

char TrackbarName[50];

int width, height, r, g, b, x, y;
int stepSlider   = 5, stepSliderMax   = 20,
    jitterSlider = 3, jitterSliderMax = 10,
    radiusSlider = 3, radiusSliderMax = 10,
    topSlider    = 10, topSliderMax   = 200;

void onTrackbarCanny(int, void *)
{
  cv::Canny(image, border, topSlider, 3 * topSlider);

  xRange.resize(height / stepSlider);
  yRange.resize(width / stepSlider);

  std::iota(xRange.begin(), xRange.end(), 0);
  std::iota(yRange.begin(), yRange.end(), 0);

  for (uint i = 0; i < xRange.size(); ++i)
  {
    xRange[i] = xRange[i] * stepSlider + stepSlider / 2;
  }

  for (uint i = 0; i < yRange.size(); ++i)
  {
    yRange[i] = yRange[i] * stepSlider + stepSlider / 2;
  }

  points = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));

  unsigned seed = std::time(0);
  std::random_shuffle(xRange.begin(), xRange.end());

  for (auto i : xRange)
  {
    std::random_shuffle(yRange.begin(), yRange.end());
    for (auto j : yRange)
    {
      if (jitterSlider)
      {
        x = i + std::rand() % (2 * jitterSlider) - jitterSlider + 1;
      }
      else
      {
        x = i;
      }
      if (jitterSlider)
      {
        y = j + std::rand() % (2 * jitterSlider) - jitterSlider + 1;
      }
      else
      {
        y = j;
      }
      b = image.at<cv::Vec3b>(x, y)[0];
      g = image.at<cv::Vec3b>(x, y)[1];
      r = image.at<cv::Vec3b>(x, y)[2];
      cv::circle(points, cv::Point(y, x), radiusSlider, CV_RGB(r, g, b), -1, cv::LINE_AA);
    }
  }

  for (int i = 0; i < height; ++i)
  {
    for (int j = 0; j < width; ++j)
    {
      int borderRadius = border.at<uchar>(i, j) * (topSlider / 40 + 1) / 255;
      b = image.at<cv::Vec3b>(i, j)[0];
      g = image.at<cv::Vec3b>(i, j)[1];
      r = image.at<cv::Vec3b>(i, j)[2];
      cv::circle(points, cv::Point(j, i), borderRadius, CV_RGB(r, g, b), -1, cv::LINE_AA);
    }
  }

  cv::imshow("Cannyllism", points);
}

int main(int argc, char **argv)
{
  image = cv::imread(argv[1], cv::IMREAD_COLOR);

  if (!image.data)
  {
    std::cout << "Could not open or find the image: " << argv[1] << std::endl;
    return -1;
  }

  cv::namedWindow("Cannyllism", cv::WINDOW_NORMAL);
  cv::imshow("Cannyllism", image);

  std::srand(std::time(0));

  width = image.size().width;
  height = image.size().height;

  sprintf(TrackbarName, "Lower Threshold x %d", topSliderMax);
  cv::createTrackbar(TrackbarName, "Cannyllism", &topSlider, topSliderMax, onTrackbarCanny);

  sprintf(TrackbarName, "Step x %d", stepSliderMax);
  cv::createTrackbar(TrackbarName, "Cannyllism", &stepSlider, stepSliderMax, onTrackbarCanny);

  sprintf(TrackbarName, "Jitter x %d", jitterSliderMax);
  cv::createTrackbar(TrackbarName, "Cannyllism", &jitterSlider, jitterSliderMax, onTrackbarCanny);

  sprintf(TrackbarName, "Ray x %d", radiusSliderMax);
  cv::createTrackbar(TrackbarName, "Cannyllism", &radiusSlider, radiusSliderMax, onTrackbarCanny);

  cv::waitKey();
  return 0;
}
