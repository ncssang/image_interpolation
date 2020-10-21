#ifndef DRAW_LINE_HPP
#define DRAW_LINE_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <vector>
#include <time.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

std::vector<cv::Point> get_line_points_AB(cv::Point point_a, cv::Point point_b);
std::vector<cv::Point> get_line_points_bresenham(cv::Point point_a, cv::Point point_b);
std::vector<cv::Point> get_line_points_dda(cv::Point point_a, cv::Point point_b);

void draw_line(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour);
void draw_line_bresenham(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour);
void draw_line_dda(cv::Mat image, cv::Point point_1, cv::Point point_2, cv::Scalar colour);

int get_value(int point_0, int point_1, int point_2, int point_3, float x);
int fx_interpolation_8UC1(cv::Mat input_image, int col_input_0, int col_input_1, int col_input_2, int col_input_3, int row_input_0, int row_input_1, int row_input_2, int row_input_3, float d_x, float d_y);
cv::Vec3b bicubic_3c(cv::Mat input_image, int col_input_0, int col_input_1, int col_input_2, int col_input_3, int row_input_0, int row_input_1, int row_input_2, int row_input_3, float d_x, float d_y);
void resize_bicubic(cv::Mat input_image, cv::Mat output_image);

#endif //DRAW_LINE_HPP