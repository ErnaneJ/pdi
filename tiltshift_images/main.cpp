#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

int sliderMax = 100;
int unfocus_slider = 0;
int decay_slider = 0; 
int offset_slider = 0;
int offset = 0;
float unfocus = 0; //0.30;
int decay = 0; //50;

cv::Mat image;

void tiltShift(){
  cv::Mat gauss_image = cv::Mat::zeros(image.rows, image.cols, CV_32FC3),
          focus_image = cv::Mat::zeros(image.rows, image.cols, CV_32FC3),
          unfocus_image = cv::Mat::zeros(image.rows, image.cols, CV_32FC3),
          focus_mask = cv::Mat(image.rows, image.cols, CV_32FC3, cv::Scalar(0, 0, 0)),
          unfocus_mask = cv::Mat(image.rows, image.cols, CV_32FC3, cv::Scalar(1, 1, 1)),
          output = cv::Mat::zeros(image.rows, image.cols, CV_32FC3);
  cv::Vec3f funcion_output;

  // float unfocus = 0.30;
  // int decay = 50;
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
  cv::GaussianBlur(image, gauss_image, cv::Size(7, 7), 0, 0);

  focus_image = image.mul(focus_mask);
  unfocus_image = gauss_image.mul(unfocus_mask);

  output = focus_image + unfocus_image;

  image.convertTo(image, CV_8UC3);
  output.convertTo(output, CV_8UC3);

  cv::imshow("Tiltshift", output);
  cv::imwrite("../tiltshift.jpg", output);
}

void on_trackbar(int, void*)
{
  unfocus = (float)unfocus_slider / sliderMax;
  decay = decay_slider;
  offset = (offset_slider - unfocus_slider) * image.rows / sliderMax;
  tiltShift();
}

int main(int argc, char **argv)
{
  image = cv::imread(argv[1], cv::IMREAD_COLOR);
  if(!image.data){
    std::cout << "Image not found" << std::endl;
    exit(-1);
  } 

  tiltShift();

  cv::createTrackbar("Unfocus", "Tiltshift",
                      &unfocus_slider,
                      sliderMax,
                      on_trackbar );
  on_trackbar(unfocus_slider, 0 );

  cv::createTrackbar("Decay", "Tiltshift",
                      &decay_slider,
                      sliderMax,
                      on_trackbar );
  on_trackbar(decay_slider, 0);

  cv::createTrackbar("Offset", "Tiltshift",
                      &offset_slider,
                      sliderMax,
                      on_trackbar );
  on_trackbar(offset_slider, 0);

  cv::waitKey(0);

  std::cout << "Image saved as tiltshift.jpg" << std::endl;

  return 0;
}
