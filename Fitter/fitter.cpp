#include "fitter.h"
#include <numeric>

using namespace cv;

PolyFitter::PolyFitter(const std::vector<cv::Point> &points, int n)
{
	order_ = n;
	mat_ = PolyFit(points, n);
}

int PolyFitter::evalute(int x) const
{
	double y = 0;
	for (int j = 0; j < order_ + 1; ++j)
	{
		y += mat_.at<double>(j, 0) * pow(x, j);
	}
	return std::round(y);
}

cv::Mat PolyFitter::PolyFit(const std::vector<cv::Point> &points, int n)
{
	int size = points.size();
	int x_num = n + 1;
	Mat mat_u(size, x_num, CV_64F);
	Mat mat_y(size, 1, CV_64F);

	for (int i = 0; i < mat_u.rows; ++i)
	{
		for (int j = 0; j < mat_u.cols; ++j)
		{
			mat_u.at<double>(i, j) = pow(points[i].x, j);
		}
	}
		
	for (int i = 0; i < mat_y.rows; ++i)
	{
		mat_y.at<double>(i, 0) = points[i].y;
	}

	Mat mat_k(x_num, 1, CV_64F);
	mat_k = (mat_u.t()*mat_u).inv()*mat_u.t()*mat_y;
	//std::cout << mat_k << std::endl;
	return mat_k;
}

std::vector<int> GradientThreshold(const SMARTFACE::VolumeData &volume, const int size)
{
	const float kRatio = 0.2;
	const int kCols = volume.getDim(0);
	const int kRows = volume.getDim(1) * (1 - kRatio);
	const int kDeps = volume.getDim(2);

	const int kOffset = size / 2;
	std::vector<int> amnoticssums;
	std::vector<int> bonesums;
	uchar pixel = 0;
	for (int i = kDeps * kRatio; i < kDeps * (1 - kRatio); i++)
	{
		int maxgradient = 0; 
		int maxupsum = 0;
		int maxdownsum = 0;
		for (int j = kCols * kRatio; j < kCols * (1 - kRatio); j++)
		{
			int upsum = 0, downsum = 0;
			int start = 0;
			for (int k = 0; k < kRows - kOffset; k++)
			{
				pixel = volume(j, k, i);
				if (pixel > 0)
				{
					start = k;
					break;
				}
			}
			if(start + 2 * size > kRows)
				continue;;
			for (int k = start + kOffset; k < kRows - kOffset; k++)
			{
				pixel = volume(j, k + kOffset, i);
				if (pixel == 0)
					break;
				if (k == start + kOffset)
				{
					for (int m = 1; m <= kOffset; m++)
					{
						pixel = volume(j, k - m, i);
						downsum += pixel;
						pixel = volume(j, k + m, i);
						upsum += pixel;
					}
				}
				else
				{
					pixel = volume(j, k - kOffset + 1, i);
					downsum -= pixel;
					pixel = volume(j, k-1, i);
					downsum += pixel;
					pixel = volume(j, k, i);
					upsum -= pixel;
					pixel = volume(j, k+kOffset, i);
					upsum += pixel;
				}
				if (maxgradient < (upsum - downsum))
				{
					maxgradient = (upsum - downsum);
					maxupsum = upsum;
					maxdownsum = downsum;
				}	
			}
			if (maxgradient > kOffset * 10)
			{
				amnoticssums.push_back(maxdownsum / kOffset);
				bonesums.push_back(maxupsum / kOffset);
			}
		}
	}
	std::sort(amnoticssums.begin(), amnoticssums.end(), std::greater<int>());
	std::sort(bonesums.begin(), bonesums.end());
	int n = amnoticssums.size() * 0.5;

	int amnotic = std::accumulate(amnoticssums.begin(), amnoticssums.begin() + n, 0) / n ;
	int bone = std::accumulate(bonesums.begin(), bonesums.begin() + n, 0) / n ;
	
	std::vector<int> result;
	result.push_back(amnotic);
	result.push_back(bone);
	return result;
}

SurfaceFitter::SurfaceFitter(const std::vector<cv::Point3i> &points)
{
	mat_ = PolyFit(points);
}

