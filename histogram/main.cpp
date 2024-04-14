#include <iostream>
#include <opencv2/opencv.hpp>
#include "../helpers/cam.hpp"
#include "../helpers/histogram.hpp"

int main(int argc, char **argv)
{
  cv::Mat image, histR, histG, histB;
  cv::VideoCapture cap;

  int width, height, camera, equalizationEnabled = 0;

  camera = cameraEnumerator();
  cap.open(camera);

  if (!cap.isOpened())
  {
    std::cout << "No available cameras";
    exit(-1);
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  std::cout << "Width = " << width << std::endl;
  std::cout << "Height = " << height << std::endl;

  int nbins = 64;
  int histw = nbins, histh = nbins / 2;

  cv::Mat histImgR(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Mat histImgG(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Mat histImgB(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));

  while (true)
  {
    cap >> image;

    calculateAndVisualizeRGBHistograms(image, histImgR, histImgG, histImgB, histR, histG, histB, nbins);

    histImgR.copyTo(image(cv::Rect(0, 0, nbins, histh)));
    histImgG.copyTo(image(cv::Rect(0, histh, nbins, histh)));
    histImgB.copyTo(image(cv::Rect(0, 2 * histh, nbins, histh)));

    cv::imshow("image", image);
    int key = cv::waitKey(30);
    if (key == 27){
      std::cout << "Exiting..." << std::endl;
      break;
    }
  }

  return 0;
}