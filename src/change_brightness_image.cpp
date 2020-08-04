#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::Mat image = cv::imread("/home/nq/workspace/image_interpolation/MyImage.jpg");
    if (image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
        return -1;
    }
    cv::Mat imageBrighnessHigh50;
    image.convertTo(imageBrighnessHigh50, -1, 1, 50); //increase the brightness by 50
    cv::Mat imageBrighnessHigh100;
    image.convertTo(imageBrighnessHigh100, -1, 1, 100); //increase the brightness by 100
    cv::Mat imageBrighnessLow50;
    image.convertTo(imageBrighnessLow50, -1, 1, -50); //decrease the brightness by 50
    cv::Mat imageBrighnessLow100;
    image.convertTo(imageBrighnessLow100, -1, 1, -100); //decrease the brightness by 100
    cv::String windowNameOriginalImage = "Original Image";
    cv::String windowNameBrightnessHigh50 = "Brightness Increased by 50";
    cv::String windowNameWithBrightnessHigh100 = "Brightness Increased by 100";
    cv::String windowNameBrightnessLow50 = "Brightness Decreased by 50";
    cv::String windowNameBrightnessLow100 = "Brightness Decreased by 100";
    namedWindow(windowNameOriginalImage, cv::WINDOW_NORMAL);
    namedWindow(windowNameBrightnessHigh50, cv::WINDOW_NORMAL);
    namedWindow(windowNameWithBrightnessHigh100, cv::WINDOW_NORMAL);
    namedWindow(windowNameBrightnessLow50, cv::WINDOW_NORMAL);
    namedWindow(windowNameBrightnessLow100, cv::WINDOW_NORMAL);
    imshow(windowNameOriginalImage, image);
    imshow(windowNameBrightnessHigh50, imageBrighnessHigh50);
    imshow(windowNameWithBrightnessHigh100, imageBrighnessHigh100);
    imshow(windowNameBrightnessLow50, imageBrighnessLow50);
    imshow(windowNameBrightnessLow100, imageBrighnessLow100);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}