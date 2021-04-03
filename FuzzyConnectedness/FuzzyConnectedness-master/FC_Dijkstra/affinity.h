#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <cxcore.h>
#include <vector>
#include"commom.h"
using namespace std;

vector<SparseNode> affinity(CvMat* I, vector<SparseNode> A, float k2);