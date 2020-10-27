#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <vector>

struct Pixel
{
    float column;
    float row;
    Pixel(const float column = 0, const float row = 0) : column(column), row(row)
    {
    }
};

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
float get_euclid_distance(cv::Point2f point_1, cv::Point2f point_2)
{
    float d_x = point_1.x - point_2.x;
    float d_y = point_1.y - point_2.y;
    float distance = sqrt(d_x * d_x + d_y * d_y);
    return distance;
}

void rotate_image(cv::Mat& input_image, cv::Mat& output_image, float angle)
{
    std::vector<Pixel> pixel_map;
    std::
        size_t map_size = input_image.cols * input_image.rows;
    pixel_map.reserve(map_size);
    std::cout << "map_size: " << map_size << std::endl;

    cv::Point2f center;
    center.x = (float)input_image.cols / 2;
    center.y = (float)input_image.rows / 2;
    float rad = ((angle)*M_PI) / 180;
    std::cout << "rad: " << rad << std::endl;
    // cv::Point2f a;
    // a.x = input_image.cols;
    // a.y = input_image.rows;
    // float x = get_euclid_distance(center, a);

    cv::Size size(input_image.cols, input_image.rows);
    // std::cout << "1" << std::endl;
    int number_of_channels = input_image.channels();
    int image_type = input_image.type();
    output_image.create(size, image_type);

    for (int row_output = 0; row_output < output_image.rows; ++row_output)
    {
        for (int col_output = 0; col_output < output_image.cols; ++col_output)
        {
            float column = ((col_output - center.x) * cos(rad) - (row_output - center.y) * sin(rad)) + center.x;
            float row = ((col_output - center.x) * sin(rad) + (row_output - center.y) * cos(rad)) + center.y;
            pixel_map.emplace_back(Pixel(column, row));
        }
    }
    // for (size_t i = 0; i < pixel_map.size(); ++i)
    // {
    //     std::cout << i << ": " << pixel_map[i].column << "; " << pixel_map[i].row << std::endl;
    // }

    for (int output_row = 0; output_row < output_image.rows; ++output_row)
    {
        for (int output_col = 0; output_col < output_image.cols; ++output_col)
        {
            int output_index = output_row * output_image.cols + output_col;
            float input_image_row = pixel_map[output_index].row;
            float input_image_col = pixel_map[output_index].column;

            int input_top, input_bottom;
            if (std::floor(input_image_row) == -1)
            {
                input_top = 0;
                input_bottom = 0;
            }
            else if (std::floor(input_image_row) == input_image.rows - 1)
            {
                input_top = input_image.rows - 1;
                input_bottom = input_image.rows - 1;
            }
            else
            {
                input_top = std::floor(input_image_row);
                input_bottom = input_top + 1;
            }

            int input_left, input_right;
            if (std::floor(input_image_col) == -1)
            {
                input_left = 0;
                input_right = 0;
            }
            else if (std::floor(input_image_col) == input_image.cols - 1)
            {
                input_left = input_image.cols - 1;
                input_right = input_image.cols - 1;
            }
            else
            {
                input_left = std::floor(input_image_col);
                input_right = input_left + 1;
            }

            int source_top_left_index = input_top * input_image.cols + input_left;
            int source_top_right_index = input_top * input_image.cols + input_right;
            int source_bottom_left_index = input_bottom * input_image.cols + input_left;
            int source_bottom_right_index = input_bottom * input_image.cols + input_right;
            float d_x = input_image_col - input_left;
            float d_y = input_image_row - input_top;
            for (int i = 0; i < number_of_channels; ++i)
            {
                output_image.data[output_index * number_of_channels + i] = (1 - d_y) * ((1 - d_x) * input_image.data[source_top_left_index * number_of_channels + i] + d_x * input_image.data[source_top_right_index * number_of_channels + i]) +
                                                                                d_y * ((1 - d_x) * input_image.data[source_bottom_left_index * number_of_channels + i] + d_x * input_image.data[source_bottom_right_index * number_of_channels + i]);
            }
        }
    }
}

int main()
{
    cv::Mat input_image = cv::imread("01.jpg");
    if (input_image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
        return -1;
    }
    float angle;
    std::cout << "Nhap goc: " << std::endl;
    std::cin >> angle;
    cv::Mat output_image;
    rotate_image(input_image, output_image, angle);

    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);
    cv::waitKey(0);

    return 0;
}