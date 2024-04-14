/**
 * @brief Calculate and visualize histograms for each color channel.
 *
 * This function calculates and visualizes histograms for each color channel
 * (Red, Green, and Blue) of the input image.
 *
 * @param image The input image.
 * @param histImgR The histogram image for the Red channel.
 * @param histImgG The histogram image for the Green channel.
 * @param histImgB The histogram image for the Blue channel.
 * @param histR The histogram for the Red channel.
 * @param histG The histogram for the Green channel.
 * @param histB The histogram for the Blue channel.
 * @param nbins Number of bins for histogram calculation.
 */
void calculateAndVisualizeRGBHistograms(const cv::Mat &image, cv::Mat &histImgR, cv::Mat &histImgG, cv::Mat &histImgB, cv::Mat &histR, cv::Mat &histG, cv::Mat &histB, int nbins)
{
  std::vector<cv::Mat> planes;
  cv::split(image, planes);

  const float range[] = {0, 255};
  const float *histrange = {range};
  bool uniform = true;
  bool accumulate = false;

  cv::calcHist(&planes[0], 1, 0, cv::Mat(), histB, 1, &nbins, &histrange, uniform, accumulate);
  cv::calcHist(&planes[1], 1, 0, cv::Mat(), histG, 1, &nbins, &histrange, uniform, accumulate);
  cv::calcHist(&planes[2], 1, 0, cv::Mat(), histR, 1, &nbins, &histrange, uniform, accumulate);

  cv::normalize(histR, histR, 0, histImgR.rows, cv::NORM_MINMAX, -1, cv::Mat());
  cv::normalize(histG, histG, 0, histImgG.rows, cv::NORM_MINMAX, -1, cv::Mat());
  cv::normalize(histB, histB, 0, histImgB.rows, cv::NORM_MINMAX, -1, cv::Mat());

  histImgR.setTo(cv::Scalar(0));
  histImgG.setTo(cv::Scalar(0));
  histImgB.setTo(cv::Scalar(0));

  for (int i = 0; i < nbins; i++)
  {
    cv::line(histImgR, cv::Point(i, histImgR.rows),
             cv::Point(i, histImgR.rows - cvRound(histR.at<float>(i))),
             cv::Scalar(0, 0, 255), 1, 8, 0);
    cv::line(histImgG, cv::Point(i, histImgG.rows),
             cv::Point(i, histImgG.rows - cvRound(histG.at<float>(i))),
             cv::Scalar(0, 255, 0), 1, 8, 0);
    cv::line(histImgB, cv::Point(i, histImgB.rows),
             cv::Point(i, histImgB.rows - cvRound(histB.at<float>(i))),
             cv::Scalar(255, 0, 0), 1, 8, 0);
  }
}

/**
 * @brief Calculate and visualize histogram for a grayscale image.
 *
 * This function calculates and visualizes histogram for a grayscale image.
 *
 * @param image The input image.
 * @param histImg The histogram image.
 * @param hist The histogram.
 * @param nbins Number of bins for histogram calculation.
 */
void calculateAndVisualizeMonoHistograms(const cv::Mat &image, cv::Mat &histImg, cv::Mat &hist, int nbins)
{
  std::vector<cv::Mat> planes;
  cv::split(image, planes);

  const float range[] = {0, 255};
  const float *histrange = {range};
  bool uniform = true;
  bool accumulate = false;

  cv::calcHist(&planes[0], 1, 0, cv::Mat(), hist, 1, &nbins, &histrange, uniform, accumulate);

  cv::normalize(hist, hist, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());

  histImg.setTo(cv::Scalar(0));

  for (int i = 0; i < nbins; i++)
  {
    cv::line(histImg, cv::Point(i, histImg.rows),
             cv::Point(i, histImg.rows - cvRound(hist.at<float>(i))),
             cv::Scalar(255, 255, 255), 1, 8, 0);
  }
}
