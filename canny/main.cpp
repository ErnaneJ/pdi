#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat image, border;

char TrackbarName[50];
int top_slider = 10, top_slider_max = 200;

void onTrackbarCanny(int, void *)
{
  cv::Canny(image, border, top_slider, 3 * top_slider);
  cv::imshow("Canny", border);
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
    return EXIT_FAILURE;
  }

  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  if (image.empty())
  {
    std::cerr << "Error opening image: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  sprintf(TrackbarName, "Lower Threshold");

  cv::namedWindow("Canny", 1);
  cv::createTrackbar(TrackbarName, "Canny", &top_slider, top_slider_max, onTrackbarCanny);

  onTrackbarCanny(top_slider, 0);

  cv::waitKey();
  cv::imwrite("cannyborders.png", border);
  return EXIT_SUCCESS;
}