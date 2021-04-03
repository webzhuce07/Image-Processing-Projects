#pragma once

#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <math.h>
#include <time.h>

struct SparseNode
{
	int x;
	int y;
	float value;
};

struct CodNode
{
	int x;
	int y;
};

void ShowMatrix( CvMat* I, int amplification=1, bool saveImg=false);
void ShowMatrixWithSeeds( CvMat* I, CvMat* S);
void ShowResult( CvMat* I, float dist[]);