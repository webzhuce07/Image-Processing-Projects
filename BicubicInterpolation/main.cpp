#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat srcImage = imread("coins.png",0);
	Mat dstImage;
	resize(srcImage, dstImage, cv::Size(0, 0), 4.0, 4.0, INTER_CUBIC);
	imshow("Origin", srcImage);
	imshow("Bicubic", dstImage);
	waitKey(0);
	return 0;
}