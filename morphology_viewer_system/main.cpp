#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
  cv::Mat image, erosion, dilation, opening, closing, open_close;
  cv::Mat structuring_element;

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " <input_image>\n";
    return -1;
  }

  // Load the input image
  image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

  if (image.empty())
  {
    std::cout << "Error loading image: " << argv[1] << std::endl;
    return -1;
  }

  std::cout << "Structuring Element: \n"
            << structuring_element << std::endl;
  cv::imshow("image", image);

  bool running = true;
  std::cout << "\n== Morphological Operations ==\n\n"
            << "1 - Erosion\n"
            << "2 - Dilation\n"
            << "3 - Opening\n"
            << "4 - Closing\n"
            << "5 - Gradient\n"
            << "6 - TopHat\n"
            << "7 - BlackHat\n"
            << "8 - HitMiss\n"
            << "9 - Opening and Closing\n\n"
            << "[ESC] - Exit\n"
            << "\n=============================\n\n";

  while (running)
  {
    int operation = cv::waitKey(0);
    switch (operation)
    {
    case 49: // 1
      std::cout << "-> (1) Erosion\n";
      cv::erode(image, image, structuring_element);
      break;
    case 50: // 2
      std::cout << "-> (2) Dilation\n";
      cv::dilate(image, image, structuring_element);
      break;
    case 51: // 3
      std::cout << "-> (3) Opening\n";
      cv::morphologyEx(image, image, cv::MORPH_OPEN, structuring_element);
      break;
    case 52: // 4
      std::cout << "-> (4) Closing\n";
      cv::morphologyEx(image, image, cv::MORPH_CLOSE, structuring_element);
      break;
    case 53: // 5
      std::cout << "-> (5) Gradient\n";
      cv::morphologyEx(image, image, cv::MORPH_GRADIENT, structuring_element);
      break;
    case 54: // 6
      std::cout << "-> (6) TopHat\n";
      cv::morphologyEx(image, image, cv::MORPH_TOPHAT, structuring_element);
      break;
    case 55: // 7
      std::cout << "-> (7) BlackHat\n";
      cv::morphologyEx(image, image, cv::MORPH_BLACKHAT, structuring_element);
      break;
    case 56: // 8
      std::cout << "-> (8) HitMiss\n";
      cv::morphologyEx(image, image, cv::MORPH_HITMISS, structuring_element);
      break;
    case 57: // 9
      std::cout << "-> (9) Opening and Closing\n";
      cv::morphologyEx(image, image, cv::MORPH_OPEN, structuring_element);
      cv::morphologyEx(image, image, cv::MORPH_CLOSE, structuring_element);
      break;
    case 27: // esc
      std::cout << "Exiting...\n";
      running = false;
      break;
    case 118: // v - vertical
      structuring_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 5));
      std::cout << "Vertical Structuring Element: "
                << structuring_element << std::endl;
      break;
    case 104: // h - horizontal
      structuring_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 1));
      std::cout << "Horizontal Structuring Element: "
                << structuring_element << std::endl;
      break;
    case 113: // q - square
      structuring_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
      std::cout << "Square Structuring Element: "
                << structuring_element << std::endl;
      break;
    default:
      std::cout << "Invalid operation: " << operation << std::endl;
      break;
    }

    cv::imshow("image", image);
    cv::imwrite("../assets/output.png", image);
  }

  std::cout << "System terminated! Image saved."
            << "=============================\n\n";

  return 0;
}
