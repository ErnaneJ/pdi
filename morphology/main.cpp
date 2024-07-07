#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::Mat image, erosion, dilation, opening, closing, open_close;
    cv::Mat structuring_element;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <input_image>" << std::endl;
        return -1;
    }

    // Load the input image
    image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

    if(image.empty()) {
        std::cout << "Error loading image: " << argv[1] << std::endl;
        return -1;
    }

    // Structuring element (kernel) for morphological operations
    structuring_element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    // Erosion
    cv::erode(image, erosion, structuring_element);

    // Dilation
    cv::dilate(image, dilation, structuring_element);

    // Opening (erosion followed by dilation)
    cv::morphologyEx(image, opening, cv::MORPH_OPEN, structuring_element);

    // Closing (dilation followed by erosion)
    cv::morphologyEx(image, closing, cv::MORPH_CLOSE, structuring_element);

    // Opening followed by closing
    cv::morphologyEx(opening, open_close, cv::MORPH_CLOSE, structuring_element);

    // Concatenate the results horizontally for visualization
    cv::imwrite("../assets/object-erosion-operation.png", erosion);
    cv::imwrite("../assets/object-dilation-operation.png", dilation);
    cv::imwrite("../assets/object-opening-operation.png", opening);
    cv::imwrite("../assets/object-closing-operation.png", closing);
    cv::imwrite("../assets/object-open-close-operation.png", open_close);

    cv::Mat images_array[] = {erosion, dilation, opening, closing, open_close};
    cv::hconcat(images_array, 5, image);

    // Display the morphological operations result
    cv::imshow("Morphology Operations", image);
    cv::waitKey();

    return 0;
}
