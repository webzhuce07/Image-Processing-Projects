#pragma once

#include <opencv2\opencv.hpp>


/*
输入：
guidedImg　　----引导图像,单通道或者三通道
inputImg　　----输入待滤波图像，单通道或者三通道
r　　　　　　----滤波窗口半径
eps　　　　　----截断值eps

输出：
outputImg　　----引导图滤波后图像
*/

class GuidedFilter
{
public:
	GuidedFilter();
	~GuidedFilter();

private:

	cv::Mat runGuidedFilter_Gray(cv::Mat I, cv::Mat P, int type, int radius, double eps);
	cv::Mat runGuidedFilter_Color(cv::Mat I, cv::Mat P, int type, int radius, double eps);

public:
	cv::Mat myGuidedFilter_GrayGuided(cv::Mat guidedImg, cv::Mat inputImg, int radius, double eps);
	cv::Mat myGuidedFilter_ColorGuided(cv::Mat guidedImg, cv::Mat inputImg, int radius, double eps);


};