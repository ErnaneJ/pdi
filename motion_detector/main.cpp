#include <iostream>
#include <opencv2/opencv.hpp>
#include "../helpers/cam.hpp"
#include "../helpers/histogram.hpp"

/**
 * @brief Draw a red border around the image.
 *
 * This function draws a red border around the input image.
 *
 * @param image The input image.
 * @return The image with a red border.
 */
cv::Mat drawRedBorder(const cv::Mat &image)
{
  cv::Mat borderedImage;
  cv::Scalar red = cv::Scalar(0, 0, 255); // BGR color code for red

  borderedImage = image.clone();

  cv::line(borderedImage, cv::Point(0, 0), cv::Point(image.cols, 0), red, 5);
  cv::line(borderedImage, cv::Point(0, image.rows - 1), cv::Point(image.cols, image.rows - 1), red, 5);
  cv::line(borderedImage, cv::Point(0, 0), cv::Point(0, image.rows), red, 5);
  cv::line(borderedImage, cv::Point(image.cols - 1, 0), cv::Point(image.cols - 1, image.rows), red, 5);

  return borderedImage;
}

int main(int argc, char **argv)
{
  cv::Mat frame, histR, histG, histB, lastHistR, lastHistG, lastHistB;
  cv::VideoCapture cap;

  int width, height, camera, equalizationEnabled = 0;

  camera = cameraEnumerator();
  cap.open(camera);

  if (!cap.isOpened())
  {
    std::cout << "No available cameras.";
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

  int motionCounter = 0;
  while (true)
  {
    cap >> frame;

    calculateAndVisualizeRGBHistograms(frame, histImgR, histImgG, histImgB, histR, histG, histB, nbins);

    histImgR.copyTo(frame(cv::Rect(0, 0, nbins, histh)));
    histImgG.copyTo(frame(cv::Rect(0, histh, nbins, histh)));
    histImgB.copyTo(frame(cv::Rect(0, 2 * histh, nbins, histh)));

    if (lastHistR.empty())
    {
      lastHistR = histR.clone();
      lastHistG = histG.clone();
      lastHistB = histB.clone();
    }

    double diffR = cv::compareHist(lastHistR, histR, cv::HISTCMP_CHISQR);
    double diffG = cv::compareHist(lastHistG, histG, cv::HISTCMP_CHISQR);
    double diffB = cv::compareHist(lastHistB, histB, cv::HISTCMP_CHISQR);

    double avgDiffs = (diffR + diffG + diffB) / 3;

    if (avgDiffs > 2.5)
    {
      frame = drawRedBorder(frame);
      std::cout << "🚨 Motion detected! { id: " << motionCounter << ", avgDiffs: " << avgDiffs << " };" << std::endl;
      motionCounter++;
    }

    cv::imshow("Video Capture", frame);

    lastHistR = histR.clone();
    lastHistG = histG.clone();
    lastHistB = histB.clone();

    int key = cv::waitKey(30);
    if (key == 27)
    {
      std::cout << "\nExiting..." << std::endl;
      break;
    }
  }

  return 0;
}