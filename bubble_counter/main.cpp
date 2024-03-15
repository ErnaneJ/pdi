#include <iostream>
#include <opencv2/opencv.hpp>

#define WINDOW_NAME "Bubble Counter"
#define IMAGE_FILE "bubble.png"
#define BACKGROUND_COLOR 0
#define BUBBLE_COLOR 255
#define HOLE_COLOR 100

/**
 * @brief Captures an image from a file.
 * 
 * This function reads an image file specified by IMAGE_FILE macro 
 * and converts it to grayscale.
 * 
 * @return The captured image as a grayscale cv::Mat.
 **/
cv::Mat captureImage()
{
  cv::Mat image = cv::imread(IMAGE_FILE, cv::IMREAD_GRAYSCALE);
  if (image.empty())
  {
    std::cerr << "\n❌ Bubble Counter - Error: Image (" << IMAGE_FILE << ") not found.\n" << std::endl;
    exit(1);
  }
  return image;
}

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
}

/**
 * @brief Handles bubbles at the edges of the image.
 * 
 * This function fills bubbles found at the edges of the image with
 * the specified background color.
 * 
 * @param image The image containing bubbles.
 * @param backgroundColor The color to fill bubbles with.
 **/
void handleEdgeBubbles(cv::Mat &image, int backgroundColor = BACKGROUND_COLOR)
{
  for (int row = 0; row < image.rows; row++)
  {
    for (int column = 0; column < image.cols; column++)
    {
      bool isEdge = row == 0 || row == image.rows - 1 || column == 0 || column == image.cols - 1;
      bool isBubble = image.at<uchar>(row, column) != backgroundColor;

      if (isEdge && isBubble)
        cv::floodFill(image, cv::Point(column, row), backgroundColor);
    }
  }
}

/**
 * @brief Counts bubbles without holes in the image.
 * 
 * This function counts bubbles without any holes in the image.
 * 
 * @param image The image containing bubbles.
 * @return The count of bubbles without holes.
 **/
int countsBubblesWithoutHoles(cv::Mat image)
{
  int bubblesWithoutHoles = 0;

  for (int row = 0; row < image.rows; row++)
  {
    for (int column = 0; column < image.cols; column++)
    {
      if (image.at<uchar>(row, column) == BUBBLE_COLOR)
      {
        cv::floodFill(image, cv::Point(column, row), BACKGROUND_COLOR);
        bubblesWithoutHoles++;
      }
    }
  }

  return bubblesWithoutHoles;
}

/**
 * @brief Counts bubbles with holes in the image.
 * 
 * This function counts bubbles with holes and the total number of holes 
 * present in the image. It distinguishes bubbles and holes by flood-filling with different colors.
 * 
 * @param image The image containing bubbles and holes.
 * @return A vector containing the count of bubbles with holes and the total count of holes.
**/
std::vector<int> countsBubblesWithHoles(cv::Mat image)
{
  int bubblesWithHoles = 0, holes = 0;

  cv::floodFill(image, cv::Point(0, 0), HOLE_COLOR);

  for (int row = 0; row < image.rows; row++)
  {
    for (int column = 0; column < image.cols; column++)
    {
      if (image.at<uchar>(row, column) == BACKGROUND_COLOR)
      {
        if (image.at<uchar>(row - 1, column) == BUBBLE_COLOR)
        {
          cv::floodFill(image, cv::Point(column, row - 1), BACKGROUND_COLOR);
          bubblesWithHoles++;
        }
        holes++;
        cv::floodFill(image, cv::Point(column, row), HOLE_COLOR);
      }
    }
  }

  cv::floodFill(image, cv::Point(0, 0), BACKGROUND_COLOR);

  return { bubblesWithHoles, holes };
}

/**
* @brief The main function.
*
* This function orchestrates the entire bubble counting process. It captures the image, removes bubbles at the edges, 
* counts bubbles with and without holes, and displays the results.
*
* @param argc The number of command-line arguments.
* @param argv The array of command-line arguments.
* @return An integer indicating the exit status.
***/
int main(int, char **)
{
  cv::Mat image = captureImage();

  showImage(image, "Bubbles - Original", 0);

  handleEdgeBubbles(image);
  showImage(image, "Bubbles - Edges Removed", 1);

  std::vector<int> holesInfo = countsBubblesWithHoles(image);
  int bubblesWithHoles = holesInfo[0];
  int holes = holesInfo[1];
  showImage(image, "Bubbles - Holes counted", 2);

  int bubblesWithoutHoles = countsBubblesWithoutHoles(image);
  showImage(image, "Bubbles - Complete Count", 3);

  std::cout << "┌──────────────────────────────────────────────┐" << std::endl;
  std::cout << "│          Bubble Counter - "<< IMAGE_FILE << std::endl;
  std::cout << "│──────────────────────────────────────────────┤" << std::endl;
  std::cout << "│✅ Bubbles with holes: " << bubblesWithHoles << std::endl;
  std::cout << "│✅ Bubbles without holes: " << bubblesWithoutHoles << std::endl;
  std::cout << "│✅ Total of bubbles: " << bubblesWithHoles + bubblesWithoutHoles <<  std::endl;
  std::cout << "│✅ Holes: " << holes << std::endl;
  std::cout << "│ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┤" << std::endl;
  std::cout << "│🎓 Sudent: Ernane Ferreira - DCA0445" << std::endl;
  std::cout << "└──────────────────────────────────────────────┘" << std::endl;

  cv::waitKey(0);

  return 0;
};