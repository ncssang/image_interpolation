#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

struct Pixel
{
    float column;
    float row;
    Pixel(const float column = 0, const float row = 0) : column(column), row(row)
    {
    }
};

int get_value(int point_0, int point_1, int point_2, int point_3, float x)
{
    float a = -0.5 * point_0 + 1.5 * point_1 - 1.5 * point_2 + 0.5 * point_3;
    float b = point_0 - 2.5 * point_1 + 2 * point_2 - 0.5 * point_3;
    float c = -0.5 * point_0 + 0.5 * point_2;
    float d = point_1;
    int value = (int)std::floor(d + x * (c + x * (b + x * a)));
    if (value < 0)
    {
        value = 0;
    }
    else if (value > 255)
    {
        value = 255;
    }
    return value;
}

void resize_bicubic(cv::Mat& input_image, cv::Mat& output_image, const cv::Size& size)
{
    std::vector<Pixel> pixel_map;
    size_t map_size = size.width * size.height;
    pixel_map.reserve(map_size);
    for (size_t i = 0; i < map_size; ++i)
    {
        int column_new = i % size.width;
        int row_new = i / size.width;
        float column = ((float)(input_image.cols) / (size.width) * (column_new + 0.5)) - 0.5;
        float row = ((float)(input_image.rows) / (size.height) * (row_new + 0.5)) - 0.5;
        pixel_map.emplace_back(Pixel(column, row));
    }
    int number_of_channels = input_image.channels();
    int image_type = input_image.type();
    output_image.create(size, image_type);

    for (int row_output = 0; row_output < output_image.cols; ++row_output)
    {
        for (int col_output = 0; col_output < output_image.rows; ++col_output)
        {
            int output_index = row_output * output_image.cols + col_output;
            float input_image_row = pixel_map[output_index].row;
            float input_image_col = pixel_map[output_index].column;
            int input_x0, input_x1, input_x2, input_x3;
            if (std::floor(input_image_col) == -1)
            {
                input_x0 = 0;
                input_x1 = 0;
                input_x2 = 0;
                input_x3 = 1;
            }
            else if (std::floor(input_image_col) == 0)
            {
                input_x0 = 0;
                input_x1 = 0;
                input_x2 = 1;
                input_x3 = 2;
            }
            else if (std::floor(input_image_col) == input_image.cols - 2)
            {
                input_x0 = input_image.cols - 3;
                input_x1 = input_image.cols - 2;
                input_x2 = input_image.cols - 1;
                input_x3 = input_image.cols - 1;
            }
            else if (std::floor(input_image_col) == input_image.cols - 1)
            {
                input_x0 = input_image.cols - 2;
                input_x1 = input_image.cols - 1;
                input_x2 = input_image.cols - 1;
                input_x3 = input_image.cols - 1;
            }
            else
            {
                input_x0 = std::floor(input_image_col) - 1;
                input_x1 = input_x0 + 1;
                input_x2 = input_x0 + 2;
                input_x3 = input_x0 + 3;
            }
            int input_y0, input_y1, input_y2, input_y3;
            if (std::floor(input_image_row) == -1)
            {
                input_y0 = 0;
                input_y1 = 0;
                input_y2 = 0;
                input_y3 = 1;
            }
            else if (std::floor(input_image_row) == 0)
            {
                input_y0 = 0;
                input_y1 = 0;
                input_y2 = 1;
                input_y3 = 2;
            }
            else if (std::floor(input_image_row) == input_image.rows - 2)
            {
                input_y0 = input_image.rows - 3;
                input_y1 = input_image.rows - 2;
                input_y2 = input_image.rows - 1;
                input_y3 = input_image.rows - 1;
            }
            else if (std::floor(input_image_row) == input_image.rows - 1)
            {
                input_y0 = input_image.rows - 2;
                input_y1 = input_image.rows - 1;
                input_y2 = input_image.rows - 1;
                input_y3 = input_image.rows - 1;
            }
            else
            {
                input_y0 = std::floor(input_image_row) - 1;
                input_y1 = input_y0 + 1;
                input_y2 = input_y0 + 2;
                input_y3 = input_y0 + 3;
            }
            float d_x = input_image_col - std::floor(input_image_col);
            float d_y = input_image_row - std::floor(input_image_row);
            for (int i = 0; i < number_of_channels; ++i)
            {
                uchar source_x_y_0 = get_value(input_image.data[(input_y0 * input_image.cols + input_x0) * number_of_channels + i],
                                               input_image.data[(input_y0 * input_image.cols + input_x1) * number_of_channels + i],
                                               input_image.data[(input_y0 * input_image.cols + input_x2) * number_of_channels + i],
                                               input_image.data[(input_y0 * input_image.cols + input_x3) * number_of_channels + i],
                                               d_x);
                uchar source_x_y_1 = get_value(input_image.data[(input_y1 * input_image.cols + input_x0) * number_of_channels + i],
                                               input_image.data[(input_y1 * input_image.cols + input_x1) * number_of_channels + i],
                                               input_image.data[(input_y1 * input_image.cols + input_x2) * number_of_channels + i],
                                               input_image.data[(input_y1 * input_image.cols + input_x3) * number_of_channels + i],
                                               d_x);
                uchar source_x_y_2 = get_value(input_image.data[(input_y2 * input_image.cols + input_x0) * number_of_channels + i],
                                               input_image.data[(input_y2 * input_image.cols + input_x1) * number_of_channels + i],
                                               input_image.data[(input_y2 * input_image.cols + input_x2) * number_of_channels + i],
                                               input_image.data[(input_y2 * input_image.cols + input_x3) * number_of_channels + i],
                                               d_x);
                uchar source_x_y_3 = get_value(input_image.data[(input_y3 * input_image.cols + input_x0) * number_of_channels + i],
                                               input_image.data[(input_y3 * input_image.cols + input_x1) * number_of_channels + i],
                                               input_image.data[(input_y3 * input_image.cols + input_x2) * number_of_channels + i],
                                               input_image.data[(input_y3 * input_image.cols + input_x3) * number_of_channels + i],
                                               d_x);
                uchar source_value = get_value(source_x_y_0, source_x_y_1, source_x_y_2, source_x_y_3, d_y);
                output_image.data[output_index * number_of_channels + i] = source_value;
            }
        }
    }
}

