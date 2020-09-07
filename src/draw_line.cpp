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
