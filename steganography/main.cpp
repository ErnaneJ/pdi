/**
 * @file steganography.cpp
 * @brief Implementation of steganography algorithms to encode and decode hidden information within images.
 */

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#define N_BITS 3
#define OUTPUT_PATH "./assets/output/"

/**
 * @brief Encodes a hidden image into a carrier image using steganography.
 */
void encode()
{
  std::cout << "\n-------------------------------------------------------\n"
            << std::endl;
  std::cout << "🔐 Encode 🔐\n " << std::endl;

  cv::Mat carrierImage, hiddenImage, finalImage;
  cv::Vec3b carrierVal, hiddenVal, finalVal;

  std::cout << "👉 Enter the path of the carrier image: ";
  std::string carrierImagePath;
  std::cin >> carrierImagePath;

  std::cout << "👉 Enter the path of the hidden image: ";
  std::string hiddenImagePath;
  std::cin >> hiddenImagePath;

  carrierImage = cv::imread(carrierImagePath, cv::IMREAD_COLOR);
  hiddenImage = cv::imread(hiddenImagePath, cv::IMREAD_COLOR);

  if (carrierImage.empty() || hiddenImage.empty())
  {
    std::cout << "❌ Images did not load correctly" << std::endl;
    exit(-1);
  }

  if (carrierImage.rows != hiddenImage.rows ||
      carrierImage.cols != hiddenImage.cols)
  {
    std::cout << "❌ Images must have the same size" << std::endl;
    exit(-1);
  }

  std::cout << "\n✅ Images loaded successfully!" << std::endl;
  std::cout << "\n🚀 Encoding....." << std::endl;

  finalImage = carrierImage.clone();

  for (int i = 0; i < carrierImage.rows; i++)
  {
    for (int j = 0; j < carrierImage.cols; j++)
    {
      carrierVal = carrierImage.at<cv::Vec3b>(i, j);
      hiddenVal = hiddenImage.at<cv::Vec3b>(i, j);

      carrierVal[0] = carrierVal[0] >> N_BITS << N_BITS;
      carrierVal[1] = carrierVal[1] >> N_BITS << N_BITS;
      carrierVal[2] = carrierVal[2] >> N_BITS << N_BITS;

      hiddenVal[0] = hiddenVal[0] >> (8 - N_BITS);
      hiddenVal[1] = hiddenVal[1] >> (8 - N_BITS);
      hiddenVal[2] = hiddenVal[2] >> (8 - N_BITS);

      finalVal[0] = carrierVal[0] | hiddenVal[0];
      finalVal[1] = carrierVal[1] | hiddenVal[1];
      finalVal[2] = carrierVal[2] | hiddenVal[2];

      finalImage.at<cv::Vec3b>(i, j) = finalVal;
    }
  }

  cv::imwrite(std::string(OUTPUT_PATH) + "encode-steganography.png", finalImage);
  std::cout << "\n🎉 Image encoded successfully!" << std::endl;
  std::cout << "\nℹ️  Image 2 (" << hiddenImagePath
            << ") was stored in the least significant bits of image 1 (" << carrierImagePath
            << ").\nℹ️  The result can be viewed in the output folder (" << std::string(OUTPUT_PATH) + "encode-steganography.png"
            << ")." << std::endl;

  std::cout << "\n-------------------------------------------------------" << std::endl;
}

/**
 * @brief Decodes a hidden image from an encoded image using steganography.
 */
void decode()
{
  cv::Mat hiddenImage, finalImage;
  cv::Vec3b hiddenVal, finalVal;

  std::cout << "\n-------------------------------------------------------\n"
            << std::endl;
  std::cout << "🔓 Decode 🔓\n " << std::endl;

  std::cout << "👉 Enter the encoded image path: ";
  std::string encodedPath;
  std::cin >> encodedPath;

  finalImage = cv::imread(encodedPath, cv::IMREAD_COLOR);

  if (finalImage.empty())
  {
    std::cout << "❌ Image did not load correctly" << std::endl;
    exit(-1);
  }

  std::cout << "\n✅ Image loaded successfully!" << std::endl;

  hiddenImage = finalImage.clone();

  for (int i = 0; i < finalImage.rows; i++)
  {
    for (int j = 0; j < finalImage.cols; j++)
    {
      finalVal = finalImage.at<cv::Vec3b>(i, j);
      hiddenVal = hiddenImage.at<cv::Vec3b>(i, j);

      for (int color = 0; color < 3; color++)
        hiddenVal[color] = (finalVal[color] & ((1 << N_BITS) - 1)) << (8 - N_BITS);

      hiddenImage.at<cv::Vec3b>(i, j) = hiddenVal;
    }
  }

  imwrite(std::string(OUTPUT_PATH) + "decode-steganography.png", hiddenImage);
  std::cout << "\n🎉 Image decoded successfully!" << std::endl;
  std::cout << "\nℹ️  Selected image (" << encodedPath
            << ") has been decoded."
            << "\nℹ️  The result can be viewed in the output folder (" << std::string(OUTPUT_PATH) + "decode-steganography.png"
            << ")." << std::endl;
  std::cout << "\n-------------------------------------------------------" << std::endl;
}

