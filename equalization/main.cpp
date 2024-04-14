#include <iostream>
#include <opencv2/opencv.hpp>
#include "../helpers/cam.hpp"
#include "../helpers/histogram.hpp"

int main(int argc, char **argv)
{
  cv::Mat frame, rgbFrame, monochromeFrame, equalizedFrame, histR, histG, histB, histMono, histEq;
  cv::VideoCapture cap;

  int width, height, camera;
  bool equalizationEnabled = false;

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

  cv::namedWindow("Original Image (BGR)", cv::WINDOW_NORMAL);
  cv::namedWindow("Original Image (Monochrome)", cv::WINDOW_NORMAL);
  cv::namedWindow("Equalized Image", cv::WINDOW_NORMAL);

  cv::moveWindow("Original Image (BGR)", 0, 0);
  cv::moveWindow("Original Image (Monochrome)", width, 0);
  cv::moveWindow("Equalized Image", width * 2, 0);

  int nbins = 64;
  int histw = nbins, histh = nbins / 2;

  cv::Mat histImgR(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Mat histImgG(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Mat histImgB(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
  cv::Mat histImgEq(histh, histw, CV_8UC1, cv::Scalar(0, 0, 0));
  cv::Mat histImgMono(histh, histw, CV_8UC1, cv::Scalar(0, 0, 0));

  while (true)
  {
    cap >> frame;
    rgbFrame = frame.clone();

    calculateAndVisualizeRGBHistograms(rgbFrame, histImgR, histImgG, histImgB, histR, histG, histB, nbins);

    histImgR.copyTo(rgbFrame(cv::Rect(0, 0, nbins, histh)));
    histImgG.copyTo(rgbFrame(cv::Rect(0, histh, nbins, histh)));
    histImgB.copyTo(rgbFrame(cv::Rect(0, 2 * histh, nbins, histh)));

    cv::imshow("Original Image (BGR)", rgbFrame);
    
    monochromeFrame = frame.clone();
    cv::cvtColor(monochromeFrame, monochromeFrame, cv::COLOR_BGR2GRAY);
    calculateAndVisualizeMonoHistograms(monochromeFrame, histImgMono, histMono, nbins);
    histImgMono.copyTo(monochromeFrame(cv::Rect(0, 0, nbins, histh)));
    cv::imshow("Original Image (Monochrome)", monochromeFrame);
    
    cv::equalizeHist(monochromeFrame, equalizedFrame);
    calculateAndVisualizeMonoHistograms(equalizedFrame, histImgEq, histEq, nbins);
    histImgEq.copyTo(equalizedFrame(cv::Rect(0, 0, nbins, histh)));
    cv::imshow("Equalized Image", equalizedFrame);

    int key = cv::waitKey(30);
    if (key == 27){ // ESC
      std::cout << "Exiting..." << std::endl;
      break;
    }
  }

  cv::destroyAllWindows();

  return 0;
}