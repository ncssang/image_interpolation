#include <opencv2/opencv.hpp>
#include <iostream>



int main()
{
    cv::VideoCapture cap(0);
    if (cap.isOpened() == false)
    {
        std::cout << "Cannot open the video file" << std::endl;
        std::cin.get();
        return -1;
    }
    //Defining window names for above images
    cv::String windowNameOriginalImage = "Original Image";
    cv::String windowNameContrastHigh2 = "Contrast Increased by 2";
    cv::String windowNameContrastHigh4 = "Contrast Increased by 4";
    cv::String windowNameContrastLow0_5 = "Contrast Decreased by 0.5";
    cv::String windowNameContrastLow0_25 = "Contrast Decreased by 0.25";
    //Create and open windows for above images
    namedWindow(windowNameOriginalImage, cv::WINDOW_NORMAL);
    namedWindow(windowNameContrastHigh2, cv::WINDOW_NORMAL);
    namedWindow(windowNameContrastHigh4, cv::WINDOW_NORMAL);
    namedWindow(windowNameContrastLow0_5, cv::WINDOW_NORMAL);
    namedWindow(windowNameContrastLow0_25, cv::WINDOW_NORMAL);
    while (true)
    {
        cv::Mat frame;
        bool bSuccess = cap.read(frame);
        if (bSuccess == false)
        {
            std::cout << "Found the end of the video" << std::endl;
            break;
        }
        cv::Mat frameContrastHigh2;
        frame.convertTo(frameContrastHigh2, -1, 2, 0); //increase the contrast by 2
        cv::Mat frameContrastHigh4;
        frame.convertTo(frameContrastHigh4, -1, 4, 0); //increase the contrast by 4
        cv::Mat frameContrastLow0_5;
        frame.convertTo(frameContrastLow0_5, -1, 0.5, 0); //decrease the contrast by 0.5
        cv::Mat frameContrastLow0_25;
        frame.convertTo(frameContrastLow0_25, -1, 0.25, 0); //decrease the contrast by 0.25
        imshow(windowNameOriginalImage, frame);
        imshow(windowNameContrastHigh2, frameContrastHigh2);
        imshow(windowNameContrastHigh4, frameContrastHigh4);
        imshow(windowNameContrastLow0_5, frameContrastLow0_5);
        imshow(windowNameContrastLow0_25, frameContrastLow0_25);
        if (cv::waitKey(10) == 27)
        {
            std::cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
            break;
        }
    }
    return 0;
}