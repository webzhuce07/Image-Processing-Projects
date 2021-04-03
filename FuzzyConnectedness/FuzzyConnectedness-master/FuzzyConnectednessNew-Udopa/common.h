#pragma once

#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <math.h>
#include <time.h>

void ShowMatrix( CvMat* I, char* name, int window_x, int window_y, int amplification=1, bool saveImg=false);
void ShowMatrix1( CvMat* I, char* name, int window_x, int window_y, int amplification, bool saveImg);
void ShowMatrix2( CvMat* I, char* name, int window_x, int window_y);
void WhiteAndBlackTH( CvMat* I);
void Comparison( CvMat* flag);