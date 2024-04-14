#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

int sliderMax = 100;
int unfocus_slider = 0;
int decay_slider = 0;
int offset_slider = 0;
int offset = 0;
float unfocus = 0; // 0.30;
int decay = 0;     // 50;
int frame_slider = 0;
int frame_count = 0;
int current_frame = 0;
int skip_frames = 1;
int skip_frames_slider = 1;

cv::Mat image;
cv::VideoCapture capture("../obra.mp4");
int videoWidth = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
int videoHeight = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

std::string videoFooter = "../obra.mp4 - " + std::to_string(videoWidth) + "x" + std::to_string(videoHeight);

void tiltShift()
{
  cv::Mat gauss_image = cv::Mat::zeros(image.rows, image.cols, CV_32FC3),
          focus_image = cv::Mat::zeros(image.rows, image.cols, CV_32FC3),
          unfocus_image = cv::Mat::zeros(image.rows, image.cols, CV_32FC3),
          focus_mask = cv::Mat(image.rows, image.cols, CV_32FC3, cv::Scalar(0, 0, 0)),
          unfocus_mask = cv::Mat(image.rows, image.cols, CV_32FC3, cv::Scalar(1, 1, 1)),
          output = cv::Mat::zeros(image.rows, image.cols, CV_32FC3);
  cv::Vec3f funcion_output;

  int l1 = unfocus * image.rows + offset;
  int l2 = image.rows - (unfocus * image.rows - offset);

  for (int i = 0; i < image.rows; i++)
  {
    for (int j = 0; j < image.cols; j++)
    {
      funcion_output[0] = (tanh((float(i - l1) / decay)) - tanh((float(i - l2) / decay))) / 2;
      funcion_output[1] = (tanh((float(i - l1) / decay)) - tanh((float(i - l2) / decay))) / 2;
      funcion_output[2] = (tanh((float(i - l1) / decay)) - tanh((float(i - l2) / decay))) / 2;
      focus_mask.at<cv::Vec3f>(i, j) = funcion_output;
    }
  }

  unfocus_mask = unfocus_mask - focus_mask;

  image.convertTo(image, CV_32FC3);
  cv::GaussianBlur(image, gauss_image, cv::Size(15, 15), 0, 0);

  focus_image = image.mul(focus_mask);
  unfocus_image = gauss_image.mul(unfocus_mask);

  output = focus_image + unfocus_image;

  image.convertTo(image, CV_8UC3);
  output.convertTo(output, CV_8UC3);

  cv::putText(output, videoFooter, cv::Point(10, capture.get(cv::CAP_PROP_FRAME_HEIGHT) - 10), cv::FONT_HERSHEY_SIMPLEX, .5, cv::Scalar(255, 255, 255), 1);

  cv::imshow("Tiltshift - Video", output);
}

void on_trackbar(int, void *)
{
  unfocus = (float)unfocus_slider / sliderMax;
  decay = decay_slider;
  offset = (offset_slider - unfocus_slider) * image.rows / sliderMax;
  capture.set(cv::CAP_PROP_POS_FRAMES, frame_slider);
  skip_frames = skip_frames_slider;

  tiltShift();
}

int main(int argc, char **argv)
{
  if (!capture.isOpened())
  {
    std::cout << "Erro abrindo o video!" << std::endl;
    exit(-1);
  }

  capture >> image;
  cv::namedWindow("Tiltshift - Video", cv::WINDOW_AUTOSIZE);

  frame_count = capture.get(cv::CAP_PROP_FRAME_COUNT);
  cv::createTrackbar("Frame", "Tiltshift - Video", &frame_slider, frame_count - 1, on_trackbar, NULL);
  cv::createTrackbar("Unfocus", "Tiltshift - Video", &unfocus_slider, sliderMax, on_trackbar, NULL);
  cv::createTrackbar("Decay", "Tiltshift - Video", &decay_slider, sliderMax, on_trackbar, NULL);
  cv::createTrackbar("Offset", "Tiltshift - Video", &offset_slider, sliderMax, on_trackbar, NULL);
  cv::createTrackbar("Skip Frames", "Tiltshift - Video", &skip_frames_slider, frame_count - 1, on_trackbar, NULL);

  while(true)
  {
    if(frame_count > current_frame){
      capture >> image;
      if(skip_frames > (frame_count - current_frame)){
        skip_frames = frame_count - current_frame - 1;
      }
      for (int i = 0; i < skip_frames; i++){
        capture >> image;
        if (image.empty()) break;
      }
    }

    current_frame = capture.get(cv::CAP_PROP_POS_FRAMES); 
    cv::setTrackbarPos("Frame", "Tiltshift - Video", current_frame); // Update the position of the trackbar

    tiltShift();

    if (cv::waitKey(30) > 0)
      break;
  }

  int centerX = image.cols / 2;
  int centerY = image.rows / 2;
  std::string text = "Press any key to exit.";
  cv::Size textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 1, 2, nullptr);
  int textX = centerX - textSize.width / 2;
  int textY = centerY + textSize.height / 2;

  cv::putText(image, text, cv::Point(textX, textY), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
  
  cv::imshow("Tiltshift - Video", image);
  cv::waitKey(0);

  return 0;
}