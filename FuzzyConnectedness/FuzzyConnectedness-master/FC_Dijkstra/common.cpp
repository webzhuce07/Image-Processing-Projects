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
	IplImage* newimg = cvCreateImage( cvSize(Ic->rows, Ic->cols), IPL_DEPTH_8U, 1); //size为图像宽高，depth为位深度，channels为通道数
	for(int i=0;i<Ic->rows;i++)
		for(int j=0;j<Ic->cols;j++)
		{
			CV_MAT_ELEM( *Ic, float, i, j ) = CV_MAT_ELEM( *Ic, float, i, j )*255;
		}
	
	cvConvert(Ic ,newimg);
	
	if(saveImg==true)
		cvSaveImage("saveImage.jpg",newimg);

	cvNamedWindow( "ShowMatrix", 1 );//创建窗口
	cvShowImage( "ShowMatrix", newimg );//显示图像
	//cvWaitKey(0); //等待按键
	//cvDestroyWindow( "Image" );//销毁窗口
	//cvReleaseImage( &newimg ); //释放图像
}

void ShowMatrixWithSeeds(CvMat* I, CvMat* S)
{
	CvMat* Ic;
	Ic = cvCloneMat(I);
	IplImage* newimg = cvCreateImage( cvSize(Ic->rows, Ic->cols), IPL_DEPTH_8U, 1); //size为图像宽高，depth为位深度，channels为通道数

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

	cvNamedWindow( "Seed regions", 1 );//创建窗口
	cvShowImage( "Seed regions", newimg );//显示图像
}

void ShowResult( CvMat* I, float dist[])
{
	IplImage* newimg = cvCreateImage( cvSize(I->rows, I->cols), IPL_DEPTH_8U, 3); //size为图像宽高，depth为位深度，channels为通道数

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

	cvNamedWindow( "ShowResult", 1 );//创建窗口
	cvShowImage( "ShowResult", newimg );//显示图像
}