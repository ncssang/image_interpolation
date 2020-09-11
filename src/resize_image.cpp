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
    cv::Mat input_image = cv::imread("00.png", 0);
    if (input_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
        return -1;
    }
    float ratio;
    do
    {
        std::cout << "Nhap ratio: " << std::endl;
        std::cin >> ratio;
    } while (ratio <= 0);
    cv::Mat output_image(cv::Size(input_image.cols * ratio, input_image.rows * ratio), input_image.type());
    float actual_ratio = (float)output_image.cols / input_image.cols;
    std::cout << "actual_ratio: " << actual_ratio << std::endl;
    for (int col_output = 0; col_output < output_image.cols; ++col_output)
    {
        int col_input = (int)std::floor((float)(col_output + 0.5) / actual_ratio);
        // int col_input = (int)std::floor((float)((col_output + 0.5) * input_image.cols) / output_image.cols);
        for (int row_output = 0; row_output < output_image.rows; ++row_output)
        {
            int row_input = (int)std::floor((float)(row_output + 0.5) / actual_ratio);
            // int row_input = (int)std::floor((float)((row_output + 0.5) * input_image.rows) / output_image.rows);
            if (input_image.type() == CV_8UC1)
            {
                output_image.at<uchar>(cv::Point(col_output, row_output)) = input_image.at<uchar>(cv::Point(col_input, row_input));
            }
            else if (input_image.type() == CV_8UC3)
            {
                output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = input_image.at<cv::Vec3b>(cv::Point(col_input, row_input));
            }
        }
    }
    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);

    cv::waitKey(0);
    return 0;
}