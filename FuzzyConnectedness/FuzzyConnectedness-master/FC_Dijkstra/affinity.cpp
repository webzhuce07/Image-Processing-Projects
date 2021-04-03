#include"affinity.h"

vector<SparseNode> affinity( CvMat* I, vector<SparseNode> A, float k2)
{
	float a;
	int i=0,j=0,i_i=0,i_j=0,j_i=0,j_j=0;
	SparseNode tempnode;
	vector<SparseNode> K;
	for(int n=0;n<A.size();n++)
	{
		i = A[n].x;
		i_i = (i%(I->rows));
		i_j = i/(I->rows);
		//printf("%d,%d\n",i_i,i_j);
		j = A[n].y;
		j_i = (j%(I->rows));
		j_j = j/(I->rows);
		/*printf("%d,%d\n",A[n].y,A[n].x);
		cvWaitKey(0);*/
		a = A[n].value/(1+k2*abs(CV_MAT_ELEM( *I, float, i_i, i_j ) - CV_MAT_ELEM( *I, float, j_i, j_j )));
		tempnode.x = i;
		tempnode.y = j;
		tempnode.value = a;
		K.push_back(tempnode);
	}

	return K;
}