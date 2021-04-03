#include"adjacency.h"

/*Input: image (used for size only), n=L1 distance of neighbourhood,
k1=distance decline factor*/
vector<SparseNode> adjacency(CvMat* I, int n, float k1)
{
	int r = I->rows;
	int c = I->cols;
	int N = r*c;
	/*printf("%d,%d",r,c);
	cvWaitKey(0);*/

	/*abs delta of column and row coordinates*/
	CvMat* tempdr1 = cvCreateMat( 1, r, sizeof(int));
	CvMat* tempdR1 = cvCreateMat( r, r, sizeof(int));
	for(int i=0;i<r;i++)
		CV_MAT_ELEM( *tempdr1, int, 0, i) = i+1;	
	cvRepeat( tempdr1, tempdR1);

	CvMat* tempdr2 = cvCreateMat( r, 1, sizeof(int));
	CvMat* tempdR2 = cvCreateMat( r, r, sizeof(int));
	for(int i=0;i<r;i++)
		CV_MAT_ELEM( *tempdr2, int, i, 0) = i+1;	
	cvRepeat( tempdr2, tempdR2);

	//for(int i=0;i<r;i++)	//ÏÔÊ¾¾ØÕótempdR2
	//{
	//	for(int j=0;j<r;j++)
	//	{
	//		printf("%d ",CV_MAT_ELEM( *tempdR2, int, i, j));
	//	}
	//	printf("\n");
	//}
	//cvWaitKey(0);

	CvMat* dR = cvCreateMat( r, r, sizeof(int));
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<r;j++)
		{
			CV_MAT_ELEM( *dR, int, i, j ) = abs(CV_MAT_ELEM( *tempdR1, int, i, j ) - CV_MAT_ELEM( *tempdR2, int, i, j ));
			//printf("%d,",CV_MAT_ELEM( *dR, int, i, j ));
		}
		//printf("\n");
	}
	//cvWaitKey(0);
	
	CvMat* tempdc1 = cvCreateMat( 1, c, sizeof(int));
	CvMat* tempdC1 = cvCreateMat( c, c, sizeof(int));
	for(int i=0;i<c;i++)
		CV_MAT_ELEM( *tempdc1, int, 0, i) = i+1;	
	cvRepeat( tempdc1, tempdC1);

	CvMat* tempdc2 = cvCreateMat( c, 1, sizeof(int));
	CvMat* tempdC2 = cvCreateMat( c, c, sizeof(int));
	for(int i=0;i<c;i++)
		CV_MAT_ELEM( *tempdc2, int, i, 0) = i+1;	
	cvRepeat( tempdc2, tempdC2);

	CvMat* dC = cvCreateMat( c, c, sizeof(int));
	for(int i=0;i<c;i++)
	{
		for(int j=0;j<c;j++)
		{
			CV_MAT_ELEM( *dC, int, i, j ) = abs(CV_MAT_ELEM( *tempdC1, int, i, j ) - CV_MAT_ELEM( *tempdC2, int, i, j ));
			//printf("%d,",CV_MAT_ELEM( *dC, int, i, j ));
		}
		//printf("\n");
	}
	//cvWaitKey(0);
	
	/*output matrix*/
	/*CvSparseMat* tempmatrix;//Old Version
	int size[2] = {N,N};
	tempmatrix = cvCreateSparseMat( 2, size, CV_32FC1);*/
	
	vector<SparseNode> A;

	/*float **tempmatrix = new float *[N];
	for(int i=0; i<N; i++)
		tempmatrix[i] = new float [N];
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
		{
			tempmatrix[i][j] = 0.0;
		}*/

	/*for each L1 distance level*/
	vector<int> bi;
	vector<int> bj;
	vector<int> i1;
	vector<int> j1;

	for(int dc=0;dc<=n;dc++)
	{
		//[i,j]=find(dC == dc); /*r*r blocks with dc<=n*/
		bi.clear();
		bj.clear();
		for(int i=0;i<c;i++)
		{
			for(int j=0;j<r;j++)
			{
				if(CV_MAT_ELEM( *dC, int, j, i ) == dc)
				{
					bi.push_back(i);
					bj.push_back(j);
				};
			}
		}
		/*for(int i=0;i<bi.size();i++)
			printf("[%d,%d]\n",bi[i],bj[i]);
		cvWaitKey(0);*/

		for(char dr=0;dr<=n-dc;dr++)
		{
			//[i,j]=find(dR == dr); /*pixels within block with dr+dc<=n*/
			i1.clear();
			j1.clear();
			for(int i=0;i<c;i++)
			{
				for(int j=0;j<r;j++)
				{
					if(CV_MAT_ELEM( *dR, int, j, i ) == dr)
					{
						i1.push_back(i);
						j1.push_back(j);
					}
				}
			}

			/*The adjaceny*/
			float a;
			a = 1.0/(1+k1*sqrt(pow(dr,2.0)+pow(dc,2.0)));
			/*printf("%f\n",a);
			cvWaitKey(0);*/

			/*compute global indices and put things into A*/
			vector<int> p;
			vector<int> q;
			
			for(int k=0;k<bi.size();k++)
			{
				/*p=[p;(bi(k)-1)*r+i];
				q=[q;(bj(k)-1)*r+j];*/
				for(int i=0;i<i1.size();i++)
				{
					p.push_back(bi[k]*r+i1[i]);
				}
				for(int j=0;j<j1.size();j++)
				{
					q.push_back(bj[k]*r+j1[j]);
				}
			}
			/*printf("[%d,%d]\n",p.size(),q.size());
			cvWaitKey(0);*/

			/*	A=A+sparse(p,q,repmat(a,size(q)),N,N);	*/
			/*for(int i=0;i<p.size();i++)
			{
				tempmatrix[p[i]][q[i]] += a;
			}*/

			for(int i=0;i<p.size();i++)
			{
				bool flag = false;
				for(int j=0;j<A.size();j++)
				{
					if(A[j].x==q[i] && A[j].y==p[i])
					{
						A[j].value += a;
						flag = true;
						break;
					}
				}
				if(flag==false)
				{
					SparseNode tempnode;
					tempnode.x = q[i];
					tempnode.y = p[i];
					tempnode.value = a;
					A.push_back(tempnode);
				}
			}
		}
	}

	/*for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(tempmatrix[j][i] != 0.0)
			{
				SparseNode tempnode;
				tempnode.x = j;
				tempnode.y = i;
				tempnode.value = tempmatrix[j][i];
				A.push_back(tempnode);
			}
		}
	}*/

	return A;
}