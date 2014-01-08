#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[]) {
    std::cout << "Hello my world!";

    // read an image
    cv::Mat image= cv::imread("C:/Users/Jeffrey/Documents/GitHub/Scanner/OpenCVTest/img.jpg",-1);
    // create image window named "My Image"
      cv::namedWindow("Image:");
    // show the image on window
    cv::imshow("Image:", image);
    // wait key for 5000 ms
    cv::waitKey(0);
    return 0;
}
