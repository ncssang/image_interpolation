#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include "opencv2/xfeatures2d.hpp"
#include <sstream>
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
            float temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            float temp_y = point_a.y;
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
            float temp_x = point_a.x;
            point_a.x = point_b.x;
            point_b.x = temp_x;
            float temp_y = point_a.y;
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
    cv::Mat image = cv::imread("Capture.PNG");
    if (image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
    }
    // cv::Size sz = image.size();
    // int imageWidth = sz.width;
    // int imageHeigh = sz.height;
    // std::cout << imageWidth << ": " << imageHeigh << std::endl;
    std::cout << image.cols << " " << image.rows << std::endl;
    std::vector<int> histograms_blue(256, 0);
    std::vector<int> histograms_green(256, 0);
    std::vector<int> histograms_red(256, 0);
    for (int row = 0; row < image.rows; ++row)
    {
        for (int col = 0; col < image.cols; ++col)
        {
            cv::Vec3b intensity = image.at<cv::Vec3b>(cv::Point(col, row));
            uchar blue = intensity.val[0];
            histograms_blue[blue]++;
            uchar green = intensity.val[1];
            histograms_green[green]++;
            uchar red = intensity.val[2];
            histograms_red[red]++;
        }
    }
    int histogram_blue_max = 0;
    for (size_t i = 0; i < histograms_blue.size(); ++i)
    {
        if (histogram_blue_max < histograms_blue[i])
        {
            histogram_blue_max = histograms_blue[i];
        }
    }
    int histogram_green_max = 0;
    for (size_t i = 0; i < histograms_green.size(); ++i)
    {
        if (histogram_green_max < histograms_green[i])
        {
            histogram_green_max = histograms_green[i];
        }
    }
    int histogram_red_max = 0;
    for (size_t i = 0; i < histograms_red.size(); ++i)
    {
        if (histogram_red_max < histograms_red[i])
        {
            histogram_red_max = histograms_red[i];
        }
    }
    int max = 0;
    if (max < histogram_blue_max)
    {
        max = histogram_blue_max;
    }
    if (max < histogram_green_max)
    {
        max = histogram_green_max;
    }
    if (max < histogram_red_max)
    {
        max = histogram_red_max;
    }
    std::cout << "Max = " << max << std::endl;
    float ratio = max / 200;
    std::ofstream output_file_stream_blue;
    output_file_stream_blue.open("histogram_blue.csv");
    for (size_t i = 0; i < histograms_blue.size(); ++i)
    {
        output_file_stream_blue << i << "," << histograms_blue[i] << std::endl;
    }
    output_file_stream_blue.close();
    std::ofstream output_file_stream_green;
    output_file_stream_green.open("histogram_green.csv");
    for (size_t i = 0; i < histograms_green.size(); ++i)
    {
        output_file_stream_green << i << "," << histograms_green[i] << std::endl;
    }
    output_file_stream_green.close();
    std::ofstream output_file_stream_red;
    output_file_stream_red.open("histogram_red.csv");
    for (size_t i = 0; i < histograms_red.size(); ++i)
    {
        output_file_stream_red << i << "," << histograms_red[i] << std::endl;
    }
    output_file_stream_red.close();

    cv::Mat image_histogram(cv::Size(255, 200), CV_8UC3, cv::Scalar(0, 0, 0));
    for (size_t i = 0; i < histograms_blue.size() - 1; ++i)
    {
        cv::Point point_a, point_b;
        point_a.x = i;
        point_a.y = (int)std::floor((histograms_blue[i] / ratio) + 0.5);
        std::cout << "a_x: " << point_a.x << ", a_y: " << point_a.y << std::endl;
        point_b.x = i + 1;
        point_b.y = (int)std::floor((histograms_blue[i + 1] / ratio) + 0.5);
        std::cout << "b_x: " << point_b.x << ", b_y: " << point_b.y << std::endl;
        draw_line(image_histogram, cv::Point(point_a), cv::Point(point_b), cv::Scalar(255, 0, 0));

        cv::Point point_c, point_d;
        point_c.x = i;
        point_c.y = (int)std::floor((histograms_green[i] / ratio) + 0.5);
        point_d.x = i + 1;
        point_d.y = (int)std::floor((histograms_green[i + 1] / ratio) + 0.5);
        draw_line(image_histogram, cv::Point(point_c), cv::Point(point_d), cv::Scalar(0, 255, 0));

        cv::Point point_e, point_f;
        point_e.x = i;
        point_e.y = (int)std::floor((histograms_red[i] / ratio) + 0.5);
        point_f.x = i + 1;
        point_f.y = (int)std::floor((histograms_red[i + 1] / ratio) + 0.5);
        draw_line(image_histogram, cv::Point(point_e), cv::Point(point_f), cv::Scalar(0, 0, 255));
        // image_histogram.at<cv::Vec3b>(cv::Point(i, histograms_blue[i] / ratio)) = cv::Vec3b(255, 0, 0);
        // image_histogram.at<cv::Vec3b>(cv::Point(i, histograms_green[i] / ratio)) = cv::Vec3b(0, 255, 0);
        // image_histogram.at<cv::Vec3b>(cv::Point(i, histograms_red[i] / ratio)) = cv::Vec3b(0, 0, 255);

        // cv::line(image_histogram, cv::Point(i, histograms_blue[i] / ratio), cv::Point(i + 1, histograms_blue[i + 1] / ratio), cv::Scalar(255, 0, 0), 1);
        // cv::line(image_histogram, cv::Point(i, histograms_green[i] / ratio), cv::Point(i + 1, histograms_green[i + 1] / ratio), cv::Scalar(0, 255, 0), 1);
        // cv::line(image_histogram, cv::Point(i, histograms_red[i] / ratio), cv::Point(i + 1, histograms_red[i + 1] / ratio), cv::Scalar(0, 0, 255), 1);
    }
    cv::flip(image_histogram, image_histogram, 0);
    cv::imwrite("image_histogram.png", image_histogram);
    cv::imshow("image_histogram", image_histogram);
    cv::imshow("image", image);
    cv::waitKey(0);
    return 0;
}