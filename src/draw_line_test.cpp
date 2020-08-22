#include "image_interpolation/draw_line.hpp"

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
    // draw_line(image_draw_line, cv::Point(45, 45), cv::Point(150, 150), cv::Scalar(0, 100, 255));
    draw_line_dda(image_draw_line, cv::Point(45, 100), cv::Point(50, 20), cv::Scalar(0, 100, 255));
    // draw_line_bresenham(image_draw_line, cv::Point(45, 100), cv::Point(6, 4), cv::Scalar(0, 100, 255));

    cv::flip(image_draw_line, image_draw_line, 0);
    cv::imwrite("draw_line.png", image_draw_line);
    cv::imshow("image_draw_line", image_draw_line);
    cv::waitKey(0);
}