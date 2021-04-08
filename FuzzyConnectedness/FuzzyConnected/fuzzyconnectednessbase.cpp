


#include "fuzzyconnectednessbase.h"

FuzzyConnectednessBase::FuzzyConnectednessBase(const cv::Mat& input)
{
	m_Threshold = 1.0;
	m_ObjectSeed = cv::Point(-1, -1);
	m_Weight = 1.0;
	input.copyTo(m_InputImage);
}

FuzzyConnectednessBase::~FuzzyConnectednessBase()
{

}

void FuzzyConnectednessBase::MakeSegmentObject()
{
	m_InputImage.copyTo(m_SegmentObject);
	for (int r = 0; r < m_SegmentObject.rows; r++)
	{
		for (int c = 0; c < m_SegmentObject.cols; c++)
		{
			if (m_FuzzyScene.at<double>(r, c) > m_Threshold)
				m_SegmentObject.at<unsigned short>(r, c) = 255;
			else
				m_SegmentObject.at<unsigned short>(r, c) = 0;
			{
			}
		}
	}
}

void FuzzyConnectednessBase::UpdateThreshold(const double x)
{
	this->SetThreshold(x);
	this->MakeSegmentObject();
}

void FuzzyConnectednessBase::PushNeighbors(const cv::Point &center)
{
	cv::Point currentPoint = center;
	//in column
	if (currentPoint.x < m_InputImage.cols - 1)
	{
		currentPoint.x++;
		m_Queue.push(currentPoint);
		currentPoint.x--;
	}
	if (currentPoint.x > 0)
	{
		currentPoint.x--;
		m_Queue.push(currentPoint);
		currentPoint.x++;
	}

	//in row
	if (currentPoint.y < m_InputImage.rows - 1)
	{
		currentPoint.y++;
		m_Queue.push(currentPoint);
		currentPoint.y--;
	}
	if (currentPoint.y > 0)
	{
		currentPoint.y--;
		m_Queue.push(currentPoint);
		currentPoint.y++;
	}
}

double FuzzyConnectednessBase::FindStrongPath(const cv::Point &center)
{
	cv::Point currentPoint = center;
	double tmp = 0.0, tmp1 = 0.0, tmp2 = 0.0;

	//in column
	if (currentPoint.x > 0)
	{
		currentPoint.x--;
		tmp = m_FuzzyScene.at<double>(currentPoint);
		tmp1 = this->FuzzyAffinity(currentPoint, center);
		if (tmp > tmp1)
			tmp = tmp1;
		currentPoint.x++;
	}
	if (currentPoint.x < m_InputImage.cols - 1)
	{
		currentPoint.x++;
		tmp2 = m_FuzzyScene.at<double>(currentPoint);
		tmp1 = this->FuzzyAffinity(currentPoint, center);
		if (tmp2 > tmp1)
			tmp2 = tmp1;
		if (tmp < tmp2)
			tmp = tmp2;
		currentPoint.x--;
	}

	//in row
	if (currentPoint.y > 0)
	{
		currentPoint.y--;
		tmp2 = m_FuzzyScene.at<double>(currentPoint);
		tmp1 = this->FuzzyAffinity(currentPoint, center);
		if (tmp2 > tmp1)
			tmp2 = tmp1;
		if (tmp < tmp2)
			tmp = tmp2;
		currentPoint.y++;
	}
	if (currentPoint.y < m_InputImage.rows - 1)
	{
		currentPoint.y++;
		tmp2 = m_FuzzyScene.at<double>(currentPoint);
		tmp1 = this->FuzzyAffinity(currentPoint, center);
		if (tmp2 > tmp1)
			tmp2 = tmp1;
		if (tmp < tmp2)
			tmp = tmp2;
		currentPoint.y--;
	}
	return tmp;
}

void FuzzyConnectednessBase::MakeFuzzyScene()
{
	m_FuzzyScene = cv::Mat::zeros(m_InputImage.size(), CV_64FC1);
	m_FuzzyScene.at<double>(m_ObjectSeed) = 1.0;
	this->PushNeighbors(m_ObjectSeed);
	while (!m_Queue.empty())
	{
		auto currentPoint = m_Queue.front();
		m_Queue.pop();
		auto fmax = this->FindStrongPath(currentPoint);
		if (fmax > m_FuzzyScene.at<double>(currentPoint))
		{
			m_FuzzyScene.at<double>(currentPoint) = fmax;
			this->PushNeighbors(currentPoint);
		}
	}
	this->MakeSegmentObject();
}
