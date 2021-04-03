#include <highgui.h>
#include <cv.h>  
#include <cvaux.h>
#include <cxcore.h>
#include <vector>
#include <map>
#include"commom.h"
using namespace std;

CvMat* afc( CvMat* S, vector<SparseNode> K);
vector<CodNode> remove( vector<CodNode> Q, vector<CodNode> idx);
vector<int> remove1( vector<int> Q, vector<int> idx);
void Dijkstra( int n, int v, float dist[],int prev[], float * * c);