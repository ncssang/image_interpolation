#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

std::vector<cv::Point> get_line_points(cv::Point point_a, cv::Point point_b)
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
            int y_value = (int)std::floor((a_value * i + b_value) + 0.5);
            this_point.x = i;
            this_point.y = y_value;
            line_points.push_back(this_point);
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
            int x_value = (int)std::floor((a_value * i + b_value) + 0.5);
            this_point.y = i;
            this_point.x = x_value;
            line_points.push_back(this_point);
        }
    }
    return line_points;
}
void draw_line(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour)
{
    std::vector<cv::Point> line_points = get_line_points(point_1, point_2);
    for (size_t i = 0; i < line_points.size(); ++i)
    {
        if(line_points[i].x >=0 && line_points[i].x < image.cols && line_points[i].y >= 0 && line_points[i].y < image.rows)
        {
            image.at<cv::Vec3b>(line_points[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
        }
    }
}
int main()
{
    // cv::Point point_a, point_b;
    // std::cout << "Nhap a.x_1 = ";
    // std::cin >> point_a.x;
    // std::cout << "Nhap a.y_1 = ";
    // std::cin >> point_a.y;
    // std::cout << "Nhap b.x_2 = ";
    // std::cin >> point_b.x;
    // std::cout << "Nhap b.y_2 = ";
    // std::cin >> point_b.y;

    cv::Mat image_draw_line(cv::Size(400, 300), CV_8UC3, cv::Scalar(0, 0, 0));
    draw_line(image_draw_line, cv::Point(45, 45), cv::Point(1500, 150), cv::Scalar(0, 100, 255));
    cv::flip(image_draw_line, image_draw_line, 0);
    cv::imwrite("draw_line.png", image_draw_line);
    cv::imshow("image_draw_line", image_draw_line);
    cv::waitKey(0);
}