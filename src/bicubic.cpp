#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <vector>

int get_value(int point_0, int point_1, int point_2, int point_3, float x)
{
    float a = -0.5 * point_0 + 1.5 * point_1 - 1.5 * point_2 + 0.5 * point_3;
    float b = point_0 - 2.5 * point_1 + 2 * point_2 - 0.5 * point_3;
    float c = -0.5 * point_0 + 0.5 * point_2;
    float d = point_1;
    int value = (int)std::floor(d + x * (c + x * (b + x * a)));
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 255)
    {
        value = 255;
    }
    return value;
}

int fx_interpolation(cv::Mat input_image, int col_input_0, int col_input_1, int col_input_2, int col_input_3, int row_input_0, int row_input_1, int row_input_2, int row_input_3, float d_x, float d_y)
{
    int fx00 = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
    int fx01 = input_image.at<uchar>(cv::Point(col_input_0, row_input_1));
    int fx02 = input_image.at<uchar>(cv::Point(col_input_0, row_input_2));
    int fx03 = input_image.at<uchar>(cv::Point(col_input_0, row_input_3));

    int fx10 = input_image.at<uchar>(cv::Point(col_input_1, row_input_0));
    int fx11 = input_image.at<uchar>(cv::Point(col_input_1, row_input_1));
    int fx12 = input_image.at<uchar>(cv::Point(col_input_1, row_input_2));
    int fx13 = input_image.at<uchar>(cv::Point(col_input_1, row_input_3));

    int fx20 = input_image.at<uchar>(cv::Point(col_input_2, row_input_0));
    int fx21 = input_image.at<uchar>(cv::Point(col_input_2, row_input_1));
    int fx22 = input_image.at<uchar>(cv::Point(col_input_2, row_input_2));
    int fx23 = input_image.at<uchar>(cv::Point(col_input_2, row_input_3));

    int fx30 = input_image.at<uchar>(cv::Point(col_input_3, row_input_0));
    int fx31 = input_image.at<uchar>(cv::Point(col_input_3, row_input_1));
    int fx32 = input_image.at<uchar>(cv::Point(col_input_3, row_input_2));
    int fx33 = input_image.at<uchar>(cv::Point(col_input_3, row_input_3));

    int x_y_0 = get_value(fx00, fx01, fx02, fx03, d_y);
    int x_y_1 = get_value(fx10, fx11, fx12, fx13, d_y);
    int x_y_2 = get_value(fx20, fx21, fx22, fx23, d_y);
    int x_y_3 = get_value(fx30, fx31, fx32, fx33, d_y);
    int fx = get_value(x_y_0, x_y_1, x_y_2, x_y_3, d_x);
    return fx;
}

void resize_bicubic(cv::Mat input_image, cv::Mat output_image)
{
    float actual_ratio = (float)output_image.cols / input_image.cols;
    for (int col_output = 0; col_output < output_image.cols; ++col_output)
    {
        for (int row_output = 0; row_output < output_image.rows; ++row_output)
        {
            float col_input = (col_output + 0.5) / actual_ratio;
            int col_input_0, col_input_1, col_input_2, col_input_3;
            float d_x;
            if (col_input < 0.5)
            {
                col_input = 0.5;
            }
            if (col_input > input_image.cols - 0.5)
            {
                col_input = input_image.cols - 0.5;
            }
            if (col_input < 1.5)
            {
                col_input_0 = 0;
                col_input_1 = 0;
                col_input_2 = 1;
                col_input_3 = 2;
            }
            else if (col_input >= 1.5 && col_input < input_image.cols - 1.5)
            {
                col_input_0 = (int)std::floor(col_input - 1.5);
                col_input_1 = col_input_0 + 1;
                col_input_2 = col_input_0 + 2;
                col_input_3 = col_input_0 + 3;
            }
            else if (col_input >= input_image.cols - 1.5)
            {
                col_input_0 = input_image.cols - 3;
                col_input_1 = col_input_0 + 1;
                col_input_2 = col_input_0 + 2;
                col_input_3 = col_input_0 + 2;
            }
            d_x = col_input - (col_input_1 + 0.5);

            float row_input = (row_output + 0.5) / actual_ratio;
            std::cout << "row_input: " << row_input << std::endl;
            int row_input_0, row_input_1, row_input_2, row_input_3;
            float d_y;
            if (row_input < 0.5)
            {
                row_input = 0.5;
            }
            if (row_input > input_image.rows - 0.5)
            {
                row_input = input_image.rows - 0.5;
            }

            if (row_input < 1.5)
            {
                row_input_0 = 0;
                row_input_1 = 0;
                row_input_2 = 1;
                row_input_3 = 2;
            }
            else if (row_input >= 1.5 && row_input < input_image.rows - 1.5)
            {
                row_input_0 = (int)std::floor(row_input - 1.5);
                row_input_1 = row_input_0 + 1;
                row_input_2 = row_input_0 + 2;
                row_input_3 = row_input_0 + 3;
            }
            else if (row_input >= input_image.rows - 1.5)
            {
                row_input_0 = input_image.rows - 3;
                row_input_1 = row_input_0 + 1;
                row_input_2 = row_input_0 + 2;
                row_input_3 = row_input_0 + 2;
            }
            d_y = row_input - (row_input_1 + 0.5);

            int fx = fx_interpolation(input_image, col_input_0, col_input_1, col_input_2, col_input_3, row_input_0, row_input_1, row_input_2, row_input_3, d_x, d_y);
            output_image.at<uchar>(cv::Point(col_output, row_output)) = fx;
        }
    }
    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);
    cv::waitKey(0);
}

int main()
{
    cv::Mat input_image = cv::imread("00.png", 0);
    if (input_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
        return -1;
    }
    std::cout << "input_image. cols = " << input_image.cols << "input_image.rows = " << input_image.rows << std::endl;
    float ratio;
    do
    {
        std::cout << "enter ratio: " << std::endl;
        std::cin >> ratio;
    } while (ratio <= 0);
    cv::Mat output_image(cv::Size(input_image.cols * ratio, input_image.rows * ratio), input_image.type());
    resize_bicubic(input_image, output_image);
    return 0;
}