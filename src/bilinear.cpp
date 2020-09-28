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
    // std::cout << "p_A: " << p_A << "; p_B: " << p_B << std::endl;
    int p_AB = (1 - d_y) * p_A + d_y * p_B;
    return p_AB;
}
int main()
{
    cv::Mat input_image = cv::imread("Histogram_Calculation_Original_Image.jpg", 0);
    if (input_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
        return -1;
    }
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
        // std::cout << "col_input = " << col_input << std::endl;
        if (col_input >= 0.5 && col_input <= input_image.cols - 0.5)
        {
            int col_input_0 = (int)std::floor(col_input - 0.5);
            int col_input_1 = col_input_0 + 1;
            // std::cout << "col_input_0 = " << col_input_0 << std::endl;
            // std::cout << "col_input_1 = " << col_input_1 << std::endl;
            float d_x = col_input - (col_input_0 + 0.5);
            // std::cout << "d_x = " << d_x << std::endl;
            for (int row_output = 0; row_output < output_image.rows; ++row_output)
            {
                float row_input = (float)(row_output + 0.5) / actual_ratio;
                if (row_input >= 0.5 && row_input <= input_image.rows - 0.5)
                {
                    int row_input_0 = (int)std::floor(row_input - 0.5);
                    int row_input_1 = row_input_0 + 1;
                    float d_y = row_input - (row_input_0 + 0.5);
                    int fx00 = input_image.at<uchar>(cv::Point(col_input_0, row_input_0));
                    // std::cout << "f_00 = " << fx00 << std::endl;
                    int fx01 = input_image.at<uchar>(cv::Point(col_input_0, row_input_1));
                    int fx10 = input_image.at<uchar>(cv::Point(col_input_1, row_input_0));
                    int fx11 = input_image.at<uchar>(cv::Point(col_input_1, row_input_1));
                    int fx_interpolation = get_value(fx00, fx01, fx10, fx11, d_x, d_y);
                    output_image.at<uchar>(cv::Point(col_output, row_output)) = fx_interpolation;
                }
            }
        }
    }
    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);

    cv::waitKey(0);
    return 0;
}