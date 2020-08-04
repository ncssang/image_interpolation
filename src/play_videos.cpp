#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

// using namespace cv;

int main()
{
    cv::VideoCapture cap(0);
    if (cap.isOpened() == false)
    {
        std::cout << "Cannot open the video file" << std::endl;
        std::cin.get();
        return -1;
    }
    double fps = cap.get(cv::CAP_PROP_FPS);
    std::cout << "Frames per seconds : " << fps << std::endl;
    cv::String window_name = "My First Video";
    namedWindow(window_name, cv::WINDOW_NORMAL);
    int frame_count = 0;
    while (true)
    {
        cv::Mat frame;
        bool bSuccess = cap.read(frame);
        if (bSuccess == false)
        {
            std::cout << "Found the end of the video" << std::endl;
            break;
        }
        ++frame_count;
        std::string ss_frame_count = std::to_string(frame_count);
        cv::putText(frame, ss_frame_count, cv::Point(25, 25), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 0, 255), 1, CV_AA);
        cv::Size sz = frame.size();
        int imageWith = sz.width;
        std::string image_With = std::to_string(imageWith);
        int imageHigh = sz.height;
        std::string image_High = std::to_string(imageHigh);
        std::string multiple = "x";
        std::string with_high = image_With + multiple + image_High;
       cv::putText(frame, with_high, cv::Point(imageWith - 100, imageHigh - 40), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 0, 255), 1, CV_AA);
        imshow(window_name, frame);
        if (cv::waitKey(10) == 27)
        {
            std::cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
            break;
        }
    }
    return 0;
}