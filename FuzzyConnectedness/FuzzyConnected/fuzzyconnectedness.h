#pragma once
#include "fuzzyconnectednessbase.h"


class FuzzyConnectedness : public FuzzyConnectednessBase
{
public:
	FuzzyConnectedness(cv::Mat& input);
	virtual~FuzzyConnectedness();

	/** Setting the parameters for segmentation. */
	void SetParameters(const double inmean, const double invar,
		const double indifmean, const double indifvar, const double inweight);

protected:
	double m_Mean;
	double m_Variance; //estimation of the Variance.
	double m_Diff_Mean;
	double m_Diff_Variance;

	virtual double FuzzyAffinity(const cv::Point& point1, const cv::Point& point2);
};