#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <math.h>
using namespace cv;
using namespace std;


//����Ӧ�Աȶ���ǿ�㷨��C��ʾ�Ը�Ƶ��ֱ������ϵ��,n��ʾ�˲��뾶��maxCG��ʾ��CG�����ֵ����
Mat ACE(Mat src, int C = 3, int n = 3, float MaxCG = 7.5) {
	int row = src.rows;
	int col = src.cols;
	Mat meanLocal; //ͼ��ֲ���ֵ
	Mat varLocal; //ͼ��ֲ�����
	Mat meanGlobal; //ȫ�־�ֵ
	Mat varGlobal; //ȫ�ֱ�׼��
	blur(src.clone(), meanLocal, Size(n, n));
	Mat highFreq = src - meanLocal;
	varLocal = highFreq.mul(highFreq);
	varLocal.convertTo(varLocal, CV_32F);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			varLocal.at<float>(i, j) = (float)sqrt(varLocal.at<float>(i, j));
		}
	}
	meanStdDev(src, meanGlobal, varGlobal);
	Mat gainArr = meanGlobal / varLocal; //����ϵ������
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (gainArr.at<float>(i, j) > MaxCG) {
				gainArr.at<float>(i, j) = MaxCG;
			}
		}
	}
	printf("%d %d\n", row, col);
	gainArr.convertTo(gainArr, CV_8U);
	gainArr = gainArr.mul(highFreq);
	Mat dst1 = meanLocal + gainArr;
	//Mat dst2 = meanLocal + C * highFreq;
	return dst1;
}

int main() {
	Mat src = imread("spectrum.jpg");
	vector <Mat> now;
	split(src, now);
	int C = 150;
	int n = 5;
	float MaxCG = 3;
	Mat dst1 = ACE(now[0], C, n, MaxCG);
	Mat dst2 = ACE(now[1], C, n, MaxCG);
	Mat dst3 = ACE(now[2], C, n, MaxCG);
	now.clear();
	Mat dst;
	now.push_back(dst1);
	now.push_back(dst2);
	now.push_back(dst3);
	cv::merge(now, dst);
	imshow("origin", src);
	imshow("result", dst);
	imwrite("result.jpg", dst);
	waitKey(0);
	return 0;
}