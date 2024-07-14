#include <iostream>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

int main(int argc, char **argv)
{
  cv::Mat image;

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " <input_image>\n";
    return -1;
  }

  image = cv::imread(argv[1], cv::IMREAD_COLOR);

  if (image.empty())
  {
    std::cout << "Error loading image: " << argv[1] << std::endl;
    return -1;
  }

  cv::Mat structuring_element;
  structuring_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
  cv::morphologyEx(image, image, cv::MORPH_OPEN, structuring_element);
  cv::morphologyEx(image, image, cv::MORPH_CLOSE, structuring_element);

  structuring_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 5));
  
  cv::erode(image, image, structuring_element);
  cv::erode(image, image, structuring_element);
  cv::erode(image, image, structuring_element);
  cv::erode(image, image, structuring_element);
  cv::erode(image, image, structuring_element);

  cv::dilate(image, image, structuring_element);
  cv::dilate(image, image, structuring_element);
  cv::dilate(image, image, structuring_element);
  cv::dilate(image, image, structuring_element);

  cv::imshow("image", image);
  cv::imwrite("../assets/output.png", image);

  tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
  if (api->Init(NULL, "eng"))
  {
    std::cerr << "Could not initialize tesseract.\n";
    return 1;
  }

  api->SetImage(image.data, image.cols, image.rows, 3, image.step);

  char *outText = api->GetUTF8Text();
  std::cout << "OCR output: " << outText;

  api->End();
  delete[] outText;
  delete api;

  cv::imshow("image", image);
  cv::waitKey(0);

  return 0;
}
