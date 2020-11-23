#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

int main()
{
    // cv::Mat image = cv::imread("index.jpeg");
    // if (image.empty())
    // {
    //     std::cout << "Could not open or find the image" << std::endl;
    //     std::cin.get();
    //     return -1;
    // }
    // cv::String windowName = "The Eye";
    // namedWindow(windowName);
    // imshow(windowName, image);
    // cv::waitKey(0);
    // destroyWindow(windowName);
    // return 0;
    cv::Mat image(600, 800, CV_8UC3, cv::Scalar(255, 0, 0));
    cv::putText(image,
                "Here is some text",
                cv::Point(25, 25),                // Coordinates
                cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                1.0,                            // Scale. 2.0 = 2x bigger
                cv::Scalar(0, 0, 255),      // BGR Color
                1,                              // Line Thickness (Optional)
                cv::LINE_AA);                     // Anti-alias (Optional)
    cv::String windowName = "Window with Blank Image";
    cv::namedWindow(windowName);
    imshow(windowName, image);
    cv::waitKey(0);
    cv::destroyWindow(windowName);
    return 0;
}
