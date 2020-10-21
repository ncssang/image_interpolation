#include "image_interpolation/draw_line.hpp"

std::vector<cv::Point> get_line_points_AB(cv::Point point_a, cv::Point point_b)
{
    std::vector<cv::Point> line_points;

    float d_x = point_b.x - point_a.x;
    float d_y = point_b.y - point_a.y;
    if (d_x == 0 && d_y == 0)
    {
        cv::Point this_point;
        this_point.x = point_a.x;
        this_point.y = point_b.y;
        line_points.push_back(this_point);
    }
    else if (abs(d_x) > abs(d_y)) //tinh y = ax + b
    {
        if (point_a.x > point_b.x)
        {
            int temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            int temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        float a_value = d_y / d_x;
        float b_value = point_a.y - ((d_y / d_x) * point_a.x);
        for (int i = point_a.x; i <= point_b.x; ++i)
        {
            cv::Point this_point;
            int y_value = (int)std::round(a_value * i + b_value);
            this_point.x = i;
            this_point.y = y_value;
            line_points.push_back(this_point);
        }
        for (size_t i = 0; i < line_points.size(); ++i)
        {
            std::cout << i << "; " << line_points[i] << std::endl;
        }
    }
    else
    {
        float a_value = d_x / d_y;
        float b_value = point_a.x - ((d_x / d_y) * point_a.y);
        if (point_a.y > point_b.y)
        {
            int temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            int temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        for (int i = point_a.y; i <= point_b.y; ++i)
        {
            cv::Point this_point;
            int x_value = (int)std::round(a_value * i + b_value);
            this_point.y = i;
            this_point.x = x_value;
            line_points.push_back(this_point);
        }
        for (size_t i = 0; i < line_points.size(); ++i)
        {
            std::cout << i << "; " << line_points[i] << std::endl;
        }
    }
    return line_points;
}

std::vector<cv::Point> get_line_points_bresenham(cv::Point point_a, cv::Point point_b)
{
    std::vector<cv::Point> line_points;

    cv::Point this_point;
    this_point.x = point_a.x;
    this_point.y = point_a.y;
    if (point_b.x - point_a.x == 0 && point_b.y - point_a.y == 0)
    {
        cv::Point this_point;
        this_point.x = point_a.x;
        this_point.y = point_b.y;
        line_points.push_back(this_point);
    }
    else if (abs(point_b.x - point_a.x) > abs(point_b.y - point_a.y))
    {
        if (point_a.x > point_b.x)
        {
            int temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            int temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        int d_x = point_b.x - point_a.x;
        int d_y = point_b.y - point_a.y;
        this_point.y = point_a.y;
        if (point_b.y > point_a.y) // đường thẳng đi lên y = ax + b;
        {
            int p_1 = 2 * d_y - d_x;
            std::cout << "p_1 = " << p_1 << std::endl;
            for (int i = point_a.x; i <= point_b.x; ++i)
            {
                this_point.x = i;
                line_points.push_back(this_point);
                if (p_1 < 0)
                {
                    p_1 += 2 * d_y;
                }
                else
                {
                    p_1 += 2 * (d_y - d_x);
                    this_point.y++;
                }
            }
            for (size_t i = 0; i < line_points.size(); ++i)
            {
                std::cout << i << "; " << line_points[i] << std::endl;
            }
        }
        else // đường thẳng đi xuống y = ax + b;
        {
            int p_2 = 2 * d_y + d_x;
            for (int i = point_a.x; i <= point_b.x; ++i)
            {
                this_point.x = i;
                line_points.push_back(this_point);
                if (p_2 < 0)
                {
                    p_2 += 2 * d_y + 2 * d_x;
                    this_point.y--;
                }
                else
                {
                    p_2 += 2 * d_y;
                }
            }
            for (size_t i = 0; i < line_points.size(); ++i)
            {
                std::cout << i << "; " << line_points[i] << std::endl;
            }
        }
    }
    else // đường thẳng đi lên x = ay + b;
    {
        if (point_a.y > point_b.y)
        {
            int temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            int temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        int d_x = point_b.x - point_a.x;
        int d_y = point_b.y - point_a.y;
        if (point_b.x > point_a.x)
        {
            this_point.x = point_a.x;
            int p_3 = 2 * d_x - d_y;
            std::cout << "p_3 = " << p_3 << std::endl;
            for (int i = point_a.y; i <= point_b.y; ++i)
            {
                this_point.y = i;
                line_points.push_back(this_point);
                if (p_3 < 0)
                {
                    p_3 += 2 * d_x;
                }
                else
                {
                    p_3 += 2 * d_x - 2 * d_y;
                    this_point.x++;
                }
            }
            for (size_t i = 0; i < line_points.size(); ++i)
            {
                std::cout << i << "; " << line_points[i] << std::endl;
            }
        }
        else // đường thẳng đi xuống x = ay + b;
        {
            this_point.x = point_a.x;
            int p_4 = 2 * d_x + d_y;
            std::cout << "p_4 = " << p_4 << std::endl;
            for (int i = point_a.y; i <= point_b.y; ++i)
            {
                this_point.y = i;
                line_points.push_back(this_point);
                if (p_4 < 0)
                {
                    p_4 += 2 * d_y;
                    this_point.x--;
                }
                p_4 += 2 * d_x;
            }
            for (size_t i = 0; i < line_points.size(); ++i)
            {
                std::cout << i << "; " << line_points[i] << std::endl;
            }
        }
    }
    return line_points;
}

std::vector<cv::Point> get_line_points_dda(cv::Point point_a, cv::Point point_b)
{
    std::vector<cv::Point> line_points;
    float d_x = point_b.x - point_a.x;
    float d_y = point_b.y - point_a.y;
    float steps = 0;
    if (abs(d_x) < abs(d_y))
    {
        steps = abs(d_y);
    }
    else
    {
        steps = abs(d_x);
    }
    std::cout << "steps = " << steps << std::endl;
    if (d_x == 0 && d_y == 0)
    {
        cv::Point this_point;
        this_point.x = point_a.x;
        this_point.y = point_b.y;
        line_points.push_back(this_point);
    }
    else if (abs(d_x) > abs(d_y)) //tinh y = ax + b
    {
        if (point_a.x > point_b.x)
        {
            int temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            int temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        float d_y = point_b.y - point_a.y;
        float y_inc = d_y / steps;
        float y = point_a.y;
        line_points.push_back(point_a);
        for (int i = point_a.x + 1; i <= point_b.x; ++i)
        {
            cv::Point this_point;
            this_point.x = i;
            y = y + y_inc;
            this_point.y = (int)std::round(y);
            std::cout << "y = " << y << std::endl;
            line_points.push_back(this_point);
        }
        for (size_t i = 0; i < line_points.size(); ++i)
        {
            std::cout << i << "; " << line_points[i] << std::endl;
        }
    }
    else
    {
        if (point_a.y > point_b.y)
        {
            int temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            int temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        float d_x = point_b.x - point_a.x;
        float x_inc = d_x / steps;
        float x = point_a.x;
        line_points.push_back(point_a);
        for (int i = point_a.y + 1; i <= point_b.y; ++i)
        {
            cv::Point this_point;
            this_point.y = i;
            x = x + x_inc;
            this_point.x = (int)std::round(x);
            line_points.push_back(this_point);
        }
        for (size_t i = 0; i < line_points.size(); ++i)
        {
            std::cout << i << "; " << line_points[i] << std::endl;
        }
    }

    return line_points;
}

void draw_line(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour)
{
    std::vector<cv::Point> line_points = get_line_points_AB(point_1, point_2);
    for (size_t i = 0; i < line_points.size(); ++i)
    {
        if (line_points[i].x >= 0 && line_points[i].x < image.cols && line_points[i].y >= 0 && line_points[i].y < image.rows)
        {
            image.at<cv::Vec3b>(line_points[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
        }
    }
}

void draw_line_bresenham(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour)
{
    std::vector<cv::Point> line_points = get_line_points_bresenham(point_1, point_2);
    for (size_t i = 0; i < line_points.size(); ++i)
    {
        image.at<cv::Vec3b>(line_points[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
    }
}

void draw_line_dda(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour)
{
    std::vector<cv::Point> line_points = get_line_points_dda(point_1, point_2);
    for (size_t i = 0; i < line_points.size(); ++i)
    {
        image.at<cv::Vec3b>(line_points[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
    }
}

// ham tra ve nhieu gia tri
void add_value(int& a, int& b, int value)
{
    a = a + value;
    b += value;
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
            if (input_image.type() == CV_8UC1)
            {
                int fx_1c = fx_interpolation_8UC1(input_image, col_input_0, col_input_1, col_input_2, col_input_3, row_input_0, row_input_1, row_input_2, row_input_3, d_x, d_y);
                output_image.at<uchar>(cv::Point(col_output, row_output)) = fx_1c;
            }
            else if (input_image.type() == CV_8UC3)
            {
                cv::Vec3b fx_3c = bicubic_3c(input_image, col_input_0, col_input_1, col_input_2, col_input_3, row_input_0, row_input_1, row_input_2, row_input_3, d_x, d_y);
                output_image.at<cv::Vec3b>(cv::Point(col_output, row_output)) = fx_3c;
            }
        }
    }
    imshow("Input_Image", input_image);
    imshow("Output_Image", output_image);
    cv::waitKey(0);
}

cv::Vec3b bicubic_3c(cv::Mat input_image, int col_input_0, int col_input_1, int col_input_2, int col_input_3, int row_input_0, int row_input_1, int row_input_2, int row_input_3, float d_x, float d_y)
{
    cv::Vec3b fx00_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_0));
    cv::Vec3b fx01_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_1));
    cv::Vec3b fx02_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_2));
    cv::Vec3b fx03_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_0, row_input_3));
    int fx_interpolation_b1 = get_value(fx00_3c[0], fx01_3c[0], fx02_3c[0], fx03_3c[0], d_y);
    int fx_interpolation_g1 = get_value(fx00_3c[1], fx01_3c[1], fx02_3c[1], fx03_3c[1], d_y);
    int fx_interpolation_r1 = get_value(fx00_3c[2], fx01_3c[2], fx02_3c[2], fx03_3c[2], d_y);

    cv::Vec3b fx10_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_0));
    cv::Vec3b fx11_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_1));
    cv::Vec3b fx12_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_2));
    cv::Vec3b fx13_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_1, row_input_3));
    int fx_interpolation_b2 = get_value(fx10_3c[0], fx11_3c[0], fx12_3c[0], fx13_3c[0], d_y);
    int fx_interpolation_g2 = get_value(fx10_3c[1], fx11_3c[1], fx12_3c[1], fx13_3c[1], d_y);
    int fx_interpolation_r2 = get_value(fx10_3c[2], fx11_3c[2], fx12_3c[2], fx13_3c[2], d_y);

    cv::Vec3b fx20_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_2, row_input_0));
    cv::Vec3b fx21_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_2, row_input_1));
    cv::Vec3b fx22_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_2, row_input_2));
    cv::Vec3b fx23_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_2, row_input_3));
    int fx_interpolation_b3 = get_value(fx20_3c[0], fx21_3c[0], fx22_3c[0], fx23_3c[0], d_y);
    int fx_interpolation_g3 = get_value(fx20_3c[1], fx21_3c[1], fx22_3c[1], fx23_3c[1], d_y);
    int fx_interpolation_r3 = get_value(fx20_3c[2], fx21_3c[2], fx22_3c[2], fx23_3c[2], d_y);

    cv::Vec3b fx30_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_3, row_input_0));
    cv::Vec3b fx31_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_3, row_input_1));
    cv::Vec3b fx32_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_3, row_input_2));
    cv::Vec3b fx33_3c = input_image.at<cv::Vec3b>(cv::Point(col_input_3, row_input_3));
    int fx_interpolation_b4 = get_value(fx30_3c[0], fx31_3c[0], fx32_3c[0], fx33_3c[0], d_y);
    int fx_interpolation_g4 = get_value(fx30_3c[1], fx31_3c[1], fx32_3c[1], fx33_3c[1], d_y);
    int fx_interpolation_r4 = get_value(fx30_3c[2], fx31_3c[2], fx32_3c[2], fx33_3c[2], d_y);

    int fx_interpolation_b = get_value(fx_interpolation_b1, fx_interpolation_b2, fx_interpolation_b3, fx_interpolation_b4, d_x);
    int fx_interpolation_g = get_value(fx_interpolation_g1, fx_interpolation_g2, fx_interpolation_g3, fx_interpolation_g4, d_x);
    int fx_interpolation_r = get_value(fx_interpolation_r1, fx_interpolation_r2, fx_interpolation_r3, fx_interpolation_r4, d_x);
    cv::Vec3b fx_3c = cv::Vec3b(fx_interpolation_b, fx_interpolation_g, fx_interpolation_r);
    return fx_3c;
}

int fx_interpolation_8UC1(cv::Mat input_image, int col_input_0, int col_input_1, int col_input_2, int col_input_3, int row_input_0, int row_input_1, int row_input_2, int row_input_3, float d_x, float d_y)
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
