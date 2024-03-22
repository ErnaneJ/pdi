#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

/**
 * @brief Generate a sinusoidal wave image and save it as both YAML and PNG files.
 *
 * @param side Length of each side of the square image.
 * @param periods Number of periods of the sinusoidal wave.
 */
void generateSinusoidalImage(int side, int periods)
{
  cv::Mat image = cv::Mat::zeros(side, side, CV_32FC1);

  for (int i = 0; i < side; i++)
  {
    for (int j = 0; j < side; j++)
    {
      image.at<float>(i, j) = 127 * sin(M_PI * periods * j / side) + 128;
    }
  }

  // Save image as YAML file
  std::stringstream ss_yml;
  ss_yml << "sinusoidal-" << side << '-' << periods << ".yml";
  cv::FileStorage fs_yml(ss_yml.str(), cv::FileStorage::WRITE);
  fs_yml << "mat" << image;
  fs_yml.release();

  // Save image as PNG file
  cv::Mat image_png;
  cv::normalize(image, image_png, 0, 255, cv::NORM_MINMAX);
  image_png.convertTo(image_png, CV_8U);
  std::stringstream ss_png;
  ss_png << "sinusoidal-" << side << '-' << periods << ".png";
  cv::imwrite(ss_png.str(), image_png);
}

/**
 * @brief Display a previously generated sinusoidal image.
 *
 * @param side Length of each side of the square image.
 * @param periods Number of periods of the sinusoidal wave.
 * @param window_name Name of the window to display the image.
 */
void displaySinusoidalImage(int side, int periods, const std::string &window_name = "Sinusoidal Image")
{
  // Load image from YAML file
  std::stringstream ss_yml;
  ss_yml << "sinusoidal-" << side << '-' << periods << ".yml";
  cv::FileStorage fs(ss_yml.str(), cv::FileStorage::READ);
  cv::Mat image;
  fs["mat"] >> image;

  // Normalize and display image
  cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
  image.convertTo(image, CV_8U);
  cv::imshow(window_name, image);
}

void plotCurve(float *curve, int size, const std::string &window_name)
{
  cv::Mat graph = cv::Mat::zeros(256, size, CV_8UC3);
  for (int i = 0; i < size; ++i)
  {
    cv::line(graph, cv::Point(i, 128), cv::Point(i, 256 - static_cast<int>(curve[i])), cv::Scalar(255, 255, 255));
  }

  cv::imshow(window_name, graph);
}

/**
 * @brief Calculate the difference between two lines.
 *
 * @param line1 The first line.
 * @param line2 The second line.
 * @param size The size of the lines.
 * @return The difference between the lines.
 */
float *calculateLineDifference(float *line1, float *line2, int size)
{
  float *difference = new float[size];

  for (int i = 0; i < size; ++i)
  {
    std::cout << "-> " << line1[i] << " - " << line2[i] << " = " << std::abs(line1[i] - line2[i]) << std::endl;
    difference[i] = std::abs(line1[i] - line2[i]);
  }

  return difference;
}

int main()
{
  int side = 256;

  // Generate and display sinusoidal images with different periods
  generateSinusoidalImage(side, 8);
  generateSinusoidalImage(side, 4);

  // Calculate and display difference between the two sinusoidal curves
  cv::FileStorage fs1("sinusoidal-256-8.yml", cv::FileStorage::READ);
  cv::FileStorage fs2("sinusoidal-256-4.yml", cv::FileStorage::READ);

  cv::Mat image1, image2;
  fs1["mat"] >> image1;
  fs2["mat"] >> image2;

  int row_index = 128; // middle row
  int size = image1.cols;
  float *line1 = new float[size];
  float *line2 = new float[size];

  for (int col = 0; col < size; ++col)
  {
    line1[col] = image1.at<float>(row_index, col);
    line2[col] = image2.at<float>(row_index, col);
  }

  float *difference = calculateLineDifference(line1, line2, size);

  plotCurve(difference, size, "Difference - 256 - 8 vs 4 - Middle Row");
  plotCurve(line1, size, "Sinusoidal Curve - 256 - 8 - Middle Row");
  displaySinusoidalImage(side, 8, "Sinusoidal Image - 256 - 8");
  plotCurve(line2, size, "Sinusoidal Curve - 256 - 4 - Middle Row");
  displaySinusoidalImage(side, 4, "Sinusoidal Image - 256 - 4");

  delete[] line1;
  delete[] line2;
  delete[] difference;

  cv::waitKey();

  return 0;
}
