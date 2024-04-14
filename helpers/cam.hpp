#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

#define LIMIT_CAMERAS 5

/**
 * @brief Enumerate available cameras and allow the user to select one.
 *
 * This function enumerates available cameras and allows the user to select one
 * to activate.
 *
 * @return The index of the selected camera.
 */
int cameraEnumerator()
{
  std::vector<int> cameras;

  cv::VideoCapture cap;

  cameras.clear();

  for (int index = 0; index < LIMIT_CAMERAS; index++)
  {
    cap.open(index);

    if (cap.isOpened())
      cameras.push_back(index);

    cap.release();
  }

  if (cameras.size() == 1)
    return cameras[0];

  if (cameras.empty())
  {
    std::cout << "No cameras available." << std::endl;
    exit(1);
  }

  std::cout << "--------------------\n";
  for (size_t i = 0; i < cameras.size(); i++)
  {
    std::cout << "Camera [" << cameras[i] << "] is active." << std::endl;
  }
  std::cout << "--------------------\n";

  int selectedCamera = 0;
  std::cout << "Select the camera to activate: ";
  std::cin >> selectedCamera;

  return selectedCamera;
}
