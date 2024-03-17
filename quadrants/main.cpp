#include <iostream>
#include <opencv2/opencv.hpp>

#define WINDOW_NAME "Quadrants"
#define IMAGE_FILE "biel.png"

/**
 * @brief Captures an image from a file.
 *
 * This function reads an image file specified by the IMAGE_FILE macro
 * and converts it to grayscale.
 *
 * @return The captured image as a grayscale cv::Mat.
 **/
cv::Mat captureImage()
{
  cv::Mat image = cv::imread(IMAGE_FILE, cv::IMREAD_GRAYSCALE);
  if (image.empty())
  {
    std::cerr << "\n❌ Negative - Error: Image (" << IMAGE_FILE << ") not found.\n"
              << std::endl;
    exit(1);
  }
  return image;
};

/**
 * @brief Displays an image in a named window.
 *
 * This function displays the specified image in a named window.
 *
 * @param image The image to be displayed.
 * @param windowName The name of the window.
 * @param index The index for organizing multiple windows.
 **/
void showImage(cv::Mat image, std::string windowName = WINDOW_NAME, int index = 0)
{
  cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
  cv::imshow(windowName, image);

  cv::Rect windowRect = cv::getWindowImageRect(windowName);
  cv::moveWindow(windowName, ((windowRect.width + 30) * index + 30), 0);
};

/**
 * @brief Divides the image into quadrants.
 *
 * This function divides the given image into four quadrants.
 *
 * @param image The original image.
 * @param quadrants An array to store the four quadrants.
 **/
void getQuadrants(cv::Mat image, cv::Mat quadrants[4])
{
  quadrants[0] = image(cv::Rect(0, 0, image.cols / 2, image.rows / 2));
  quadrants[1] = image(cv::Rect(image.cols / 2, 0, image.cols / 2, image.rows / 2));
  quadrants[2] = image(cv::Rect(0, image.rows / 2, image.cols / 2, image.rows / 2));
  quadrants[3] = image(cv::Rect(image.cols / 2, image.rows / 2, image.cols / 2, image.rows / 2));
};

/**
 * @brief Selects new quadrants from the given quadrants.
 *
 * This function allows the user to select new quadrants from the given quadrants.
 *
 * @param quadrants The original quadrants.
 * @param newQuadrants An array to store the new quadrants selected by the user.
 **/
void getNewQuadrants(cv::Mat quadrants[4], cv::Mat newQuadrants[4])
{
  int userIndex;
  for (int index = 0; index < 4; index++)
  {
    std::cout << "📍 Which quadrant do you want to replace " << index + 1 << "º (1-4)? ";
    std::cin >> userIndex;

    userIndex--;
    if (userIndex < 0 || userIndex > 3)
    {
      std::cerr << "\n❌ Quadrants - Error: Invalid quadrant index. Please, try again with a number between 1 and 4."
                << std::endl;
      exit(1);
    }

    newQuadrants[index] = quadrants[userIndex];
  }
};

/**
 * @brief Mounts a new image from the given quadrants.
 *
 * This function concatenates the given quadrants to form a new image.
 *
 * @param newQuadrants The quadrants to be concatenated.
 * @return The new image formed from the quadrants.
 **/
cv::Mat mountNewImage(cv::Mat newQuadrants[4])
{
  cv::Mat newImage, quadrantsTop, quadrantsBottom;
  cv::hconcat(newQuadrants[0], newQuadrants[1], quadrantsTop);
  cv::hconcat(newQuadrants[2], newQuadrants[3], quadrantsBottom);
  cv::vconcat(quadrantsTop, quadrantsBottom, newImage);

  return newImage;
};

/**
 * @brief Main function.
 *
 * This function demonstrates the use of the above functions to manipulate image quadrants.
 **/
int main(int, char **)
{
  std::cout << "\n🖼️  Quadrants - [" << IMAGE_FILE << "] 🖼️\n"
            << std::endl;
  std::cout << "──────────────────────────────────────────────\n"
            << std::endl;

  // Capture original image
  cv::Mat image = captureImage();

  // Divide original image into quadrants
  cv::Mat quadrants[4];
  getQuadrants(image, quadrants);

  // Allow user to select new quadrants
  cv::Mat newQuadrants[4];
  getNewQuadrants(quadrants, newQuadrants);

  // Combine selected quadrants to form new image
  cv::Mat newImage = mountNewImage(newQuadrants);

  // Prompt user to choose whether to display quadrants individually
  std::string userOption = "n";
  std::cout << "\n💡 Do you want to show the quadrants individually (y/n)? ";
  std::cin >> userOption;

  // Determine if quadrants should be displayed individually
  bool showQuadrants = userOption == "y" || userOption == "Y";
  showImage(image, "Biel - Original", 0);

  // Display original image and/or quadrants
  if (showQuadrants)
  {
    showImage(quadrants[0], "Biel - Quadrant 1", 2);
    showImage(quadrants[1], "Biel - Quadrant 2", 3);
    showImage(quadrants[2], "Biel - Quadrant 3", 4);
    showImage(quadrants[3], "Biel - Quadrant 4", 5);
  }
  showImage(newImage, "Biel - Quadrants", showQuadrants ? 4 : 1);

  std::cout << "\n✅ Quadrants successfully manipulated!" 
            << std::endl;

  std::cout << "\n──────────────────────────────────────────────" 
            << std::endl;
  std::cout << "\n🎓 Sudent: Ernane Ferreira - DCA0445" 
            << std::endl;

  cv::waitKey(0);

  return 0;
};