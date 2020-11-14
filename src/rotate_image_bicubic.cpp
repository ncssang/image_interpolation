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

cv::Point2f rotate(cv::Point2f A, float angle)
{
    float rad = (angle * M_PI) / 180;
    cv::Point2f B;
    B.x = A.x * cos(rad) - A.y * sin(rad);
    B.y = A.x * sin(rad) + A.y * cos(rad);
    return B;
}
cv::Point2f rotate_AX(cv::Point2f A, cv::Point2f X, float angle)
{
    cv::Point2f A1;
    A1.x = A.x - X.x;
    A1.y = A.y - X.y;
    cv::Point2f P = rotate(A1, angle);
    P.x += X.x;
    P.y += X.y;
    return P;
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
    std::size_t map_size = input_image.cols * input_image.rows;
    pixel_map.reserve(map_size);
    std::cout << "map_size: " << map_size << std::endl;

    // cv::Point2f center;
    // center.x = (float)input_image.cols / 2;
    // center.y = (float)input_image.rows / 2;
    // std::cout << "center_x: " << center.x << std::endl;
    // std::cout << "center_y: " << center.y << std::endl;
    // float rad = ((angle)*M_PI) / 180;

    // cv::Point2f point_input;
    // point_input.x = input_image.cols;
    // point_input.y = input_image.rows;

    // float distant_input = get_euclid_distance(center, point_input);
    // int radius_input = std::round(distant_input);
    // std::cout << "radius_input: " << radius_input << std::endl;

    // cv::Size size(radius_input * 2, radius_input * 2);
    // int number_of_channels = input_image.channels();
    // int image_type = input_image.type();
    // output_image.create(size, image_type);

    // cv::Point2f center_output;
    // center_output.x = (float)output_image.cols / 2;
    // center_output.y = (float)output_image.rows / 2;

    // cv::Point2f point_change;
    // point_change.x = center_output.x - center.x;;
    // point_change.y = center_output.y - center.y;
    cv::Point2f input_center;
    input_center.x = input_image.cols / 2;
    input_center.y = input_image.rows / 2;

    cv::Point2f a, b, c, d;
    a.x = input_image.cols;
    a.y = 0;
    b.x = 0;
    b.y = 0;
    c.x = 0;
    c.y = input_image.rows;
    d.x = input_image.cols;
    d.y = input_image.rows;

    cv::Point2f a_1, b_1, c_1, d_1;
    a_1 = rotate_AX(a, input_center, angle);
    b_1 = rotate_AX(b, input_center, angle);
    c_1 = rotate_AX(c, input_center, angle);
    d_1 = rotate_AX(d, input_center, angle);

    std::vector<float> point_x;
    std::vector<float> point_y;
    point_x.push_back(a_1.x);
    point_x.push_back(b_1.x);
    point_x.push_back(c_1.x);
    point_x.push_back(d_1.x);

    point_y.push_back(a_1.y);
    point_y.push_back(b_1.y);
    point_y.push_back(c_1.y);
    point_y.push_back(d_1.y);
    std::sort(point_x.begin(), point_x.end());
    std::sort(point_y.begin(), point_y.end());

    int size = point_x.size();
    int d_x, d_y;
    d_x = std::round(point_x[size - 1] - point_x[0]);
    d_y = std::round(point_y[size - 1] - point_y[0]);

    int number_of_channels = input_image.channels();
    int image_type = input_image.type();
    // output_image.create(d_y, d_x, image_type);
    output_image = cv::Mat::zeros(d_y, d_x, image_type);
    
    cv::Point2f center_output;
    center_output.x = (float)output_image.cols / 2;
    center_output.y = (float)output_image.rows / 2;

    cv::Point2f point_change;
    point_change.x = center_output.x - input_center.x;
    point_change.y = center_output.y - input_center.y;

    for (int row_output = 0; row_output < output_image.rows; ++row_output)
    {
        for (int col_output = 0; col_output < output_image.cols; ++col_output)
        {
            cv::Point2f output_point;
            output_point.x = col_output + 0.5;
            output_point.y = row_output + 0.5;
            cv::Point2f rotate = rotate_AX(output_point, center_output, angle);
            pixel_map.emplace_back(Pixel(rotate.x - point_change.x, rotate.y - point_change.y));
        }
    }

    // for (int row_output = 0; row_output < output_image.rows; ++row_output)
    // {
    //     for (int col_output = 0; col_output < output_image.cols; ++col_output)
    //     {
    //         float column = (((col_output + 0.5) - center_output.x) * cos(rad) - ((row_output + 0.5) - center_output.y) * sin(rad)) + center_output.x;
    //         float row = (((col_output + 0.5) - center_output.x) * sin(rad) + ((row_output + 0.5) - center_output.y) * cos(rad)) + center_output.y;
    //         pixel_map.emplace_back(Pixel(column - point_change.x, row - point_change.y));
    //     }
    // }
    std::cout << "input_image_col: " << pixel_map[200].column << "; input_image.row: " << pixel_map[300].row << std::endl;
    for (int row_output = 0; row_output < output_image.cols; ++row_output)
    {
        for (int col_output = 0; col_output < output_image.rows; ++col_output)
        {
            int output_index = row_output * output_image.cols + col_output;
            float input_image_row = pixel_map[output_index].row;
            float input_image_col = pixel_map[output_index].column;
            if (input_image_col >= 0 && input_image_col < input_image.cols && input_image_row >= 0 && input_image_row < input_image.rows)
            {
                int input_x0, input_x1, input_x2, input_x3;
                if (input_image_col >= 0 && input_image_col < 0.5)
                {
                    input_image_col = 0.5;
                }
                if (input_image_col > input_image.cols - 0.5)
                {
                    input_image_col = input_image.cols - 0.5;
                }
                if (input_image_col < 1.5)
                {
                    input_x0 = 0;
                    input_x1 = 0;
                    input_x2 = 1;
                    input_x3 = 2;
                }
                else if (input_image_col >= 1.5 && input_image_col < input_image.cols - 1.5)
                {
                    input_x0 = (int)std::floor(input_image_col - 1.5);
                    input_x1 = input_x0 + 1;
                    input_x2 = input_x0 + 2;
                    input_x3 = input_x0 + 3;
                }
                else if (input_image_col >= input_image.cols - 1.5)
                {
                    input_x0 = input_image.cols - 3;
                    input_x1 = input_x0 + 1;
                    input_x2 = input_x0 + 2;
                    input_x3 = input_x0 + 3;
                }

                int input_y0, input_y1, input_y2, input_y3;
                if (input_image_row >= 0 && input_image_row < 0.5)
                {
                    input_image_row = 0.5;
                }
                if (input_image_row > input_image.rows - 0.5)
                {
                    input_image_row = input_image.rows - 0.5;
                }
                if (input_image_row < 1.5)
                {
                    input_y0 = 0;
                    input_y1 = 0;
                    input_y2 = 1;
                    input_y3 = 2;
                }
                else if (input_image_row >= 1.5 && input_image_row < input_image.rows - 1.5)
                {
                    input_y0 = (int)std::floor(input_image_row - 1.5);
                    input_y1 = input_y0 + 1;
                    input_y2 = input_y0 + 2;
                    input_y3 = input_y0 + 3;
                }
                else if (input_image_row >= input_image.rows - 1.5)
                {
                    input_y0 = input_image.rows - 3;
                    input_y1 = input_y0 + 1;
                    input_y2 = input_y0 + 2;
                    input_y3 = input_y0 + 3;
                }

                float d_x = input_image_col - std::floor(input_image_col);
                float d_y = input_image_row - std::floor(input_image_row);
                for (int i = 0; i < number_of_channels; ++i)
                {
                    uchar source_x_y_0 = get_value(input_image.data[(input_y0 * input_image.cols + input_x0) * number_of_channels + i],
                                                   input_image.data[(input_y0 * input_image.cols + input_x1) * number_of_channels + i],
                                                   input_image.data[(input_y0 * input_image.cols + input_x2) * number_of_channels + i],
                                                   input_image.data[(input_y0 * input_image.cols + input_x3) * number_of_channels + i],
                                                   d_x);
                    uchar source_x_y_1 = get_value(input_image.data[(input_y1 * input_image.cols + input_x0) * number_of_channels + i],
                                                   input_image.data[(input_y1 * input_image.cols + input_x1) * number_of_channels + i],
                                                   input_image.data[(input_y1 * input_image.cols + input_x2) * number_of_channels + i],
                                                   input_image.data[(input_y1 * input_image.cols + input_x3) * number_of_channels + i],
                                                   d_x);
                    uchar source_x_y_2 = get_value(input_image.data[(input_y2 * input_image.cols + input_x0) * number_of_channels + i],
                                                   input_image.data[(input_y2 * input_image.cols + input_x1) * number_of_channels + i],
                                                   input_image.data[(input_y2 * input_image.cols + input_x2) * number_of_channels + i],
                                                   input_image.data[(input_y2 * input_image.cols + input_x3) * number_of_channels + i],
                                                   d_x);
                    uchar source_x_y_3 = get_value(input_image.data[(input_y3 * input_image.cols + input_x0) * number_of_channels + i],
                                                   input_image.data[(input_y3 * input_image.cols + input_x1) * number_of_channels + i],
                                                   input_image.data[(input_y3 * input_image.cols + input_x2) * number_of_channels + i],
                                                   input_image.data[(input_y3 * input_image.cols + input_x3) * number_of_channels + i],
                                                   d_x);
                    uchar source_value = get_value(source_x_y_0, source_x_y_1, source_x_y_2, source_x_y_3, d_y);
                    output_image.data[output_index * number_of_channels + i] = source_value;
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

    imshow("Input_Image_bicubic", input_image);
    imshow("Output_Image_bicubic", output_image);
    cv::waitKey(0);

    return 0;
}