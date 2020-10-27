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
    float rad = ((angle) * M_PI) / 180;
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

    for (int row_output = 0; row_output < output_image.rows; ++row_output)
    {
        for (int col_output = 0; col_output < output_image.cols; ++col_output)
        {
            // std::cout << "coloutput: " << output_image.cols << "; rowoutput: " << output_image.rows << std::endl;
            int output_index = row_output * output_image.cols + col_output;
            // std::cout << "output_index: " << output_index << "row_output: " << row_output << "; coloutput: " << col_output << std::endl;
            float input_image_row = pixel_map[output_index].row;
            float input_image_col = pixel_map[output_index].column;
            // std::cout << "output_index: " << output_index << "; input_image_row: " << pixel_map[output_index].row << "; input_image_col: " << pixel_map[output_index].column << std::endl;
            int input_col = std::round(input_image_col - 0.5);
            int input_row = std::round(input_image_row - 0.5);
            int input_index = input_row * input_image.cols + input_col;
            if (input_row >= 0 && input_row < input_image.rows && input_col >= 0 && input_col < input_image.cols)
            {
                for (int i = 0; i < number_of_channels; ++i)
                {
                    output_image.data[output_index * number_of_channels + i] = input_image.data[input_index * number_of_channels + i];
                }
            }
        }
    }
}

int main()
{
    cv::Mat input_image = cv::imread("01.jpg", 0);
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