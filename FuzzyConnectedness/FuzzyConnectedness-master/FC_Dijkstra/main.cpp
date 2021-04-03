/*	Fuzzy connectedness segmentation example 2016/5	*/

//#include <iostream>
#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <time.h>
#include "commom.h"
#include "adjacency.h"
#include "affinity.h"
#include "imoverlay.h"
#include "afc.h"
void main(void)
{
	IplImage* img;
	img=cvLoadImage( /*"197.bmp"*/"cameraman.tif");
	cvNamedWindow("ԭͼ",CV_WINDOW_AUTOSIZE);
	cvShowImage("ԭͼ",img);
	/*IplImage* grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCvtColor(img,grayimg,CV_BGR2GRAY);
	cvNamedWindow("�Ҷ�ͼ",CV_WINDOW_AUTOSIZE);
	cvShowImage("�Ҷ�ͼ",grayimg);*/

	/////int I[img->height][] = new (int*)[img->height][img->width];
	CvMat* I = cvCreateMat( img->height/8, img->width/8, CV_32FC1);
	cvZero(I);//��������0
	/*δ�²���*/
	/*for(int i=0;i<I->height;i++)
		for(int j=0;j<I->width;j++)
		{
			CvScalar temp = cvGet2D(img,i,j);
			CV_MAT_ELEM( *I, float, i, j ) = (float)temp.val[0]/255;
		}*/
	/*�²���һ��*/
	for(int i=0;i<I->height;i++)
		for(int j=0;j<I->width;j++)
		{
			CvScalar temp = cvGet2D(img,i*8,j*8);
			CV_MAT_ELEM( *I, float, i, j ) = (float)temp.val[0]/255;
		}

	ShowMatrix( I, 1, false);

	unsigned int time1,time2;
	/*Compute adjacency*/
	int n = 1;
	float k1 = 0.1;
	vector<SparseNode> A;
	time1 = clock();
	A = adjacency( I, n, k1);
	time2 = clock();
	printf("�ڽӶȾ���������,ʱ�䣺%d����\n",time2-time1);

	/*for(int i=0;i<A.size();i++)
		printf("(%d,%d) %f\n",A[i].x,A[i].y,A[i].value);
	cvWaitKey(0);*/

	/*Compute affinity*/
	float k2 = 2.0;
	vector<SparseNode> K;
	time1 = clock();
	K = affinity( I, A, k2);
	time2 = clock();
	printf("���ƶȾ���������,ʱ�䣺%d����\n",time2-time1);
	
	/*for(int i=0;i<K.size();i++)
		printf("(%d,%d) %f\n",K[i].x,K[i].y,K[i].value);
	cvWaitKey(0);*/

	/*���ӵ����*/
	CvMat* S = cvCreateMat( I->rows, I->cols, CV_8UC1);
	cvZero(S);
	for(int i=3;i<4;i++)
		for(int j=3;j<4;j++)
		{
			CV_MAT_ELEM( *S, char, i, j ) = 1;
		}

	ShowMatrixWithSeeds(I, S);

	/*����ģ�����Ӷ�*/
	CvMat* FC;
	time1 = clock();
	FC=afc( S, K);
	time2 = clock();
	printf("ģ�����ӶȾ���������,ʱ�䣺%d����\n",time2-time1);
	for(int i=0;i<FC->rows;i++)
	{
		for(int j=0;j<FC->cols;j++)
		{
			printf("%9f",CV_MAT_ELEM( *FC, float, i, j ));
		}
		printf("\n");
	}
	ShowMatrix( FC, 2, true);

	/*float **c;
	int cside = (I->rows) * (I->cols);
	c = new float *[cside+1] ;              //�������ռ�
	for(int r=1 ; r<=cside ; r++)
		c[r] = new float [cside+1] ;
	for(int i=1; i<=cside; i++)        //������ͼ��ֵ
		for(int j=1; j<=cside; j++)
		{
			if(i==j)
				c[i][j]=0.0;
			else 
				c[i][j] = 9999.9;  	 
		}
	for(int i=0;i<K.size();i++)
	{
		c[K[i].x+1][K[i].y+1] = 1 - K[i].value;
	}
	for(int i=1;i<=256;i++)
	{
		for(int j=0;j<=256;j++)
		{
			printf("%4f,",c[i][j]);
		}
		printf("\n");
	}
	float dist[257];
	int prev[257];
	Dijkstra(256, 85, dist, prev, c);
	for(int i=1;i<=256;i++)
	{
		printf("%f,",dist[i]);
		if(i%16==0)
			printf("\n");
	}
	ShowResult( I, dist);*/


	cvWaitKey(0);
	cvReleaseImage(&img);
	//cvReleaseImage(&grayimg);
	cvDestroyAllWindows();
}