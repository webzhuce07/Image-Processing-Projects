#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <assert.h>
#include <string>
#include "common.h"
#include "OptimalPath.h"

int SR;
vector<CvPoint> Pixels;
vector<CvMat*> CvMats;
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
	SR = 4;//�²������� sampling rate
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
	ShowMatrix( I, "�²���ͼ", 500, 200, 2, false);
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
        CvPoint pt = cvPoint(x,y);
		Pixels.push_back(pt);
		printf("ѡ���������(%d,%d)\n",x,y);
		//cvCircle( img, pt, 3, cvScalar(255,0,0,0) ,CV_FILLED, CV_AA, 0 );  
    }
	else if( event == CV_EVENT_RBUTTONDOWN )
	{
		system("cls");
		for(int i=0;i<Pixels.size();i++)
		{
			printf("(%d,%d)\n",Pixels[i].x,Pixels[i].y);
			CvMat* FlagMat;
			FlagMat = OptimalPath( (CvMat*)param, Pixels[i].x/SR, Pixels[i].y/SR, 0.0);
			CvMats.push_back(FlagMat);
		}
		CvMat* FinalMat = cvCreateMat( ((CvMat*)param)->rows, ((CvMat*)param)->cols, CV_32FC1);
		cvZero(FinalMat);
		for(int k=0;k<CvMats.size();k++)
		{
			/*char* name = ("nameis%d",i);
			ShowMatrix1( CvMats[i], name, 500, 500, 2, false);*/
			for(int i=0;i<FinalMat->rows;i++)
				for(int j=0;j<FinalMat->cols;j++)
				{
					if( CV_MAT_ELEM( *CvMats[k], float, i, j ) > 0.2 )
					{
						CV_MAT_ELEM( *FinalMat, float, i, j ) += CV_MAT_ELEM( *CvMats[k], float, i, j );
					}
				}
		}
		/*for(int i=0;i<FinalMat->rows;i++)
			for(int j=0;j<FinalMat->cols;j++)
			{
				CV_MAT_ELEM( *FinalMat, float, i, j ) = CV_MAT_ELEM( *FinalMat, float, i, j )/��CvMats.size()��;
			}*/
		ShowMatrix1( FinalMat, "���Ϸָ���", 800, 800, 2, false);
		Pixels.clear();
		CvMats.clear();
	}
	else
	{
	}
}