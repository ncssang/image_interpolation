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
    cv::Mat input_image = cv::imread("Histogram_Calculation_Original_Image.jpg", 0);
    if (input_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
        return -1;
    }
    std::cout << "out_image.cols = " << input_image.cols << "; out_image.rows = " << input_image.rows << std::endl;
    float ratio;
    do
    {
        std::cout << "Nhap ratio: " << std::endl;
        std::cin >> ratio;
    } while (ratio <= 0);

    cv::Mat output_image(cv::Size(input_image.cols * ratio, input_image.rows * ratio), input_image.type());
    float actual_ratio = (float)output_image.cols / input_image.cols;
    for (int col = 0; col < output_image.cols; ++col)
    {
        for (int row = 0; row < output_image.rows; ++row)
        {
            output_image.at<uchar>(cv::Point(col, row)) = input_image.at<uchar>(cv::Point((int)std::round((col + 1) / actual_ratio) - 1, (int)std::round((row + 1) / actual_ratio) - 1));
        }
    }
    std::cout << "out_image.cols = " << output_image.cols << "; out_image.rows = " << output_image.rows << std::endl;
    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);

    cv::waitKey(0);
    return 0;
}