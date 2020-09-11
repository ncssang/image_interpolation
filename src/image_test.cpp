#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <vector>

int main()
{
    cv::Mat image_test(cv::Size(3, 3), CV_8UC1);
    image_test.at<uchar>(cv::Point(0, 0)) = 100;
    image_test.at<uchar>(cv::Point(1, 0)) = 40;
    image_test.at<uchar>(cv::Point(2, 0)) = 220;
    image_test.at<uchar>(cv::Point(0, 1)) = 20;
    image_test.at<uchar>(cv::Point(1, 1)) = 180;
    image_test.at<uchar>(cv::Point(2, 1)) = 70;
    image_test.at<uchar>(cv::Point(0, 2)) = 90;
    image_test.at<uchar>(cv::Point(1, 2)) = 140;
    image_test.at<uchar>(cv::Point(2, 2)) = 125;
    imshow("image_test", image_test);
    cv::imwrite("image_test.png", image_test);
    cv::waitKey(0);
    return 0;
}