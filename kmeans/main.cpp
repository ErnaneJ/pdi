#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

#define NUM_CLUSTERS 8
#define NUM_ITERATIONS 1 // 5 (default)
#define NUM_RUNS 10      // 1 (default)

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: kmeans input.jpg\n";
    return EXIT_FAILURE;
  }

  cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
  if (img.empty())
  {
    std::cout << "Could not open or find the image: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  cv::Mat samples(img.rows * img.cols, 3, CV_32F);
  for (int y = 0; y < img.rows; ++y)
  {
    for (int x = 0; x < img.cols; ++x)
    {
      for (int z = 0; z < 3; ++z)
      {
        samples.at<float>(y + x * img.rows, z) = img.at<cv::Vec3b>(y, x)[z];
      }
    }
  }

  for (int run = 0; run < NUM_RUNS; ++run)
  {
    cv::Mat labels, centers;
    cv::kmeans(samples, NUM_CLUSTERS, labels,
               cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10000, 0.0001),
               NUM_ITERATIONS, cv::KMEANS_RANDOM_CENTERS, centers);

    cv::Mat labeledImage(img.size(), img.type());
    for (int y = 0; y < img.rows; ++y)
    {
      for (int x = 0; x < img.cols; ++x)
      {
        int index = labels.at<int>(y + x * img.rows, 0);
        labeledImage.at<cv::Vec3b>(y, x)[0] = static_cast<uchar>(centers.at<float>(index, 0));
        labeledImage.at<cv::Vec3b>(y, x)[1] = static_cast<uchar>(centers.at<float>(index, 1));
        labeledImage.at<cv::Vec3b>(y, x)[2] = static_cast<uchar>(centers.at<float>(index, 2));
      }
    }

    std::ostringstream filename;
    filename << "../assets/sushi-kmeans-" << run + 1 << ".jpg";
    cv::imwrite(filename.str(), labeledImage);
    cv::imshow("kmeans", labeledImage);
    cv::waitKey();
  }

  return EXIT_SUCCESS;
}