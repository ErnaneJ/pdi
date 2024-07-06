#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat image, filteredImage, paddedImage;
const int gammaL_max = 10, gammaH_max = 50, sharpC_max = 100, cutoff_max = 200;
int gammaL_slider = 2, gammaH_slider = 20, sharpC_slider = 1, cutoff_slider = 5,
    gammaL, gammaH, sharpC, cutoff, dft_M, dft_N;

void shiftDFT(cv::Mat &image)
{
  cv::Mat tmp, topLeft, topRight, bottomLeft, bottomRight;
  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;

  topLeft = image(cv::Rect(0, 0, cx, cy));
  topRight = image(cv::Rect(cx, 0, cx, cy));
  bottomLeft = image(cv::Rect(0, cy, cx, cy));
  bottomRight = image(cv::Rect(cx, cy, cx, cy));

  topLeft.copyTo(tmp);
  bottomRight.copyTo(topLeft);
  tmp.copyTo(bottomRight);

  bottomLeft.copyTo(tmp);
  topRight.copyTo(bottomLeft);
  tmp.copyTo(topRight);
}

cv::Mat createHomomorphicFilter(double gammaL, double gammaH, double sharpC, double cutoff)
{
  cv::Mat filter = cv::Mat(paddedImage.size(), CV_32FC2, cv::Scalar(0));
  cv::Mat tmp = cv::Mat(dft_M, dft_N, CV_32F);

  for (int i = 0; i < dft_M; i++)
  {
    for (int j = 0; j < dft_N; j++)
    {
      tmp.at<float>(i, j) = (gammaH - gammaL) * (1 - exp(-sharpC * ((pow(i - dft_M / 2, 2) + pow(j - dft_N / 2, 2)) / pow(cutoff, 2)))) + gammaL;
    }
  }

  cv::Mat components[] = {tmp, tmp};
  cv::imshow("Filter", tmp);
  cv::merge(components, 2, filter);
  cv::normalize(filter, filter, 0, 1, cv::NORM_MINMAX);
  return filter;
}

void applyHomomorphicFilter()
{
  std::vector<cv::Mat> planes;
  cv::Mat zeros = cv::Mat_<float>::zeros(paddedImage.size());
  cv::Mat realInput = cv::Mat_<float>(paddedImage);
  cv::Mat complexImage;

  realInput += cv::Scalar::all(1);
  log(realInput, realInput);
  cv::normalize(realInput, realInput, 0, 1, cv::NORM_MINMAX);

  planes.push_back(realInput);
  planes.push_back(zeros);
  cv::merge(planes, complexImage);

  cv::dft(complexImage, complexImage);
  shiftDFT(complexImage);
  cv::resize(complexImage, complexImage, paddedImage.size());
  cv::normalize(complexImage, complexImage, 0, 1, cv::NORM_MINMAX);

  cv::Mat filter = createHomomorphicFilter(gammaL, gammaH, sharpC, cutoff);
  cv::mulSpectrums(complexImage, filter, complexImage, 0);
  shiftDFT(complexImage);
  cv::idft(complexImage, complexImage);
  cv::normalize(complexImage, complexImage, 0, 1, cv::NORM_MINMAX);

  planes.clear();
  cv::split(complexImage, planes);
  exp(planes[0], planes[0]);
  cv::normalize(planes[0], planes[0], 0, 1, cv::NORM_MINMAX);
  filteredImage = planes[0].clone();
}

void onTrackbar(int, void *)
{
  gammaL = static_cast<double>(gammaL_slider) / 10;
  gammaH = static_cast<double>(gammaH_slider) / 10;
  sharpC = static_cast<double>(sharpC_slider);
  cutoff = static_cast<double>(cutoff_slider);
  applyHomomorphicFilter();
  cv::imshow("Homomorphic Filtered Image", filteredImage);
}

int main(int argc, char *argv[])
{
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  if (image.empty())
  {
    std::cerr << "Error opening image: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  cv::namedWindow("Original Image", cv::WINDOW_NORMAL);
  cv::namedWindow("Homomorphic Filtered Image", cv::WINDOW_NORMAL);
  cv::namedWindow("Filter", cv::WINDOW_NORMAL);

  cv::imshow("Original Image", image);

  dft_M = cv::getOptimalDFTSize(image.rows);
  dft_N = cv::getOptimalDFTSize(image.cols);
  cv::copyMakeBorder(image, paddedImage, 0, dft_M - image.rows, 0, dft_N - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
  filteredImage = paddedImage.clone();

  std::cout << "Original size: " << image.rows << "x" << image.cols << std::endl;
  std::cout << "Padded size: " << paddedImage.rows << "x" << paddedImage.cols << std::endl;

  cv::createTrackbar("Gamma L", "Homomorphic Filtered Image", &gammaL_slider, gammaL_max, onTrackbar);
  cv::createTrackbar("Gamma H", "Homomorphic Filtered Image", &gammaH_slider, gammaH_max, onTrackbar);
  cv::createTrackbar("Sharpness", "Homomorphic Filtered Image", &sharpC_slider, sharpC_max, onTrackbar);
  cv::createTrackbar("Cutoff Frequency", "Homomorphic Filtered Image", &cutoff_slider, cutoff_max, onTrackbar);

  onTrackbar(0, 0);
  cv::waitKey(0);
  return EXIT_SUCCESS;
}
