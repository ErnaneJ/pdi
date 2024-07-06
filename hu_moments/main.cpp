#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " <base_image> <target_image1> <target_image2> ..." << std::endl;
    return -1;
  }

  // Load base image
  cv::Mat baseImageGray = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  cv::Mat baseImageColor = cv::imread(argv[1], cv::IMREAD_COLOR);

  if (baseImageGray.empty() || baseImageColor.empty())
  {
    std::cerr << "Could not open or find the base image!" << std::endl;
    return -1;
  }

  // Resize base image for faster processing
  double scaleFactor = 0.5; // Adjust as necessary
  // cv::resize(baseImageGray, baseImageGray, cv::Size(), scaleFactor, scaleFactor);
  // cv::resize(baseImageColor, baseImageColor, cv::Size(), scaleFactor, scaleFactor);

  // Loop through each target image
  for (int i = 2; i < argc; ++i)
  {
    // Load target image
    cv::Mat targetImageGray = cv::imread(argv[i], cv::IMREAD_GRAYSCALE);
    if (targetImageGray.empty())
    {
      std::cerr << "Could not open or find target image " << argv[i] << "!" << std::endl;
      continue;
    }

    // Resize target image for faster processing
    // cv::resize(targetImageGray, targetImageGray, cv::Size(), scaleFactor, scaleFactor);

    // Calculate Hu Moments for the target image
    double targetHuMoments[7];
    cv::Moments targetMoments = cv::moments(targetImageGray, false);
    cv::HuMoments(targetMoments, targetHuMoments);

    for (int j = 0; j < 7; j++)
    {
      targetHuMoments[j] = -1 * std::copysign(1.0, targetHuMoments[j]) * std::log10(std::abs(targetHuMoments[j]));
    }

    // Sliding window approach to find the target in the base image
    int windowHeight = targetImageGray.rows;
    int windowWidth = targetImageGray.cols;
    double minDiff = std::numeric_limits<double>::max();
    cv::Point bestMatch;

    std::cout << "🔍 Searching for target " << i - 1 << " in the base image" << std::endl;
    for (int y = 0; y <= baseImageGray.rows - windowHeight; y++)
    {
      for (int x = 0; x <= baseImageGray.cols - windowWidth; x++)
      {
        cv::Rect window(x, y, windowWidth, windowHeight);
        cv::Mat subImage = baseImageGray(window);

        // Calculate Hu Moments for the current window
        double subImageHuMoments[7];
        cv::Moments subImageMoments = cv::moments(subImage, false);
        cv::HuMoments(subImageMoments, subImageHuMoments);

        for (int j = 0; j < 7; j++)
        {
          subImageHuMoments[j] = -1 * std::copysign(1.0, subImageHuMoments[j]) * std::log10(std::abs(subImageHuMoments[j]));
        }

        // Calculate difference between moments
        double diff = 0.0;
        for (int j = 0; j < 7; j++)
        {
          diff += std::abs(targetHuMoments[j] - subImageHuMoments[j]);
        }

        // Update best match if current diff is lower
        if (diff < minDiff)
        {
          minDiff = diff;
          bestMatch = cv::Point(x, y);
        }
      }
    }

    // Draw a rectangle on the base image
    int rectWidth = windowWidth;
    int rectHeight = windowHeight;
    cv::Point topLeft(bestMatch.x, bestMatch.y);
    cv::Point bottomRight(bestMatch.x + rectWidth, bestMatch.y + rectHeight);
    cv::rectangle(baseImageColor, topLeft, bottomRight, cv::Scalar(0, 0, 255), 4); // Red rectangle, thickness 4

    std::cout << "Target " << i - 1 << " best match found at: " << bestMatch << std::endl;
    std::cout << "Difference: " << minDiff << std::endl;
    std::cout << "Target " << i - 1 << " Hu Moments: ";
    for (int j = 0; j < 7; j++)
    {
      std::cout << targetHuMoments[j] << " ";
    }
    std::cout << std::endl;

    // Display the base image with the rectangle
    cv::imshow("Base Image with Rectangle", baseImageColor);
    cv::waitKey(0);
  }

  std::cout << std::endl;

  return 0;
}
