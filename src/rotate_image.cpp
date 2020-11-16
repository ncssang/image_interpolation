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

void rotate_image(cv::Mat& input_image, cv::Mat& output_image, float angle, Method method)
{
    std::vector<Pixel> pixel_map;
    std::size_t map_size = input_image.cols * input_image.rows;
    pixel_map.reserve(map_size);
    std::cout << "map_size: " << map_size << std::endl;
    // float rad = ((angle)*M_PI) / 180;

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
    // float x_min = point_x[0];
    // for (size_t i = 0; i < point_x.size(); ++i)
    // {
    //     if (point_x[i] < x_min)
    //     {
    //         x_min = point_x[i];
    //     }
    // }
    // float y_min = point_y[0];
    // for (size_t i = 0; i < point_y.size(); ++i)
    // {
    //     if (point_y[i] < y_min)
    //     {
    //         y_min = point_y[i];
    //     }
    // }
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

    if (method == NEAREST)
    {
        for (int row_output = 0; row_output < output_image.rows; ++row_output)
        {
            for (int col_output = 0; col_output < output_image.cols; ++col_output)
            {
                int output_index = row_output * output_image.cols + col_output;
                float input_image_row = pixel_map[output_index].row;
                float input_image_col = pixel_map[output_index].column;
                if (input_image_col >= 0 && input_image_col < input_image.cols && input_image_row >= 0 && input_image_row < input_image.rows)
                {
                    int input_col, input_row;
                    if (input_image_col >= 0 && input_image_col < 0.5)
                    {
                        input_col = 0;
                    }
                    else if (input_image_col < input_image.cols && input_image_col > input_image.cols - 1)
                    {
                        input_col = input_image.cols - 1;
                    }
                    else
                    {
                        input_col = std::round(input_image_col - 0.5);
                    }

                    if (input_image_row >= 0 && input_image_row < 0.5)
                    {
                        input_row = 0;
                    }
                    else if (input_image_row < input_image.rows && input_image_row > input_image.rows - 1)
                    {
                        input_row = input_image.rows - 1;
                    }
                    else
                    {
                        input_row = std::round(input_image_row - 0.5);
                    }

                    int input_index = input_row * input_image.cols + input_col;
                    for (int i = 0; i < number_of_channels; ++i)
                    {
                        output_image.data[output_index * number_of_channels + i] = input_image.data[input_index * number_of_channels + i];
                    }
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
                int output_index = output_row * output_image.cols + output_col;
                float input_image_row = pixel_map[output_index].row;
                float input_image_col = pixel_map[output_index].column;
                if (input_image_row >= 0 && input_image_row < input_image.rows && input_image_col >= 0 && input_image_col < input_image.cols)
                {
                    int input_top, input_bottom, input_left, input_right;
                    if (input_image_row > input_image.rows - 1 && input_image_row < input_image.rows)
                    {
                        input_top = input_image.rows - 1;
                        input_bottom = input_image.rows;
                    }
                    else if (input_image_row >= 0 && input_image_row < 0.5)
                    {
                        input_top = 0;
                        input_bottom = 0;
                    }
                    else
                    {
                        input_top = std::floor(input_image_row);
                        input_bottom = input_top + 1;
                    }

                    if (input_image_col > input_image.cols - 1 && input_image_col < input_image.cols)
                    {
                        input_left = input_image.cols - 1;
                        input_right = input_image.cols;
                    }
                    else if (input_image_col >= 0 && input_image_col < 0.5)
                    {
                        input_left = 0;
                        input_right = 0;
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
    }
    else
    {
        for (int row_output = 0; row_output < output_image.rows; ++row_output)
        {
            for (int col_output = 0; col_output < output_image.cols; ++col_output)
            {
                int output_index = row_output * output_image.cols + col_output;
                float input_image_row = pixel_map[output_index].row;
                float input_image_col = pixel_map[output_index].column;
                if (input_image_col >= 0 && input_image_col < input_image.cols && input_image_row >= 0 && input_image_row < input_image.rows)
                {
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
    // for (int i = 0; i < 720; ++i)
    // {
    // std::string file_name = "image_";
    // file_name = file_name + std::to_string(i);
    // file_name.append(".png");
    // std::cout << file_name << std::endl;
    // cv::imwrite(file_name, output_image);
    cv::Mat output_image_nearest;
    rotate_image(input_image, output_image_nearest, angle, NEAREST);

    cv::Mat output_image_bilinear;
    rotate_image(input_image, output_image_bilinear, angle, BILINEAR);
    cv::Mat output_image_bicubic;
    rotate_image(input_image, output_image_bicubic, angle, BICUBIC);
    imshow("Input_Image_nearest", input_image);
    imshow("Output_Image_nearest", output_image_nearest);
    imshow("output_image_bilinear", output_image_bilinear);
    imshow("output_image_bicubic", output_image_bicubic);
    cv::waitKey(0);
    // }

    return 0;
}