/**
 * @brief Separates the bit planes of an image for visualization.
 */
void bitPlane()
{
  cv::Mat finalImage, totalImage, images[8];
  cv::Vec3b finalVal;

  std::cout << "\n-------------------------------------------------------\n"
            << std::endl;
  std::cout << "🖼️ Bit Plane 🖼️\n " << std::endl;

  std::cout << "👉 Enter the path of the image: ";
  std::string imagePath;
  std::cin >> imagePath;

  finalImage = cv::imread(imagePath, cv::IMREAD_COLOR);

  if (finalImage.empty())
  {
    std::cout << "❌ Image did not load correctly" << std::endl;
    exit(-1);
  }

  std::cout << "\n✅ Image loaded successfully!" << std::endl;

  std::cout << "\n🚀 Separating bit planes....." << std::endl;
  std::cout << "\nℹ️  The image will be displayed in 8 windows, each one showing a different bit plane." << std::endl;

  std::cout << "\n🖼️  Showing plans from LSB to MSB.\n"
            << std::endl;
  for (int plane = 0; plane < 8; plane++)
  {
    images[plane] = finalImage.clone();
    for (int i = 0; i < images[plane].rows; i++)
    {
      for (int j = 0; j < images[plane].cols; j++)
      {
        finalVal = images[plane].at<cv::Vec3b>(i, j);
        finalVal[0] = finalVal[0] & (1 << (sizeof(finalVal[0]) * 8 - plane)) - 1;
        finalVal[1] = finalVal[1] & (1 << (sizeof(finalVal[1]) * 8 - plane)) - 1;
        finalVal[2] = finalVal[2] & (1 << (sizeof(finalVal[2]) * 8 - plane)) - 1;

        finalVal[0] = finalVal[0] << plane;
        finalVal[1] = finalVal[1] << plane;
        finalVal[2] = finalVal[2] << plane;

        images[plane].at<cv::Vec3b>(i, j) = finalVal;
      }
    }
    cv::imshow("Plane " + std::to_string(plane) + " displayed", images[plane]);
    std::cout << "ℹ️  Plane " << plane << " displayed. Press any key to continue." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
  }

  std::cout << "\n🎉 Finished displaying bit planes." << std::endl;

  std::cout << "\n-------------------------------------------------------" << std::endl;
}

/**
 * @brief Main function to run the steganography program.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Exit status.
 */
int main(int argc, char **argv)
{
  int menuOption;
  std::cout << "\nWelcome to the Menu! 🌟\n"
            << std::endl;
  std::cout << "1️⃣  → 🔐 Encode: Indicate the path of two images and obtain as output the second image hidden inside the first." << std::endl;
  std::cout << "2️⃣  → 🔓 Decode: Indicate the path of an image and obtain as output the image hidden inside it." << std::endl;
  std::cout << "3️⃣  → 🖼️  Bit Plane: Indicate the path of an image and obtain as output the image with the bit planes separated." << std::endl;
  std::cout << "4️⃣  → ❌ Exit: Close the program." << std::endl;
  std::cout << "\nWhat would you like to do? ";
  std::cin >> menuOption;

  switch (menuOption)
  {
  case 1:
    encode();
    break;
  case 2:
    decode();
    break;
  case 3:
    bitPlane();
    break;
  case 4:
    std::cout << "\nGoodbye! 👋\n" << std::endl;
    return 0;
  default:
    std::cout << "\n❌ Invalid option. Please try again. 👇🏼\n" << std::endl;
  }

  main(argc, argv);
}