#include <iostream>
#include <opencv2/opencv.hpp>


int main()
{
    cv::Mat image = cv::imread("/home/nq/workspace/hello_cv/index.jpeg");
    if (image.empty())
    {
        std::cout << "Could not open or find the image" << std::endl;
        std::cin.get();
    }
    /*
 Make changes to the image as necessary
 e.g.  
  1. Change brightness/contrast of the image
  2. Smooth/Blur image
  3. Crop the image
  4. Rotate the image
  5. Draw shapes on the image
 */
   bool isSuccess = cv::imwrite("/home/nq/workspace/image_interpolation/MyImage.jpg", image);
    //bool isSuccess = cv::imwrite("D:/MyImage.png", image);
    if (isSuccess == false)
    {
        std::cout << "Failed to save the image" << std::endl;
        std::cin.get();
        return -1;
    }
    else
    {
        std::cout << "Image is succusfully saved to a file" << std::endl;
    }
    cv::String windowName = "The Saved Image"; //Name of the window
    cv::namedWindow(windowName);                   // Create a window
    imshow(windowName, image);                 // Show our image inside the created window.
    cv::waitKey(0); // Wait for any keystroke in the window
    cv::destroyWindow(windowName); //destroy the created window
    return 0;
}