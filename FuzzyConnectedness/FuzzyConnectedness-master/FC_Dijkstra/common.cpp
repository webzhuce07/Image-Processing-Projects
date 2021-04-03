#include"commom.h"

void ShowMatrix( CvMat* I, int amplification, bool saveImg)
{
	CvMat* Ic;
	Ic = cvCreateMat( (I->rows)*amplification, (I->cols)*amplification, CV_32FC1);
	for(int i=0;i<Ic->rows;i++)
		for(int j=0;j<Ic->cols;j++)
		{
			CV_MAT_ELEM( *Ic, float, i, j ) = CV_MAT_ELEM( *I, float, i/amplification, j/amplification );
		}
	IplImage* newimg = cvCreateImage( cvSize(Ic->rows, Ic->cols), IPL_DEPTH_8U, 1); //sizeΪͼ���ߣ�depthΪλ��ȣ�channelsΪͨ����
	for(int i=0;i<Ic->rows;i++)
		for(int j=0;j<Ic->cols;j++)
		{
			CV_MAT_ELEM( *Ic, float, i, j ) = CV_MAT_ELEM( *Ic, float, i, j )*255;
		}
	
	cvConvert(Ic ,newimg);
	
	if(saveImg==true)
		cvSaveImage("saveImage.jpg",newimg);

	cvNamedWindow( "ShowMatrix", 1 );//��������
	cvShowImage( "ShowMatrix", newimg );//��ʾͼ��
	//cvWaitKey(0); //�ȴ�����
	//cvDestroyWindow( "Image" );//���ٴ���
	//cvReleaseImage( &newimg ); //�ͷ�ͼ��
}

void ShowMatrixWithSeeds(CvMat* I, CvMat* S)
{
	CvMat* Ic;
	Ic = cvCloneMat(I);
	IplImage* newimg = cvCreateImage( cvSize(Ic->rows, Ic->cols), IPL_DEPTH_8U, 1); //sizeΪͼ���ߣ�depthΪλ��ȣ�channelsΪͨ����

	for(int i=0;i<Ic->rows;i++)
		for(int j=0;j<Ic->cols;j++)
		{
			if(CV_MAT_ELEM( *S, char, i, j ) == 0)
			{
				CV_MAT_ELEM( *Ic, float, i, j ) = CV_MAT_ELEM( *Ic, float, i, j )*255;
			}
			else
				CV_MAT_ELEM( *Ic, float, i, j ) = 255;
		}

	cvConvert(Ic ,newimg);

	cvNamedWindow( "Seed regions", 1 );//��������
	cvShowImage( "Seed regions", newimg );//��ʾͼ��
}

void ShowResult( CvMat* I, float dist[])
{
	IplImage* newimg = cvCreateImage( cvSize(I->rows, I->cols), IPL_DEPTH_8U, 3); //sizeΪͼ���ߣ�depthΪλ��ȣ�channelsΪͨ����

	for(int i=0;i<I->rows;i++)
		for(int j=0;j<I->cols;j++)
		{
			if(dist[i*(I->cols)+j] < 2.0)
			{ 
				cvSet2D(newimg,i,j,cvScalar(255,0,0));
			}
			else
			{
				cvSet2D(newimg,i,j,cvScalar(255,255,255));
			}
		}

	cvNamedWindow( "ShowResult", 1 );//��������
	cvShowImage( "ShowResult", newimg );//��ʾͼ��
}