int SurfaceFitter::evalute(int x, int y) const
{
	float x2 = std::pow(x, 2);
	float y2 = std::pow(y, 2);
	float xy = x * y;
	// a0 + a1 * x + a2 * y + a3 * x^2 + a4 * x * y + a5 * y^ 2
	float a0 = mat_.at<float>(0, 0);
	float a1 = mat_.at<float>(1, 0);
	float a2 = mat_.at<float>(2, 0);
	float a3 = mat_.at<float>(3, 0);
	float a4 = mat_.at<float>(4, 0);
	float a5 = mat_.at<float>(5, 0);
	return a0 + a1 * x + a2 * y + a3 * x2 + a4 * xy + a5 * y2;
}

cv::Mat SurfaceFitter::PolyFit(const std::vector<cv::Point3i> &points)
{
	//coefficient matrix
	Mat a = Mat::zeros(6, 6, CV_32FC1);
	Mat b = Mat::zeros(6, 1, CV_32FC1);
	for (int i = 0; i < points.size(); i++)
	{
		a.at<float>(0, 0) += 1.0f;
		a.at<float>(0, 1) += points[i].x;
		a.at<float>(0, 2) += points[i].y;
		a.at<float>(0, 3) += std::pow(points[i].x, 2);
		a.at<float>(0, 4) += points[i].x * points[i].y;
		a.at<float>(0, 5) += std::pow(points[i].y, 2);

		a.at<float>(1, 0) += points[i].x;
		a.at<float>(1, 1) += std::pow(points[i].x, 2);
		a.at<float>(1, 2) += points[i].y * points[i].x;
		a.at<float>(1, 3) += std::pow(points[i].x, 3);
		a.at<float>(1, 4) += std::pow(points[i].x, 2) * points[i].y;
		a.at<float>(1, 5) += std::pow(points[i].y, 2) * points[i].x;

		a.at<float>(2, 0) += points[i].y;
		a.at<float>(2, 1) += points[i].x * points[i].y;
		a.at<float>(2, 2) += std::pow(points[i].y, 2);
		a.at<float>(2, 3) += std::pow(points[i].x, 2) * points[i].y;
		a.at<float>(2, 4) += points[i].x * std::pow(points[i].y, 2);
		a.at<float>(2, 5) += std::pow(points[i].y, 3);

		a.at<float>(3, 0) += std::pow(points[i].x, 2);
		a.at<float>(3, 1) += std::pow(points[i].x, 3);
		a.at<float>(3, 2) += points[i].y * std::pow(points[i].x, 2);
		a.at<float>(3, 3) += std::pow(points[i].x, 4);
		a.at<float>(3, 4) += std::pow(points[i].x, 3) * points[i].y;
		a.at<float>(3, 5) += std::pow(points[i].y, 2) * std::pow(points[i].x, 2);

		a.at<float>(4, 0) += points[i].x * points[i].y;
		a.at<float>(4, 1) += std::pow(points[i].x, 2) *points[i].y;
		a.at<float>(4, 2) += std::pow(points[i].y, 2) * points[i].x;
		a.at<float>(4, 3) += std::pow(points[i].x, 3) * points[i].y;
		a.at<float>(4, 4) += std::pow(points[i].x, 2) * std::pow(points[i].y, 2);
		a.at<float>(4, 5) += std::pow(points[i].y, 3) * points[i].x;

		a.at<float>(5, 0) += std::pow(points[i].y, 2);
		a.at<float>(5, 1) += points[i].x * std::pow(points[i].y, 2);
		a.at<float>(5, 2) += std::pow(points[i].y, 3);
		a.at<float>(5, 3) += std::pow(points[i].x, 2) * std::pow(points[i].y, 2);
		a.at<float>(5, 4) += points[i].x * std::pow(points[i].y , 3);
		a.at<float>(5, 5) += std::pow(points[i].y, 4);

		b.at<float>(0, 0) += points[i].z;
		b.at<float>(1, 0) += points[i].z *  points[i].x;
		b.at<float>(2, 0) += points[i].z * points[i].y;
		b.at<float>(3, 0) += points[i].z * std::pow(points[i].x, 2);
		b.at<float>(4, 0) += points[i].z * points[i].x * points[i].y;
		b.at<float>(5, 0) += points[i].z * std::pow(points[i].y, 2);
	}
	Mat inva;
	invert(a, inva);
	return inva * b;
}