int main()
{
    cv::VideoCapture cap(0);
    if (cap.isOpened() == false)
    {
        std::cout << "Cannot open the video file" << std::endl;
        std::cin.get();
        return -1;
    }
    float ratio;
    do
    {
        std::cout << "enter ratio: " << std::endl;
        std::cin >> ratio;
    } while (ratio <= 0);
    
    int frame_width = static_cast<int>(cap.get(3));
    int frame_height = static_cast<int>(cap.get(4));
    cv::Size frame_size(frame_width * ratio, frame_height * ratio);
    int frames_per_second = 1;
    cv::VideoWriter oVideoWriter("/home/nq/workspace/image_interpolation/MyVideo.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                 frames_per_second, frame_size, true);
    if (oVideoWriter.isOpened() == false)
    {
        std::cout << "Cannot save the video to a file" << std::endl;
        std::cin.get(); //wait for any key press
        return -1;
    };
    // double fps = cap.get(cv::CAP_PROP_FPS);
    // std::cout << "Frames per seconds : " << fps << std::endl;
    // cv::String window_name = "My First Video";
    // namedWindow(window_name, cv::WINDOW_NORMAL);
    // int frame_count = 0;
    while (true)
    {
        cv::Mat frame;
        bool bSuccess = cap.read(frame);
        if (bSuccess == false)
        {
            std::cout << "Found the end of the video" << std::endl;
            break;
        }
        cv::Mat new_frame;
        cv::Size new_size(frame.cols * ratio, frame.rows * ratio);
        resize_bicubic(frame, new_frame, new_size);
        oVideoWriter.write(new_frame); 
        imshow("new_frame", new_frame);
        // ++frame_count;
        // std::string ss_frame_count = std::to_string(frame_count);
        // cv::putText(frame, ss_frame_count, cv::Point(25, 25), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 0, 255), 1, CV_AA);
        // cv::Size sz = frame.size();
        // int imageWith = sz.width;
        // std::string image_With = std::to_string(imageWith);
        // int imageHigh = sz.height;
        // std::string image_High = std::to_string(imageHigh);
        // std::string multiple = "x";
        // std::string with_high = image_With + multiple + image_High;
        // cv::putText(frame, with_high, cv::Point(imageWith - 100, imageHigh - 40), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(0, 0, 255), 1, CV_AA);
        // imshow(window_name, frame);
        if (cv::waitKey(10) == 27)
        {
            std::cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
            break;
        }
    }

    return 0;
}