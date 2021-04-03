#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <cxcore.h>
#include <vector>
#include "commom.h"
using namespace std;

vector<SparseNode> adjacency(CvMat* I, int n, float k1);