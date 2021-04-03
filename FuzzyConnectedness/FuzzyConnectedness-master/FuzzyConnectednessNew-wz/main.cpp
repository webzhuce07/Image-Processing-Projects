#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <assert.h>
#include "common.h"
#include "OptimalPath.h"

int SR;
void ChoosePixel( int event, int x, int y, int flags, void* param);
void main( int argc, char** argv)
{
	if( cvLoadImage(argv[1]) == NULL )
	{
		cout<<"����ͼ�����"<<endl;
		system("pause");
		exit(1);
	}
	IplImage* img;
	SR = 1;//�²������� sampling rate
	img = cvLoadImage( argv[1]);			// migong1.jpg | 197.bmp | cameraman.tif
	cvNamedWindow("ԭͼ",CV_WINDOW_AUTOSIZE);
	cvMoveWindow("ԭͼ", 220, 200);
	cvShowImage("ԭͼ",img);
	/*IplImage* grayimg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCvtColor(img,grayimg,CV_BGR2GRAY);
	cvNamedWindow("�Ҷ�ͼ",CV_WINDOW_AUTOSIZE);
	cvShowImage("�Ҷ�ͼ",grayimg);*/

	CvMat* I = cvCreateMat( img->height/SR, img->width/SR, CV_32FC1);
	cvZero(I);//��������0
	for(int i=0;i<I->rows;i++)
		for(int j=0;j<I->cols;j++)
		{
			CvScalar temp = cvGet2D( img, i*SR, j*SR);
			CV_MAT_ELEM( *I, float, i, j ) = (float)temp.val[0]/255;
		}
	ShowMatrix( I, "�²���ͼ", 500, 200, 1, false);
	//WhiteAndBlackTH( I);
	
	cvSetMouseCallback( "ԭͼ", ChoosePixel, (void*)I);
	cvWaitKey(0);
	cvReleaseImage(&img);
	//cvReleaseImage(&grayimg);
	cvDestroyAllWindows();
}

void ChoosePixel( int event, int x, int y, int flags, void* param)
{
	if( event == CV_EVENT_LBUTTONDOWN )  
    {
		system("cls");
        CvPoint pt = cvPoint(x,y);  
        //cvCircle( img, pt, 3, cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );  
		OptimalPath( (CvMat*)param, pt.x/SR, pt.y/SR, 0.1);
    }
}