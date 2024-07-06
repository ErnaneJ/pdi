#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
  cv::Mat image;

  // Load the image in grayscale mode
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data)
  {
    std::cout << "Failed to open " << argv[1] << std::endl;
    return 0;
  }

  // Apply binary threshold using Otsu's method
  cv::threshold(image, image, 1, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

  // Find contours
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  // Convert image to BGR color for displaying colored contours
  cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);

  // Open SVG file for writing contours
  std::ofstream file("../assets/contours.svg");
  if (!file.is_open())
  {
    std::cout << "Failed to open contornos.svg" << std::endl;
    return 0;
  }

  // Write SVG file header
  file << "<svg height=\"" << image.rows << "\" width=\"" << image.cols
       << "\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;

  // Write contours to the SVG file and draw them on the image
  for (size_t i = 0; i < contours.size(); ++i)
  {
    file << "<path d=\"M " << contours[i][0].x << " " << contours[i][0].y << " ";
    for (size_t j = 1; j < contours[i].size(); ++j)
    {
      file << "L" << contours[i][j].x << " " << contours[i][j].y << " ";
    }
    file << "Z\" fill=\"#cccccc\" stroke=\"red\" stroke-width=\"1\" />" << std::endl;

    cv::drawContours(image, contours, static_cast<int>(i), cv::Scalar(255, 0, 0));
  }

  // Write SVG file footer
  file << "</svg>" << std::endl;
  file.close();

  // Display the image with contours
  cv::imshow("Image", image);
  cv::waitKey();

  return 0;
}
