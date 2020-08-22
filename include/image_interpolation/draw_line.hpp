#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <vector>

std::vector<cv::Point> get_line_points_AB(cv::Point point_a, cv::Point point_b);
std::vector<cv::Point> get_line_points_bresenham(cv::Point point_a, cv::Point point_b);
std::vector<cv::Point> get_line_points_dda(cv::Point point_a, cv::Point point_b);

void draw_line(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour);
void draw_line_bresenham(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour);
void draw_line_dda(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour);

#endif //DRAW_LINE_HPP