#include "fuzzyconnectedness.h"

FuzzyConnectedness::FuzzyConnectedness(cv::Mat& input)
	:FuzzyConnectednessBase(input)
{
	m_Mean = 0.0;
	m_Diff_Mean = 0.0;
	m_Variance = 0.0;
	m_Diff_Variance = 0.0;
}



void FuzzyConnectedness::SetParameters(const double inmean, const double invar, const double indifmean, const double indifvar, const double inweight)
{
	m_Mean = inmean;
	m_Variance = invar;
	m_Diff_Mean = indifmean;
	m_Diff_Variance = indifvar;

	if (inweight < 0)
	{
		this->SetWeight(0);
	}
	else if (inweight > 1)
	{
		this->SetWeight(1);
	}
	else
	{
		this->SetWeight(inweight);
	}
}

FuzzyConnectedness::~FuzzyConnectedness()
{

}

double FuzzyConnectedness::FuzzyAffinity(const cv::Point& point1, const cv::Point& point2)
{
	double f1 = m_InputImage.at<unsigned short>(point1);
	double f2 = m_InputImage.at<unsigned short>(point2);
	double tmp1 = 0.5 * (f1 + f2) - m_Mean;
	if (this->GetWeight() == 1)
	{
		return exp(-0.5 * tmp1 * tmp1 / m_Variance);
	}
	else
	{
		double tmp2 = fabs(f1 - f2) - m_Diff_Mean;
		return((this->GetWeight() * exp(-0.5 * tmp1 * tmp1 / m_Variance) +
			(1 - this->GetWeight()) * exp(-0.5 * tmp2 * tmp2 / m_Diff_Variance)));
	}
}

