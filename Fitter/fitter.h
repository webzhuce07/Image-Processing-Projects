#pragma once
#include <opencv2\opencv.hpp>

class PolyFitter
{
public:
	PolyFitter(const std::vector<cv::Point> &points, int n);
	int evalute(int x) const;
	static cv::Mat PolyFit(const std::vector<cv::Point> &points, int n);
private:
	int order_;
	cv::Mat mat_;
};

//Quadratic polynomial
class SurfaceFitter
{
public:
	SurfaceFitter(const std::vector<cv::Point3i> &points);
	int evalute(int x, int y) const;
	static cv::Mat PolyFit(const std::vector<cv::Point3i> &points);

private:
	cv::Mat mat_;
};




