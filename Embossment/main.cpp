#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int calmp(int value, int minValue = 0, int maxValue = 255) {
	if (value < minValue)
		return minValue;
	else if (value > maxValue)
		return maxValue;
	return value;
}

void reliefTrans(Mat& src, Mat& dst) {
	dst = src.clone();
	int rowNumber = dst.rows;
	int colNumber = dst.cols;

	if (src.channels() == 1)
	{
		for (int i = 1; i < rowNumber - 1; ++i) {
			for (int j = 1; j < colNumber - 1; ++j) {
				dst.at<uchar>(i, j) = calmp(src.at<uchar>(i + 1, j + 1) - src.at<uchar>(i - 1, j - 1) + 128);
			}
		}
	}
	else if (src.channels() == 3) {//color image
		for (int i = 1; i < rowNumber - 1; ++i) {
			for (int j = 1; j < colNumber - 1; ++j) {
				for (int n = 0; n < src.channels(); ++n) {
					dst.at<cv::Vec3b>(i, j)[n] = calmp(src.at<cv::Vec3b>(i + 1, j + 1)[n] - src.at<cv::Vec3b>(i - 1, j - 1)[n] + 128);
				}
			}
		}
	}
}

int main() {
	Mat srcImage = imread("coins.png",0);
	Mat dstImage;
	reliefTrans(srcImage, dstImage);
	imshow("Origin", srcImage);
	imshow("Embossment", dstImage);
	waitKey(0);
	return 0;
}