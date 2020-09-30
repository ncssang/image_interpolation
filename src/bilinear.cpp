#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <vector>

int get_value(int p_00, int p_01, int p_10, int p_11, float d_x, float d_y)
{
    int p_A = (1 - d_x) * p_00 + d_x * p_10;
    int p_B = (1 - d_x) * p_01 + d_x * p_11;
    int p_AB = (1 - d_y) * p_A + d_y * p_B;
    return p_AB;
}
int get_value_special(int p_00, int p_01, float d_y)
{
    int p_X = (1 - d_y) * p_00 + d_y * p_01;
    return p_X;
}
int main()
{
    cv::Mat input_image = cv::imread("Histogram_Calculation_Original_Image.jpg");
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
    float actual_ratio = (float)output_image.cols / input_image.cols;
    for (int col_output = 0; col_output < output_image.cols; ++col_output)
    {
        float col_input = (float)(col_output + 0.5) / actual_ratio;
        if (col_input < 0.5)
        {
            int col_input_0 = 0;
            for (int row_output = 0; row_output < output_image.rows; ++row_output)
            {
                float row_input = (float)(row_output + 0.5) / actual_ratio;
                if (row_input < 0.5)
                {
                    int row_input_0 = 0;
                    if (input_image.type() == CV_8UC1)
                    {
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                    }
                }
                else if (row_input >= 0.5 && row_input <= input_image.rows - 0.5)
                {
                    int row_input_0 = (int)std::floor(row_input - 0.5);
                    int row_input_1 = row_input_0 + 1;
                    float d_y = row_input - (row_input_0 + 0.5);
                    if (input_image.type() == CV_8UC1)
                    {
                        int fx00 = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                        int fx01 = input_image.at<uchar>(cv::Point(col_input_0, row_input_1));
                        int fx_interpolation = get_value_special(fx00, fx01, d_y);
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = fx_interpolation;
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        cv::Vec3b fx00_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                        int fx00_b = fx00_3c[0];
                        int fx00_g = fx00_3c[1];
                        int fx00_r = fx00_3c[2];
                        cv::Vec3b fx01_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_1));
                        int fx01_b = fx01_3c[0];
                        int fx01_g = fx01_3c[1];
                        int fx01_r = fx01_3c[2];
                        int fx_interpolation_b = get_value_special(fx00_b, fx01_b, d_y);
                        int fx_interpolation_g = get_value_special(fx00_g, fx01_g, d_y);
                        int fx_interpolation_r = get_value_special(fx00_r, fx01_r, d_y);
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = cv::Vec3b(fx_interpolation_b, fx_interpolation_g, fx_interpolation_r);
                    }
                }
                else if (row_input > input_image.rows - 0.5)
                {
                    int row_input_0 = input_image.rows - 1;
                    if (input_image.type() == CV_8UC1)
                    {
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                    }
                }
            }
        }
        else if (col_input >= 0.5 && col_input <= input_image.cols - 0.5)
        {
            int col_input_0 = (int)std::floor(col_input - 0.5);
            int col_input_1 = col_input_0 + 1;
            float d_x = col_input - (col_input_0 + 0.5);
            for (int row_output = 0; row_output < output_image.rows; ++row_output)
            {
                float row_input = (float)(row_output + 0.5) / actual_ratio;
                if (row_input < 0.5)
                {
                    int row_input_0 = 0;
                    if (input_image.type() == CV_8UC1)
                    {
                        int fx00 = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                        int fx10 = input_image.at<uchar>(cv::Point(col_input_1, row_input_0));
                        int fx_interpolation = get_value_special(fx00, fx10, d_x);
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = fx_interpolation;
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        cv::Vec3b fx00_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                        int fx00_b = fx00_3c[0];
                        int fx00_g = fx00_3c[1];
                        int fx00_r = fx00_3c[2];
                        cv::Vec3b fx10_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_0));
                        int fx10_b = fx10_3c[0];
                        int fx10_g = fx10_3c[1];
                        int fx10_r = fx10_3c[2];
                        int fx_interpolation_b = get_value_special(fx00_b, fx10_b, d_x);
                        int fx_interpolation_g = get_value_special(fx00_g, fx10_g, d_x);
                        int fx_interpolation_r = get_value_special(fx00_r, fx10_r, d_x);
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = cv::Vec3b(fx_interpolation_b, fx_interpolation_g, fx_interpolation_r);
                    }
                }
                else if (row_input >= 0.5 && row_input <= input_image.rows - 0.5)
                {
                    int row_input_0 = (int)std::floor(row_input - 0.5);
                    int row_input_1 = row_input_0 + 1;
                    float d_y = row_input - (row_input_0 + 0.5);
                    if (input_image.type() == CV_8UC1)
                    {
                        int fx00 = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                        int fx01 = input_image.at<uchar>(cv::Point(col_input_0, row_input_1));
                        int fx10 = input_image.at<uchar>(cv::Point(col_input_1, row_input_0));
                        int fx11 = input_image.at<uchar>(cv::Point(col_input_1, row_input_1));
                        int fx_interpolation = get_value(fx00, fx01, fx10, fx11, d_x, d_y);
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = fx_interpolation;
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        cv::Vec3b fx00_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                        int fx00_b = fx00_3c[0];
                        int fx00_g = fx00_3c[1];
                        int fx00_r = fx00_3c[2];
                        cv::Vec3b fx01_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_1));
                        int fx01_b = fx01_3c[0];
                        int fx01_g = fx01_3c[1];
                        int fx01_r = fx01_3c[2];
                        cv::Vec3b fx10_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_0));
                        int fx10_b = fx10_3c[0];
                        int fx10_g = fx10_3c[1];
                        int fx10_r = fx10_3c[2];
                        cv::Vec3b fx11_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_1));
                        int fx11_b = fx11_3c[0];
                        int fx11_g = fx11_3c[1];
                        int fx11_r = fx11_3c[2];
                        int fx_interpolation_b = get_value(fx00_b, fx01_b, fx10_b, fx11_b, d_x, d_y);
                        int fx_interpolation_g = get_value(fx00_g, fx01_g, fx10_g, fx11_g, d_x, d_y);
                        int fx_interpolation_r = get_value(fx00_r, fx01_r, fx10_r, fx11_r, d_x, d_y);
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = cv::Vec3b(fx_interpolation_b, fx_interpolation_g, fx_interpolation_r);
                    }
                }
                else if (row_input > input_image.rows - 0.5)
                {
                    int row_input_0 = input_image.rows - 1;
                    if (input_image.type() == CV_8UC1)
                    {
                        int fx00 = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                        int fx01 = input_image.at<uchar>(cv::Point(col_input_1, row_input_0));
                        int fx_interpolation = get_value_special(fx00, fx01, d_x);
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = fx_interpolation;
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        cv::Vec3b fx00_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                        int fx00_b = fx00_3c[0];
                        int fx00_g = fx00_3c[1];
                        int fx00_r = fx00_3c[2];
                        cv::Vec3b fx10_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_0));
                        int fx10_b = fx10_3c[0];
                        int fx10_g = fx10_3c[1];
                        int fx10_r = fx10_3c[2];
                        int fx_interpolation_b = get_value_special(fx00_b, fx10_b, d_x);
                        int fx_interpolation_g = get_value_special(fx00_g, fx10_g, d_x);
                        int fx_interpolation_r = get_value_special(fx00_r, fx10_r, d_x);
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = cv::Vec3b(fx_interpolation_b, fx_interpolation_g, fx_interpolation_r);
                    }
                }
            }
        }
        else if (col_input > input_image.cols - 0.5)
        {
            int col_input_0 = input_image.cols - 1;
            for (int row_output = 0; row_output < output_image.rows; ++row_output)
            {
                float row_input = (float)(row_output + 0.5) / actual_ratio;
                if (row_input < 0.5)
                {
                    int row_input_0 = 0;
                    if (input_image.type() == CV_8UC1)
                    {
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                    }
                }
                else if (row_input >= 0.5 && row_input <= input_image.rows - 0.5)
                {
                    int row_input_0 = (int)std::floor(row_input - 0.5);
                    int row_input_1 = row_input_0 + 1;
                    float d_y = row_input - (row_input_0 + 0.5);
                    if (input_image.type() == CV_8UC1)
                    {
                        int fx00 = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                        int fx01 = input_image.at<uchar>(cv::Point(col_input_0, row_input_1));
                        int fx_interpolation = get_value_special(fx00, fx01, d_y);
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = fx_interpolation;
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        cv::Vec3b fx00_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                        int fx00_b = fx00_3c[0];
                        int fx00_g = fx00_3c[1];
                        int fx00_r = fx00_3c[2];
                        cv::Vec3b fx01_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_1));
                        int fx01_b = fx01_3c[0];
                        int fx01_g = fx01_3c[1];
                        int fx01_r = fx01_3c[2];
                        int fx_interpolation_b = get_value_special(fx00_b, fx01_b, d_y);
                        int fx_interpolation_g = get_value_special(fx00_g, fx01_g, d_y);
                        int fx_interpolation_r = get_value_special(fx00_r, fx01_r, d_y);
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = cv::Vec3b(fx_interpolation_b, fx_interpolation_g, fx_interpolation_r);
                    }
                }
                else if (row_input > input_image.rows - 0.5)
                {
                    int row_input_0 = input_image.rows - 1;
                    if (input_image.type() == CV_8UC1)
                    {
                        output_image.at<uchar>(cv::Point(col_output, row_output)) = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                    }
                    else if (input_image.type() == CV_8UC3)
                    {
                        output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
                    }
                }
            }
        }
    }
    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);

    cv::waitKey(0);
    return 0;
}