#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#define RADIUS 20

/**
 * \brief Shifts the quadrants of the DFT image.
 * 
 * \param image The input DFT image.
 */
void shiftDFT(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;

  A = image(cv::Rect(0, 0, cx, cy));
  B = image(cv::Rect(cx, 0, cx, cy));
  C = image(cv::Rect(0, cy, cx, cy));
  D = image(cv::Rect(cx, cy, cx, cy));

  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

int main(int, char**) {
  int dftM, dftN;
  float mean; 

  cv::Mat imaginaryInput, complexImage, multsp;
  cv::Mat padded, filter, mag;
  cv::Mat image, imageGray, tmp, magI;
  cv::Mat_<float> realInput, zeros;
  std::vector<cv::Mat> planes;

  // Load image from YAML file
  // cv::FileStorage fs("../sinusoidal-256-8.yml", cv::FileStorage::READ);
  // fs["mat"] >> image;
  // cv::normalize(image, image, 0, 255, cv::NORM_MINMAX);
  // image.convertTo(image, CV_32FC2);
  // cv::cvtColor(image, image, cv::IMREAD_COLOR);

  // Load image from PNG file
  image = cv::imread("../target.png", cv::IMREAD_COLOR);

  dftM = cv::getOptimalDFTSize(image.rows);
  dftN = cv::getOptimalDFTSize(image.cols);

  cv::copyMakeBorder(image, padded, 0, dftM - image.rows, 0,
                     dftN - image.cols, cv::BORDER_CONSTANT,
                     cv::Scalar::all(0));

  zeros = cv::Mat_<float>::zeros(padded.size());

  complexImage = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));

  filter = complexImage.clone();

  tmp = cv::Mat(dftM, dftN, CV_32F);

  for (int i = 0; i < dftM; i++) {
    for (int j = 0; j < dftN; j++) {
      if ((i - dftM / 2) * (i - dftM / 2) +
              (j - dftN / 2) * (j - dftN / 2) <
          RADIUS * RADIUS) {
        tmp.at<float>(i, j) = 1.0;
      }
    }
  }

  cv::Mat comps[] = {tmp, tmp};
  cv::merge(comps, 2, filter);

  cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);

  cv::imwrite("../target-original.png", imageGray);
  // cv::imwrite("../sinusoidal-256-8-original.png", imageGray);
  cv::imshow("Original", imageGray);

  cv::copyMakeBorder(imageGray, padded, 0, dftM - image.rows, 0,
                      dftN - image.cols, cv::BORDER_CONSTANT,
                      cv::Scalar::all(0));

  planes.clear();
  realInput = cv::Mat_<float>(padded);
  planes.push_back(realInput);
  planes.push_back(zeros);

  cv::merge(planes, complexImage);

  cv::dft(complexImage, complexImage);
  shiftDFT(complexImage);

  {
    planes.clear();
    cv::split(complexImage, planes);

    cv::Mat magn, angl, anglInt, magnInt;
    cv::cartToPolar(planes[0], planes[1], magn, angl, false);
    cv::normalize(angl, angl, 0, 255, cv::NORM_MINMAX);
    angl.convertTo(anglInt, CV_8U);
    cv::imshow("Phase Angle", anglInt);

    cv::magnitude(planes[0], planes[1], planes[0]);
    magI = planes[0];
    magI += cv::Scalar::all(1);
    cv::log(magI, magI);
    cv::normalize(magI, magI, 0, 255, cv::NORM_MINMAX);
    magI.convertTo(magnInt, CV_8U);
    cv::imshow("Spectrum", magnInt);

    cv::imwrite("../target-dft.png", padded);
    cv::imwrite("../target-dft-spectrum.png", magnInt);
    cv::imwrite("../target-dft-angle.png", anglInt);
    // cv::imwrite("../sinusoidal-256-8-dft.png", padded);
    // cv::imwrite("../sinusoidal-256-8-dft-spectrum.png", magnInt);
    // cv::imwrite("../sinusoidal-256-8-dft-angle.png", anglInt);
  }

  cv::mulSpectrums(complexImage, filter, complexImage, 0);

  planes.clear();

  cv::split(complexImage, planes);

  mean = cv::abs(planes[0].at<float>(dftM / 2, dftN / 2));

  cv::merge(planes, complexImage);

  shiftDFT(complexImage);

  cv::idft(complexImage, complexImage);

  planes.clear();

  cv::split(complexImage, planes);

  cv::normalize(planes[0], planes[0], 0, 1, cv::NORM_MINMAX);
  cv::imshow("Filtered", planes[0]);

  cv::waitKey(0);
  return 0;
}
