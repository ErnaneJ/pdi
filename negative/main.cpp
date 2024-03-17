#include <iostream>
#include <opencv2/opencv.hpp>

#define WINDOW_NAME "Negative"
#define IMAGE_FILE "biel.png"

/**
 * @brief Captures an image from a file.
 *
 * This function reads an image file specified by the IMAGE_FILE macro
 * and converts it to grayscale.
 *
 * @return The captured image as a grayscale cv::Mat.
 **/
cv::Mat captureImage()
{
  cv::Mat image = cv::imread(IMAGE_FILE, cv::IMREAD_GRAYSCALE);
  if (image.empty())
  {
    std::cerr << "\n❌ Negative - Error: Image (" << IMAGE_FILE << ") not found.\n"
              << std::endl;
    exit(1);
  }
  return image;
}

/**
 * @brief Displays an image in a named window.
 *
 * This function displays the specified image in a named window.
 *
 * @param image The image to be displayed.
 * @param windowName The name of the window.
 * @param index The index for organizing multiple windows.
 **/
void showImage(cv::Mat image, std::string windowName = WINDOW_NAME, int index = 0)
{
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
  cv::imshow(windowName, image);

  cv::Rect windowRect = cv::getWindowImageRect(windowName);
  cv::moveWindow(windowName, ((windowRect.width + 30) * index + 30), 0);
}

/**
 * @brief Captures two points from the user.
 *
 * This function prompts the user to enter coordinates for two points (P1 and P2).
 * It checks the validity of the entered points.
 *
 * @param image The image for which points are being captured.
 * @param p1X Reference to the X-coordinate of the first point (P1).
 * @param p1Y Reference to the Y-coordinate of the first point (P1).
 * @param p2X Reference to the X-coordinate of the second point (P2).
 * @param p2Y Reference to the Y-coordinate of the second point (P2).
 **/
void capturePoints(cv::Mat image, int &p1X, int &p1Y, int &p2X, int &p2Y)
{
  std::cout << "\n🔄 Negative Region Inverter - [" << IMAGE_FILE << "] 🔄\n" <<std::endl;
  std::cout << "──────────────────────────────────────────────" << std::endl;
  std::cout << "\n🚨 Enter the coordinates for two points (P1 and P2).\n" << std::endl;
  std::cout << "📍 Enter the P1x: ";
  std::cin >> p1X;
  std::cout << "📍 Enter the P1y: ";
  std::cin >> p1Y;
  if (p1X < 0 || p1Y < 0 || p1X > image.cols || p1Y > image.rows)
  {
    std::cerr << "\n❌ Negative - Error: Invalid point. Please enter a valid point. P1x and P1y must be greater than or equal to 0 and less than the image's width (" << image.cols << " px) and height (" << image.rows << " px), respectively.\n"
              << std::endl;
    exit(1);
  }
  std::cout << "\n✅ P1x and P1y are valid.\n" << std::endl;
  std::cout << "📍 Enter the P2x: ";
  std::cin >> p2X;
  std::cout << "📍 Enter the P2y: ";
  std::cin >> p2Y;
  if (p2X < 0 || p2Y < 0 || p2X > image.cols || p2Y > image.rows || p2X <= p1X || p2Y <= p1Y)
  {
    std::cerr << "\n❌ Negative - Error: Invalid point. Please enter a valid point. P2x and P2y must be greater than or equal to 0 and less than the image's width (" << image.cols << " px) and height (" << image.rows << "), respectively and greater than P1x (" << p1X << " px) and P1y (" << p1Y << " px)\n"
              << std::endl;
    exit(1);
  }
  std::cout << "\n✅ P2x and P2y are valid." << std::endl;
  std::cout << "✅ P1 and P2 are valid." << std::endl;

  std::cout << "\n🚨 The region to be inverted is from P1(" << p1X << ", " << p1Y << ") to P2(" << p2X << ", " << p2Y << ").\n" << std::endl;
}

/**
 * @brief Inverts the pixel values in a specified region of the image.
 *
 * This function iterates over the specified region of the image and
 * inverts the pixel values.
 *
 * @param image Reference to the image.
 * @param p1X The X-coordinate of the top-left corner of the region.
 * @param p1Y The Y-coordinate of the top-left corner of the region.
 * @param p2X The X-coordinate of the bottom-right corner of the region.
 * @param p2Y The Y-coordinate of the bottom-right corner of the region.
 **/
void negativeRegion(cv::Mat &image, int p1X, int p1Y, int p2X, int p2Y)
{
  for (int row = p1Y; row <= p2Y; row++)
  {
    for (int col = p1X; col <= p2X; col++)
    {
      image.at<uchar>(row, col) = 255 - image.at<uchar>(row, col);
    }
  }
}

int main(int, char **)
{
  int p1X = 0, p1Y = 0, p2X = 0, p2Y = 0;
  cv::Mat image = captureImage();

  showImage(image, "Biel - Original", 0);

  capturePoints(image, p1X, p1Y, p2X, p2Y);
  negativeRegion(image, p1X, p1Y, p2X, p2Y);

  std::cout << "✅ Region inverted successfully!" << std::endl;
  showImage(image, "Biel - Negative", 2);

  std::cout << "\n──────────────────────────────────────────────" << std::endl;
  std::cout << "\n🎓 Sudent: Ernane Ferreira - DCA0445" << std::endl;

  cv::waitKey(0);

  return 0;
};
