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

void resize_nearest(cv::Mat input_image, cv::Mat& output_image, const cv::Size& size)
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
        std::cout << "enter ratio: " << std::endl;
        std::cin >> ratio;
    } while (ratio <= 0);

    cv::Size output_size(input_image.cols * ratio, input_image.rows * ratio);
    cv::Mat output_image;
    resize_nearest(input_image, output_image, output_size);

    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);

    cv::waitKey(0);
    return 0;
}