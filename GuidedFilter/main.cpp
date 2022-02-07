#include <opencv2\opencv.hpp>
#include "guidedFilter.h"
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    //if (argc != 2)
    //{
    //    cout << "Usage: opencv_test <image path>" << endl;
    //    return -1;
    //}

    //char *imgName = argv[1]; 
	char *imgName = "C:\\Users\\VINNO\\Desktop\\src\\cat.jpg";
    Mat inputImg;

    inputImg = imread(imgName, 1);
    if (!inputImg.data)
    {
        cout << "No image data" << endl;
        return -1;
    }
    Mat grayImg , guidedImg;
	inputImg.copyTo(guidedImg);

	GuidedFilter filter;
	grayImg = filter.myGuidedFilter_ColorGuided(inputImg, guidedImg, 80, 0.001);
    imwrite("./result.jpg", grayImg);
	imshow("", grayImg);
	waitKey(0);

    return 0;
}