#include"common.h"

using namespace std;

void ShowMatrix( CvMat* I, char* name, int window_x, int window_y, int amplification, bool saveImg)
{
	CvMat* Ic;
	Ic = cvCreateMat( (I->rows)*amplification, (I->cols)*amplification, CV_32FC1);
	for(int i=0;i<Ic->rows;i++)
		for(int j=0;j<Ic->cols;j++)
		{
			CV_MAT_ELEM( *Ic, float, i, j ) = CV_MAT_ELEM( *I, float, i/amplification, j/amplification );
		}
	IplImage* newimg = cvCreateImage( cvSize(Ic->rows, Ic->cols), IPL_DEPTH_8U, 1); //sizeΪͼ���ߣ�depthΪλ��ȣ�channelsΪͨ����
	
	double tmpCountMinVal = 0, tmpCountMaxVal = 0; 
	cvMinMaxLoc( Ic, &tmpCountMinVal, &tmpCountMaxVal, NULL, NULL);
	//cout<<tmpCountMaxVal<<" "<<tmpCountMinVal<<endl;
	/////////////////////////////////////////////////////////////////////
	for(int i=0;i<Ic->rows;i++)
		for(int j=0;j<Ic->cols;j++)
		{
			CV_MAT_ELEM( *Ic, float, i, j ) = ((CV_MAT_ELEM( *Ic, float, i, j )-tmpCountMinVal)/(tmpCountMaxVal-tmpCountMinVal))*255;
		}
	
	cvConvert(Ic ,newimg);
	
	if(saveImg==true)
		cvSaveImage("saveImage.jpg",newimg);

	cvNamedWindow( name, 1 );//��������
	cvMoveWindow( name, window_x, window_y);
	cvShowImage( name, newimg );//��ʾͼ��
	//cvWaitKey(0); //�ȴ�����
	//cvDestroyWindow( "Image" );//���ٴ���
	//cvReleaseImage( &newimg ); //�ͷ�ͼ��
}

void ShowMatrix1( CvMat* I, char* name, int window_x, int window_y, int amplification, bool saveImg)
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

	cvNamedWindow( name, 1 );//��������
	cvMoveWindow( name, window_x, window_y);
	cvShowImage( name, newimg );//��ʾͼ��
}

void WhiteAndBlackTH( CvMat* I)
{
	//void cvMorphologyEx(const CvArr* src, CvArr* dst, CvArr* tmp, IplConvKernel* element, int operation, int iterations = 1);
	//-CV_MOP_OPEN�����㲻��Ҫ��ʱͼ��
	//-CV_MOP_CLOSE�����㲻��Ҫ��ʱͼ��
	//-CV_MOP_GRADIENT��̬�ݶ���Ҫ��ʱͼ��
	//-CV_MOP_TOPHAT����ñ��src = dst�������Ҫ
	//-CV_MOP_BLACKHAT����ñ��src = dst�������Ҫ

	CvMat* temp = cvCreateMat( I->rows, I->cols, CV_32FC1);
	cvZero(temp);
	
	CvMat* WTH = cvCreateMat( I->rows, I->cols, CV_32FC1);
	cvMorphologyEx( I, WTH, temp, NULL, CV_MOP_TOPHAT, 1);
	ShowMatrix1( WTH, "��ñ������", 780, 500, 1, false);
	
	CvMat* BTH = cvCreateMat( I->rows, I->cols, CV_32FC1);
	cvMorphologyEx( I, BTH, temp, NULL, CV_MOP_BLACKHAT, 1);
	ShowMatrix1( BTH, "��ñ������", 880, 500, 1, false);

	for(int i=0;i<I->rows;i++)
		for(int j=0;j<I->cols;j++)
		{
			CV_MAT_ELEM( *WTH, float, i, j ) = CV_MAT_ELEM( *WTH, float, i, j ) * 0.5;
		}
	for(int i=0;i<I->rows;i++)
		for(int j=0;j<I->cols;j++)
		{
			CV_MAT_ELEM( *BTH, float, i, j ) = CV_MAT_ELEM( *BTH, float, i, j ) * 0.5;
		}

	cvSub( WTH, BTH, temp);
	cvAdd( I, temp, I);

	/*CvMat* WTH = cvCreateMat( I->rows, I->cols, CV_32FC1);
	cvMorphologyEx( I, WTH, NULL, NULL, CV_MOP_OPEN, 1);
	ShowMatrix( WTH, "��������", 780, 500, 2, false);
	CvMat* BTH = cvCreateMat( I->rows, I->cols, CV_32FC1);
	cvMorphologyEx( I, BTH, NULL, NULL, CV_MOP_CLOSE, 1);
	ShowMatrix( BTH, "��������", 880, 500, 2, false);
	for(int i=0;i<I->rows;i++)
		for(int j=0;j<I->cols;j++)
		{
			CV_MAT_ELEM( *I, float, i, j ) = CV_MAT_ELEM( *I, float, i, j ) * 3.0;
		}
	cvSub( I, WTH, I);
	cvSub( I, BTH, I);
	
	*/
	ShowMatrix1( I, "Ԥ������", 1160, 500, 1, false);

	cvWaitKey(0);
}