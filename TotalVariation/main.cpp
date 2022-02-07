#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include <cv.h>
#include <math.h>

using namespace cv;
using namespace std;

int main(void)
{
	//读取原始图像及掩模图像
	IplImage *src_uint8 = cvLoadImage("0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *mask = cvLoadImage("0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//合成需要修复的图像
	int M = mask->height;
	int N = mask->width;
	int i, j;
	CvMat *src = cvCreateMat(M, N, CV_32FC1);//存放浮点图像
	cvConvert(src_uint8, src);
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			if ((mask->imageData + i * mask->widthStep)[j] < 0)//理解此处判别条件，根据情况自行更改
			{
				((float*)(void*)(src->data.ptr + (size_t)src->step*i))[j] = 0.0;
			}
			if (((float*)(void*)(src->data.ptr + (size_t)src->step*i))[j] < 0)
			{
				((float*)(void*)(src->data.ptr + (size_t)src->step*i))[j] += 256.0;
			}
		}
	}
	cvConvert(src, src_uint8);
	src_uint8 = cvLoadImage("0.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cvConvert(src_uint8, src);
	cvShowImage("需要修复的图像", src_uint8);
	cvWaitKey(0);



	double t = getTickCount();//当前滴答数
	float lambda = 0.2;
	float delta = 0.5;
	float UO, UN, UW, US, UE, UNE, UNW, USW, USE;
	float Un, Ue, Uw, Us;
	float Wn, We, Ww, Ws;
	float Hon, Hoe, How, Hos;
	float Hoo;
	int iteration = 500;
	while (iteration)
	{
		for (i = 1; i < M - 1; i++)
		{
			for (j = 1; j < N - 1; j++)
			{
				if (((char *)(mask->imageData + i * mask->widthStep))[j] < 0)//坏损区
				{
					UO = ((float*)(void*)(src->data.ptr + (size_t)src->step*i))[j];
					UN = ((float*)(void*)(src->data.ptr + (size_t)src->step*(i - 1)))[j];
					US = ((float*)(void*)(src->data.ptr + (size_t)src->step*(i + 1)))[j];
					UE = ((float*)(void*)(src->data.ptr + (size_t)src->step*i))[j + 1];
					UW = ((float*)(void*)(src->data.ptr + (size_t)src->step*i))[j - 1];

					UNE = ((float*)(void*)(src->data.ptr + (size_t)src->step*(i - 1)))[j + 1];
					UNW = ((float*)(void*)(src->data.ptr + (size_t)src->step*(i - 1)))[j - 1];
					USE = ((float*)(void*)(src->data.ptr + (size_t)src->step*(i + 1)))[j + 1];
					USW = ((float*)(void*)(src->data.ptr + (size_t)src->step*(i + 1)))[j - 1];

					Un = sqrt((UO - UN) * (UO - UN) + ((UNW - UNE) / 2.0) * ((UNW - UNE) / 2.0));
					Ue = sqrt((UO - UE) * (UO - UE) + ((UNE - USE) / 2.0) * ((UNE - USE) / 2.0));
					Uw = sqrt((UO - UW) * (UO - UW) + ((UNW - USW) / 2.0) * ((UNW - USW) / 2.0));
					Us = sqrt((UO - US) * (UO - US) + ((USW - USE) / 2.0) * ((USW - USE) / 2.0));

					Wn = 1.0 / sqrt(Un * Un + delta * delta);
					We = 1.0 / sqrt(Ue * Ue + delta * delta);
					Ww = 1.0 / sqrt(Uw * Uw + delta * delta);
					Ws = 1.0 / sqrt(Us * Us + delta * delta);

					Hon = Wn / (Wn + We + Ww + Ws + lambda);
					Hoe = We / (Wn + We + Ww + Ws + lambda);
					How = Ww / (Wn + We + Ww + Ws + lambda);
					Hos = Ws / (Wn + We + Ww + Ws + lambda);

					Hoo = lambda / (Wn + We + Ww + Ws + lambda);
					((float*)(void*)(src->data.ptr + (size_t)src->step*i))[j] = (Hon*UN + Hoe * UE + How * UW + Hos * US + Hoo * UO);
				}
			}
		}
		iteration--;
	}
	cvConvert(src, src_uint8);
	t = ((double)getTickCount() - t) / getTickFrequency();
	printf("算法用时：%f秒\n", t);
	cvShowImage("修复结果", src_uint8);
	cvWaitKey(0);
}