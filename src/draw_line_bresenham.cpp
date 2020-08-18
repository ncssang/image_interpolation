#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <vector>

std::vector<cv::Point> get_line_points(cv::Point point_a, cv::Point point_b)
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
            float temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            float temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        float d_x = point_b.x - point_a.x;
        float d_y = point_b.y - point_a.y;
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
            float temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            float temp_y = point_a.y;
            point_a.y = point_b.y;
            point_b.y = temp_y;
        }
        float d_x = point_b.x - point_a.x;
        float d_y = point_b.y - point_a.y;
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
void draw_line_bresenham(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour)
{
    std::vector<cv::Point> line_points = get_line_points(point_1, point_2);
    for (size_t i = 0; i < line_points.size(); ++i)
    {
        image.at<cv::Vec3b>(line_points[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
    }
}

int main()
{
    cv::Point point_a, point_b;
    std::cout << "Nhap a.x_1 = ";
    std::cin >> point_a.x;
    std::cout << "Nhap a.y_1 = ";
    std::cin >> point_a.y;
    std::cout << "Nhap b.x_2 = ";
    std::cin >> point_b.x;
    std::cout << "Nhap b.y_2 = ";
    std::cin >> point_b.y;

    cv::Mat image_draw_line_bresenham(cv::Size(400, 300), CV_8UC3, cv::Scalar(0, 0, 0));
    draw_line_bresenham(image_draw_line_bresenham, cv::Point(point_a), cv::Point(point_b), cv::Scalar(0, 255, 0));
    cv::flip(image_draw_line_bresenham, image_draw_line_bresenham, 0);
    cv::imwrite("image_draw_line_bresenham.png", image_draw_line_bresenham);
    cv::imshow("image_draw_line_bresenham", image_draw_line_bresenham);
    cv::waitKey(0);
}
