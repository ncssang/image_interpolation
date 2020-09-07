#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sstream>
#include <vector>

void add_points(std::vector<cv::Point>& circle_points, cv::Point point_a, cv::Point this_point)
{
    cv::Point this_point_1;
    this_point_1.x = this_point.x + point_a.x;
    this_point_1.y = this_point.y + point_a.y;
    circle_points.push_back(this_point_1);
    cv::Point this_point_2;
    this_point_2.x = -this_point.x + point_a.x;
    this_point_2.y = this_point.y + point_a.y;
    circle_points.push_back(this_point_2);
    cv::Point this_point_3;
    this_point_3.x = this_point.x + point_a.x;
    this_point_3.y = -this_point.y + point_a.y;
    circle_points.push_back(this_point_3);
    cv::Point this_point_4;
    this_point_4.x = -this_point.x + point_a.x;
    this_point_4.y = -this_point.y + point_a.y;
    circle_points.push_back(this_point_4);
    cv::Point this_point_5;
    this_point_5.x = this_point.y + point_a.x;
    this_point_5.y = this_point.x + point_a.y;
    circle_points.push_back(this_point_5);
    cv::Point this_point_6;
    this_point_6.x = -this_point.y + point_a.x;
    this_point_6.y = this_point.x + point_a.y;
    circle_points.push_back(this_point_6);
    cv::Point this_point_7;
    this_point_7.x = this_point.y + point_a.x;
    this_point_7.y = -this_point.x + point_a.y;
    circle_points.push_back(this_point_7);
    cv::Point this_point_8;
    this_point_8.x = -this_point.y + point_a.x;
    this_point_8.y = -this_point.x + point_a.y;
    circle_points.push_back(this_point_8);
}

std::vector<cv::Point> get_circle_points_square(cv::Point point_a, int r)
{
    std::vector<cv::Point> circle_points_square;
    std::vector<cv::Point> circle_points;
    cv::Point point_1;
    point_1.x = point_a.x - r;
    point_1.y = point_a.y;
    circle_points_square.push_back(point_1);
    cv::Point point_2;
    point_2.x = point_a.x + r;
    point_2.y = point_a.y;
    circle_points_square.push_back(point_2);
    cv::Point point_3;
    point_3.x = point_a.x;
    point_3.y = point_a.y + r;
    circle_points_square.push_back(point_3);
    cv::Point point_4;
    point_4.x = point_a.x;
    point_4.y = point_a.y - r;
    circle_points_square.push_back(point_4);
    cv::Point this_point;
    this_point.x = 0;
    this_point.y = r;
    while (this_point.x <= this_point.y)
    {
        ++this_point.x;
        this_point.y = (int)std::round(sqrt(r * r - this_point.x * this_point.x));
        add_points(circle_points_square, point_a, this_point);
    }
    return circle_points_square;
}

void draw_circle_square(cv::Mat image_circle, cv::Point point_1, int r, cv::Scalar colour)
{
    std::vector<cv::Point> circle_points_square = get_circle_points_square(point_1, r);
    for (size_t i = 0; i < circle_points_square.size(); ++i)
    {
        if (circle_points_square[i].x >= 0 && circle_points_square[i].x < image_circle.cols && circle_points_square[i].y >= 0 && circle_points_square[i].y < image_circle.rows)
        {
            image_circle.at<cv::Vec3b>(circle_points_square[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
        }
    }
}

std::vector<cv::Point> get_circle_points(cv::Point point_a, int r)
{
    std::vector<cv::Point> circle_points;
    for (float i = 0; i <= M_PI_4; i = i + (1.0 / r))
    {
        cv::Point this_point;
        this_point.x = (int)std::round(cos(i) * r);
        this_point.y = (int)std::round(sin(i) * r);
        add_points(circle_points, point_a, this_point);
    }
    return circle_points;
}

void draw_circle_polar(cv::Mat image, cv::Point centre, int radius, cv::Scalar colour)
{
    std::vector<cv::Point> circle_points = get_circle_points(centre, radius);
    for (size_t i = 0; i < circle_points.size(); ++i)
    {
        if (circle_points[i].x >= 0 && circle_points[i].x < image.cols && circle_points[i].y >= 0 && circle_points[i].y < image.rows)
        {
            image.at<cv::Vec3b>(circle_points[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
        }
    }
}

int main()
{
    std::cout << M_PI << std::endl;
    std::cout << sin(M_PI_2) << std::endl;
    cv::Point point_a;
    std::cout << "Nhap a.x_1 = ";
    std::cin >> point_a.x;
    std::cout << "Nhap a.y_1 = ";
    std::cin >> point_a.y;
    int r;
    std::cout << "Nhap r = ";
    std::cin >> r;
    cv::Mat image_draw_circle_square(cv::Size(800, 800), CV_8UC3, cv::Scalar(0, 0, 0));
    // draw_circle_square(image_draw_circle_square, cv::Point(point_a), r, cv::Scalar(0, 100, 255));
    draw_circle_polar(image_draw_circle_square, cv::Point(point_a), r, cv::Scalar(0, 100, 255));
    // cv::circle(image_draw_circle, point_a, r,cv::Scalar(0, 100, 255));
    cv::flip(image_draw_circle_square, image_draw_circle_square, 0);
    cv::imwrite("draw_circle_square.png", image_draw_circle_square);
    cv::imshow("image_draw_circle_square", image_draw_circle_square);
    cv::waitKey(0);
}