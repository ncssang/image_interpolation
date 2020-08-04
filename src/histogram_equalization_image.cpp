#include <opencv2/opencv.hpp>
#include <iostream>


int main()
{
    cv::Mat image = cv::imread("/home/nq/Desktop/tmp/index.jpeg");
    if (image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
    }

        //Convert the image from BGR to YCrCb color space
    cv::Mat hist_equalized_image;
    cvtColor(image, hist_equalized_image, cv::COLOR_BGR2YCrCb);

    //Split the image into 3 channels; Y, Cr and Cb channels respectively and store it in a std::vector
    std::vector<cv::Mat> vec_channels;
    split(hist_equalized_image, vec_channels); 

    //Equalize the histogram of only the Y channel 
    equalizeHist(vec_channels[0], vec_channels[0]);

    //Merge 3 channels in the vector to form the color image in YCrCB color space.
    merge(vec_channels, hist_equalized_image); 
        
    //Convert the histogram equalized image from YCrCb to BGR color space again
    cvtColor(hist_equalized_image, hist_equalized_image, cv::COLOR_YCrCb2BGR);

    //Define the names of windows
    cv::String windowNameOfOriginalImage = "Original Image"; 
    cv::String windowNameOfHistogramEqualized = "Histogram Equalized Color Image";

    // Create windows with the above names
    namedWindow(windowNameOfOriginalImage, cv::WINDOW_NORMAL);
    namedWindow(windowNameOfHistogramEqualized, cv::WINDOW_NORMAL);

    // Show images inside the created windows.
    imshow(windowNameOfOriginalImage, image);
    imshow(windowNameOfHistogramEqualized, hist_equalized_image);

    cv::waitKey(0); // Wait for any keystroke in any one of the windows

    cv::destroyAllWindows();
 
    // cvtColor(image, image, cv::COLOR_BGR2GRAY); 

    // //equalize the histogram
    // cv::Mat hist_equalized_image;
    // equalizeHist(image, hist_equalized_image); 
                                               
    // //Define names of windows
    // cv::String windowNameOfOriginalImage = "Original Image"; 
    // cv::String windowNameOfHistogramEqualized = "Histogram Equalized Image";

    // // Create windows with the above names
    // namedWindow(windowNameOfOriginalImage, cv::WINDOW_NORMAL);
    // namedWindow(windowNameOfHistogramEqualized, cv::WINDOW_NORMAL);

    // // Show images inside created windows.
    // imshow(windowNameOfOriginalImage, image);
    // imshow(windowNameOfHistogramEqualized, hist_equalized_image);
    // cv::waitKey(0);
    // cv::destroyAllWindows();

    return 0;
}