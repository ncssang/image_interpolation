#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <vector>

enum Method : uchar
{
    NEAREST = 0,
    BILINEAR = 1,
    BICUBIC = 2
};

struct Pixel
{
    float column;
    float row;
    Pixel(const float column = 0, const float row = 0) : column(column), row(row)
    {
    }
};

uchar get_cubic_interpolation(uchar point_0, uchar point_1, uchar point_2, uchar point_3, float x)
{
    float a = -0.5 * point_0 + 1.5 * point_1 - 1.5 * point_2 + 0.5 * point_3;
    float b = point_0 - 2.5 * point_1 + 2 * point_2 - 0.5 * point_3;
    float c = -0.5 * point_0 + 0.5 * point_2;
    float d = point_1;
    int value = d + x * (c + x * (b + x * a));
    if (value < 0)
    {
        value = 0;
    }
    if (value > 255)
    {
        value = 255;
    }
    return value;
}

void resize_image(cv::Mat input_image, cv::Mat& output_image, cv::Size& size, Method method)
{
    std::vector<Pixel> pixel_map;
    size_t map_size = size.width * size.height;
    pixel_map.reserve(map_size);
    for (size_t i = 0; i < map_size; ++i)
    {
        int column_new = i % size.width;
        int row_new = i / size.width;
        float column = ((float)(input_image.cols) / (size.width) * (column_new + 0.5)) - 0.5;
        float row = ((float)(input_image.rows) / (size.height) * (row_new + 0.5)) - 0.5;
        pixel_map.emplace_back(Pixel(column, row));
    }

    int number_of_channels = input_image.channels();
    int image_type = input_image.type();
    output_image.create(size, image_type);

    if (method == NEAREST)
    {
        for (int destination_row = 0; destination_row < output_image.rows; ++destination_row)
        {
            for (int destination_column = 0; destination_column < output_image.cols; ++destination_column)
            {
                int output_index = destination_row * output_image.cols + destination_column;
                int input_row = std::round(pixel_map[output_index].row);
                int input_col = std::round(pixel_map[output_index].column);
                int input_index = input_row * input_image.cols + input_col;
                for (int i = 0; i < number_of_channels; ++i)
                {
                    output_image.data[output_index * number_of_channels + i] = input_image.data[input_index * number_of_channels + i];
                }
            }
        }
    }
    else if (method == BILINEAR)
    {
        for (int output_row = 0; output_row < output_image.rows; ++output_row)
        {
            for (int output_col = 0; output_col < output_image.cols; ++output_col)
            {
                int destination_index = output_row * output_image.cols + output_col;
                float source_row = pixel_map[destination_index].row;
                float source_column = pixel_map[destination_index].column;

                int input_top, input_bottom;
                if (std::floor(source_row) == -1)
                {
                    // The top margin
                    input_top = 0;
                    input_bottom = 0;
                }
                else if (std::floor(source_row) == input_image.rows - 1)
                {
                    // The bottom margin
                    input_top = input_image.rows - 1;
                    input_bottom = input_image.rows - 1;
                }
                else
                {
                    input_top = std::floor(source_row);
                    input_bottom = input_top + 1;
                }

                int input_left, input_right;
                if (std::floor(source_column) == -1)
                {
                    // The left margin
                    input_left = 0;
                    input_right = 0;
                }
                else if (std::floor(source_column) == input_image.cols - 1)
                {
                    // The right margin
                    input_left = input_image.cols - 1;
                    input_right = input_image.cols - 1;
                }
                else
                {
                    input_left = std::floor(source_column);
                    input_right = input_left + 1;
                }

                int source_top_left_index = input_top * input_image.cols + input_left;
                int source_top_right_index = input_top * input_image.cols + input_right;
                int source_bottom_left_index = input_bottom * input_image.cols + input_left;
                int source_bottom_right_index = input_bottom * input_image.cols + input_right;
                float d_x = source_column - input_left;
                float d_y = source_row - input_top;
                for (int i = 0; i < number_of_channels; ++i)
                {
                    // Assign to the weighted average
                    output_image.data[destination_index * number_of_channels + i] = (1 - d_y) * ((1 - d_x) * input_image.data[source_top_left_index * number_of_channels + i] + d_x * input_image.data[source_top_right_index * number_of_channels + i]) +
                                                                                    d_y * ((1 - d_x) * input_image.data[source_bottom_left_index * number_of_channels + i] + d_x * input_image.data[source_bottom_right_index * number_of_channels + i]);
                }
            }
        }
    }
    else
        for (int row_output = 0; row_output < output_image.cols; ++row_output)
        {
            for (int col_output = 0; col_output < output_image.rows; ++col_output)
            {
                // float col_input = (col_output + 0.5) / actual_ratio;
                // float d_x;
                int output_index = row_output * output_image.cols + col_output;
                float input_image_row = pixel_map[output_index].row;
                float input_image_col = pixel_map[output_index].column;
                int input_x0, input_x1, input_x2, input_x3;
                if (std::floor(input_image_col) == -1)
                {
                    input_x0 = 0;
                    input_x1 = 0;
                    input_x2 = 0;
                    input_x3 = 1;
                }
                else if (std::floor(input_image_col) == 0)
                {
                    input_x0 = 0;
                    input_x1 = 0;
                    input_x2 = 1;
                    input_x3 = 2;
                }
                else if (std::floor(input_image_col) == input_image.cols - 2)
                {
                    input_x0 = input_image.cols - 3;
                    input_x1 = input_image.cols - 2;
                    input_x2 = input_image.cols - 1;
                    input_x3 = input_image.cols - 1;
                }
                else if (std::floor(input_image_col) == input_image.cols - 1)
                {
                    input_x0 = input_image.cols - 2;
                    input_x1 = input_image.cols - 1;
                    input_x2 = input_image.cols - 1;
                    input_x3 = input_image.cols - 1;
                }
                else
                {
                    input_x0 = std::floor(input_image_col) - 1;
                    input_x1 = input_x0 + 1;
                    input_x2 = input_x0 + 2;
                    input_x3 = input_x0 + 3;
                }
                int input_y0, input_y1, input_y2, input_y3;
                if (std::floor(input_image_row) == -1)
                {
                    input_y0 = 0;
                    input_y1 = 0;
                    input_y2 = 0;
                    input_y3 = 1;
                }
                else if (std::floor(input_image_row) == 0)
                {
                    input_y0 = 0;
                    input_y1 = 0;
                    input_y2 = 1;
                    input_y3 = 2;
                }
                else if (std::floor(input_image_row) == input_image.rows - 2)
                {
                    input_y0 = input_image.rows - 3;
                    input_y1 = input_image.rows - 2;
                    input_y2 = input_image.rows - 1;
                    input_y3 = input_image.rows - 1;
                }
                else if (std::floor(input_image_row) == input_image.rows - 1)
                {
                    input_y0 = input_image.rows - 2;
                    input_y1 = input_image.rows - 1;
                    input_y2 = input_image.rows - 1;
                    input_y3 = input_image.rows - 1;
                }
                else
                {
                    input_y0 = std::floor(input_image_row) - 1;
                    input_y1 = input_y0 + 1;
                    input_y2 = input_y0 + 2;
                    input_y3 = input_y0 + 3;
                }
                float d_x = input_image_col - std::floor(input_image_col);
                float d_y = input_image_row - std::floor(input_image_row);
                for (int i = 0; i < number_of_channels; ++i)
                {
                    // Calculate the cubic interpolation for each row
                    uchar source_x_y_0 = get_cubic_interpolation(input_image.data[(input_y0 * input_image.cols + input_x0) * number_of_channels + i],
                                                                 input_image.data[(input_y0 * input_image.cols + input_x1) * number_of_channels + i],
                                                                 input_image.data[(input_y0 * input_image.cols + input_x2) * number_of_channels + i],
                                                                 input_image.data[(input_y0 * input_image.cols + input_x3) * number_of_channels + i],
                                                                 d_x);
                    uchar source_x_y_1 = get_cubic_interpolation(input_image.data[(input_y1 * input_image.cols + input_x0) * number_of_channels + i],
                                                                 input_image.data[(input_y1 * input_image.cols + input_x1) * number_of_channels + i],
                                                                 input_image.data[(input_y1 * input_image.cols + input_x2) * number_of_channels + i],
                                                                 input_image.data[(input_y1 * input_image.cols + input_x3) * number_of_channels + i],
                                                                 d_x);
                    uchar source_x_y_2 = get_cubic_interpolation(input_image.data[(input_y2 * input_image.cols + input_x0) * number_of_channels + i],
                                                                 input_image.data[(input_y2 * input_image.cols + input_x1) * number_of_channels + i],
                                                                 input_image.data[(input_y2 * input_image.cols + input_x2) * number_of_channels + i],
                                                                 input_image.data[(input_y2 * input_image.cols + input_x3) * number_of_channels + i],
                                                                 d_x);
                    uchar source_x_y_3 = get_cubic_interpolation(input_image.data[(input_y3 * input_image.cols + input_x0) * number_of_channels + i],
                                                                 input_image.data[(input_y3 * input_image.cols + input_x1) * number_of_channels + i],
                                                                 input_image.data[(input_y3 * input_image.cols + input_x2) * number_of_channels + i],
                                                                 input_image.data[(input_y3 * input_image.cols + input_x3) * number_of_channels + i],
                                                                 d_x);
                    uchar source_value = get_cubic_interpolation(source_x_y_0, source_x_y_1, source_x_y_2, source_x_y_3, d_y);
                    output_image.data[output_index * number_of_channels + i] = source_value;
                }
            }
        }
}

int main()
{
    cv::Mat input_image = cv::imread("00.png", 1);
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

    cv::Size output_size(input_image.cols * ratio, input_image.rows * ratio);

    cv::Mat resized_image_nearest;
    resize_image(input_image, resized_image_nearest, output_size, NEAREST);

    cv::Mat resized_image_bilinear;
    resize_image(input_image, resized_image_bilinear, output_size, BILINEAR);

    cv::Mat resized_image_bicubic;
    resize_image(input_image, resized_image_bicubic, output_size, BICUBIC);

    imshow("Input_Image", input_image);
    imshow("resized_image_nearest", resized_image_nearest);
    imshow("resized_image_bilinear", resized_image_bilinear);
    imshow("resized_image_bicubic", resized_image_bicubic);
    cv::waitKey(0);
    return 0;
}