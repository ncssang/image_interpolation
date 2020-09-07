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
std::vector<cv::Point> get_circle_points_bresenham(cv::Point point_a, int r)
{
    std::vector<cv::Point> circle_points;
    cv::Point point_1;
    point_1.x = point_a.x - r;
    point_1.y = point_a.y;
    circle_points.push_back(point_1);
    cv::Point point_2;
    point_2.x = point_a.x + r;
    point_2.y = point_a.y;
    circle_points.push_back(point_2);
    cv::Point point_3;
    point_3.x = point_a.x;
    point_3.y = point_a.y + r;
    circle_points.push_back(point_3);
    cv::Point point_4;
    point_4.x = point_a.x;
    point_4.y = point_a.y - r;
    circle_points.push_back(point_4);

    cv::Point this_point;
    this_point.x = 0;
    this_point.y = r;
    int p = 3 - 2 * r;
    while (this_point.x <= this_point.y)
    {
        if (p < 0)
        {
            p = p + 4 * this_point.x + 6;
        }
        else
        {
            p = p + 4 * (this_point.x - this_point.y) + 10;
            --this_point.y;
        }
        ++this_point.x;
        add_points(circle_points, point_a, this_point);
    }

    return circle_points;
}
void draw_circle(cv::Mat image_circle, cv::Point point_1, int r, cv::Scalar colour)
{
    std::vector<cv::Point> circle_points = get_circle_points_bresenham(point_1, r);
    for (size_t i = 0; i < circle_points.size(); ++i)
    {
        if (circle_points[i].x >= 0 && circle_points[i].x < image_circle.cols && circle_points[i].y >= 0 && circle_points[i].y < image_circle.rows)
        {
            image_circle.at<cv::Vec3b>(circle_points[i]) = cv::Vec3b(colour[0], colour[1], colour[2]);
        }
    }
}

int main()
{
    cv::Point point_a;
    std::cout << "Nhap a.x_1 = ";
    std::cin >> point_a.x;
    std::cout << "Nhap a.y_1 = ";
    std::cin >> point_a.y;
    int r;
    std::cout << "Nhap r = ";
    std::cin >> r;
    cv::Mat image_draw_circle(cv::Size(800, 800), CV_8UC3, cv::Scalar(0, 0, 0));
    draw_circle(image_draw_circle, cv::Point(point_a), r, cv::Scalar(0, 100, 255));
    // cv::circle(image_draw_circle, point_a, r,cv::Scalar(0, 100, 255));
    cv::flip(image_draw_circle, image_draw_circle, 0);
    cv::imwrite("draw_circle.png", image_draw_circle);
    cv::imshow("image_draw_circle", image_draw_circle);
    cv::waitKey(0);
}
