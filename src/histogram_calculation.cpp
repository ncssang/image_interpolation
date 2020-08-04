#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include "opencv2/xfeatures2d.hpp"
#include <sstream>
#include <vector>

int main()
{
    cv::Mat image = cv::imread("Histogram_Calculation_Original_Image.jpg");
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

    //  Yeu cau histogram_resolution = 255x200 = hang so
    // tim max_hist = gia tri lon nhat trong vector histogram_blue, histogram_green , histogram_red
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

    int ratio = max / 200;
    //  =>> max la gia tri lon nhat trong vector histogram_blue, histogram_green , histogram_red
    //
    //
    // tim ratio = max_hist/chieu cao mong muon = 200
    // tinh iga tri lon nha cua vector histogram = max_hist

    cv::Mat image_histogram(cv::Size(255, 200), CV_8UC3, cv::Scalar(0, 0, 0));
    for (size_t i = 0; i < histograms_blue.size(); ++i)
    {
        // image_histogram.at<cv::Vec3b>(cv::Point(i, histograms_blue[i] / ratio)) = cv::Vec3b(255, 0, 0);
        // image_histogram.at<cv::Vec3b>(cv::Point(i, histograms_green[i] / ratio)) = cv::Vec3b(0, 255, 0);
        // image_histogram.at<cv::Vec3b>(cv::Point(i, histograms_red[i] / ratio)) = cv::Vec3b(0, 0, 255);

        cv::line(image_histogram, cv::Point(i, histograms_blue[i] / ratio), cv::Point(i + 1, histograms_blue[i + 1] / ratio), cv::Scalar(255, 0, 0), 1);
        cv::line(image_histogram, cv::Point(i, histograms_green[i] / ratio), cv::Point(i + 1, histograms_green[i + 1] / ratio), cv::Scalar(0, 255, 0), 1);
        cv::line(image_histogram, cv::Point(i, histograms_red[i] / ratio), cv::Point(i + 1, histograms_red[i + 1] / ratio), cv::Scalar(0, 0, 255), 1);
    }
    cv::flip(image_histogram, image_histogram, 0);
    cv::imwrite("image_histogram.png", image_histogram);
    cv::imshow("image_histogram", image_histogram);

    cv::imshow("image", image);
    cv::waitKey(0);
}