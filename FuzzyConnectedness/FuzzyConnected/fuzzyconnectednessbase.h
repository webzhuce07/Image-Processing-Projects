#pragma once
#include <opencv2/opencv.hpp>
#include <queue>
/*
* Fuzzy connectedness image filter works on multi - dimensional image.
* Fuzzy affinity is defined between two nearby pixels in a image, it has
* higher value when the two pixel are closer, and the pixel value is similar
* also both of them are similar to the defined object pixels.
* Strength of a "path" between two pixels was defined as the weakest affinity
* between pairs of connected pixels along the "path",
*Fuzzy Connectedness of two pixels was defined as the strongest path strength
* among all possible paths between the two pixels.
* A fuzzy object was defined as the collection of pixels that within this collection
* each pair of pixel have a strong fuzzy connectedness(say, above some threshold)
* any pixel outside the object will have a weak fuzzy connectedness to any pixels
* inside the object.
* The simple fuzzy connectedness image filter implements the compute of a fuzzy
* object by given prior information of the object.
* this is the base class, all sub - classes should implement the definition of
* fuzzy affinity between two nearby pixels.In this segmentation, all fuzzy affinity
* was only defined between the 4 - connected neighbor pixels.
*
* Detail information about this algorithm can be found in :
*"Fuzzy Connectedness and Object Definition: Theory, Algorithms,
*    and Applications in Image Segmentation", J. Udupa and S. Samarasekera
*  Graphical Models and Image Processing, Vol.58, No.3.pp 246 - 261, 1996.
*/

class FuzzyConnectednessBase
{
public:
	FuzzyConnectednessBase(const cv::Mat& input);
	virtual ~FuzzyConnectednessBase();

	/** Set/Get the weight of the first term (standard statistics) in the
	 * affinity computation. */
	void SetWeight(double weight) { m_Weight = weight; }
	double GetWeight() const { return m_Weight; }

	/** Set/Get the threshold value for the segmentation. This threshold value is
	 * scaled with respect to the range of the unsigned short type.  The
	 * resulting value is used to threshold the affinity map and generate a binary
	 * image as output. */
	void SetThreshold(double threshold) { m_Threshold = threshold; }
	double GetThreshold() const { return m_Threshold; }

	/** After setting parameters, make the fuzzy scene. */
	void MakeFuzzyScene();

	/** Setting the starting point, believed to be inside the object. */
	void SetObjectSeed(cv::Point seed) { m_ObjectSeed = seed; }
	cv::Point GetObjectSeed() const { return m_ObjectSeed; }

	/** Update the binary result (needed after an update the threshold). */
	void MakeSegmentObject();

	/** A simple combination of SetThreshold and MakeSegmentObject methods. */
	void UpdateThreshold(const double x);

	/** Extract the FuzzyScene not thresholded. */
	const cv::Mat&  GetFuzzyScene() const
	{
		return m_FuzzyScene;
	};

	/** Get the segment object */
	const cv::Mat&  GetSegmentObject() const
	{
		return m_SegmentObject;
	};

protected:
	double            m_Weight;
	double            m_Threshold;
	cv::Point         m_ObjectSeed;
	cv::Size          m_Size;

	cv::Mat    m_InputImage;
	cv::Mat    m_FuzzyScene;
	cv::Mat    m_SegmentObject;

	std::queue<cv::Point> m_Queue;

	void PushNeighbors(const cv::Point &center);

	/** Define the fuzzy affinity function between two pixels. */
	virtual double FuzzyAffinity(const cv::Point &point1, const cv::Point &point2)
	{
		return 0;
	}

	double FindStrongPath(const cv::Point &center);
};