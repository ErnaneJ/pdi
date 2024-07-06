#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

void swapQuadrants(cv::Mat& image) 
{
    cv::Mat tmp, topLeft, topRight, bottomLeft, bottomRight;

    // Crop the image to the largest even size if the dimensions are odd
    image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));

    int centerX = image.cols / 2;
    int centerY = image.rows / 2;

    // Rearrange the quadrants of the Fourier transform so that the origin is at the center of the image
    // [topLeft topRight]        ->  [bottomRight bottomLeft]
    // [bottomLeft bottomRight]  ->  [topRight topLeft]
    topLeft = image(cv::Rect(0, 0, centerX, centerY));
    topRight = image(cv::Rect(centerX, 0, centerX, centerY));
    bottomLeft = image(cv::Rect(0, centerY, centerX, centerY));
    bottomRight = image(cv::Rect(centerX, centerY, centerX, centerY));

    // Swap quadrants (Top-Left with Bottom-Right)
    topLeft.copyTo(tmp);
    bottomRight.copyTo(topLeft);
    tmp.copyTo(bottomRight);

    // Swap quadrants (Top-Right with Bottom-Left)
    bottomLeft.copyTo(tmp);
    topRight.copyTo(bottomLeft);
    tmp.copyTo(topRight);
}

void createIdealFilter(const cv::Mat& image, cv::Mat& filter) 
{
    cv::Mat_<float> filter2D(image.rows, image.cols);
    int centerX = image.cols / 2;
    int centerY = image.rows / 2;
    int radius = 20;

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (std::pow(i - centerY, 2) + std::pow(j - centerX, 2) <= std::pow(radius, 2)) {
                filter2D.at<float>(i, j) = 1;
            } else {
                filter2D.at<float>(i, j) = 0;
            }
        }
    }

    cv::Mat planes[] = { cv::Mat_<float>(filter2D), cv::Mat::zeros(filter2D.size(), CV_32F) };
    cv::merge(planes, 2, filter);
}

int main(int argc, char** argv) 
{
    cv::Mat image, paddedImage, complexImage;
    std::vector<cv::Mat> planes;

    image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error opening image: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    // Expand the input image to the optimal size for DFT, padding with zeros
    int optimalRows = cv::getOptimalDFTSize(image.rows);
    int optimalCols = cv::getOptimalDFTSize(image.cols); 
    cv::copyMakeBorder(image, paddedImage, 0, optimalRows - image.rows, 0, optimalCols - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // Prepare complex matrix by merging real part (input image) and imaginary part (zeros)
    planes.push_back(cv::Mat_<float>(paddedImage));
    planes.push_back(cv::Mat::zeros(paddedImage.size(), CV_32F));
    cv::merge(planes, complexImage);

    // Compute the DFT
    cv::dft(complexImage, complexImage);
    swapQuadrants(complexImage);

    // Create the ideal filter and apply frequency filtering
    cv::Mat filter;
    createIdealFilter(complexImage, filter);
    cv::mulSpectrums(complexImage, filter, complexImage, 0);

    // Compute the inverse DFT
    swapQuadrants(complexImage);
    cv::idft(complexImage, complexImage);

    // Split the complex image into planes
    cv::split(complexImage, planes);

    // Crop the filtered image to the original size
    cv::Rect roi(0, 0, image.cols, image.rows);
    cv::Mat result = planes[0](roi);

    // Normalize the real part for display
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX);

    cv::imshow("Filtered Image", result);
    cv::imwrite("dft-filter.png", result * 255);

    cv::waitKey();
    return EXIT_SUCCESS;
}