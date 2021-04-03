#include "afc.h"

CvMat* afc( CvMat* S, vector<SparseNode> K)
{
	CvMat* FC;
	FC = cvCreateMat( S->rows, S->cols, CV_32FC1);
	cvZero(FC);
	for(int i=0;i<S->rows;i++)
		for(int j=0;j<S->cols;j++)
		{
			if(CV_MAT_ELEM( *S, char, i, j) > 0)
			{
				CV_MAT_ELEM( *FC, float, i, j) = (float)CV_MAT_ELEM( *S, char, i, j);
			}
		}
	
	/*for(int i=0;i<S->rows;i++)	//检测double(S>0)转换结果
	{
		for(int j=0;j<S->cols;j++)
		{
			printf("%f,",CV_MAT_ELEM( *FC, float, i, j));
		}
		printf("\n");
	}
	cvWaitKey(0);*/
	///*找到所有标志矩阵上非零的坐标*/
	//vector<CodNode> Q;
	//for(int i=0;i<S->rows;i++)
	//	for(int j=0;j<S->cols;j++)
	//	{
	//		if(CV_MAT_ELEM( *S, char, i, j) != 0)
	//		{
	//			CodNode tempnode;
	//			tempnode.x = i;
	//			tempnode.y = j;
	//			Q.push_back(tempnode);
	//		}
	//	}

	///*Dijkstra*/
	//while(!Q.empty())
	//{
	//	/*找到所有Q中最大值fc*/
	//	float fc=0.0;
	//	for(int i=0;i<Q.size();i++)
	//	{
	//		if( fc < CV_MAT_ELEM( *FC, float, Q[i].x, Q[i].y) )
	//			fc = CV_MAT_ELEM( *FC, float, Q[i].x, Q[i].y);
	//	}
	//	/*找到所有相同大小的fc值，坐标为idx*/
	//	vector<CodNode> idx;
	//	for(int i=0;i<Q.size();i++)
	//	{
	//		if( CV_MAT_ELEM( *FC, float, Q[i].x, Q[i].y) == fc )
	//		{
	//			CodNode tempnode;
	//			tempnode.x = Q[i].x;
	//			tempnode.y = Q[i].y;
	//			idx.push_back(tempnode);
	//		}
	//	}
	//	/*找到所有Q节点向量上最大值*/
	//	vector<int> pick;
	//	for(int i=0;i<idx.size();i++)

	//	Q = remove( Q, idx);

	//	/*单层相似度传播*/
	//	float f = 0.0;
	//	/*f=min(fc,max(K(:,pick),[],2)); %compute fc for adjacent pixels
	//	idx=find(f>FC(:)); %find those with real change
	//	FC(idx)=f(idx); %update FC
	//	Q=[Q;idx]; */
	//}
	vector<int> Q;
	int iQ = 1;
	for(int i=0;i<S->cols;i++)
		for(int j=0;j<S->rows;j++)
		{
			if(CV_MAT_ELEM( *S, char, i, j) != 0)
			{
				Q.push_back(iQ);
			}
			iQ++;
		}
	/*for(int i=0;i<Q.size();i++)	//检测Q=find(S)转换结果
		printf("%d",Q[i]);
	cvWaitKey(0);*/
	/********************************************/
	/*		稀疏矩阵map化表示计算第1部分		*/
	int sidelength = (FC->rows)*(FC->cols);
	map<int,map<int,float>> xy_map;
	//map<int,float> temp_map;
	for(int i=0;i<K.size();i++)
	{
		//temp_map[K[i].x] = K[i].value;
		//xy_map.insert(make_pair(K[i].y,temp_map));
		xy_map[K[i].y][K[i].x] = K[i].value;
	}
	/********************************************/
	unsigned int time1,time2,time=0;
	while(!Q.empty())
	{
		/* fc=max(FC(Q));寻找Q中最大的fc值 */
		float fc = 0.0;
		for(int k=0; k<Q.size(); k++)
		{
			int i,j;
			i = (Q[k]-1)%(S->rows);
			j = (Q[k]-1)/(S->rows);
			if( fc < CV_MAT_ELEM( *FC, float, i, j) )
				fc = CV_MAT_ELEM( *FC, float, i, j);
		}
		
		//cvWaitKey(0);

		/* idx=find(FC(Q)==fc); */
		vector<int> idx;
		idx.clear();
		int iidx = 1;
		for(int i=0;i<Q.size();i++)
		{
			if( CV_MAT_ELEM( *FC, float, (Q[i]-1)%(FC->rows), (Q[i]-1)/(FC->rows)) == fc )
			{
				idx.push_back(iidx);
			}
			iidx++;
		}
		/*for(int i=0;i<idx.size();i++)
		{
			printf("%d,",idx[i]);
		}
		printf("\n%f\n",fc);*/
		
		/* pick=Q(idx); */
		vector<int> pick;
		for(int i=0;i<idx.size();i++)
		{
			pick.push_back(Q[idx[i]-1]);
		}

		/* Q=remove(Q,idx); */
		/*for(int i=0;i<idx.size();i++)
		{
			printf("%d,",idx[i]);
		}
		printf("\n");
		for(int i=0;i<Q.size();i++)
		{
			printf("%d,",Q[i]);
		}
		printf("\n\n");*/
		Q = remove1( Q, idx);/////////////////////////////////////////////////////////
		
		/*printf("%d",Q.empty());
		cvWaitKey(0);*/
		
		vector<float> f;

		/* f=min(fc,max(K(:,pick),[],2)); */
		/********************************************/
		/*		  构造由K转化的稠密矩阵计算	  	    */
		/*int sidelength = (FC->rows)*(FC->cols);
		float **Kmatrix = new float *[sidelength] ;
		for(int r=0 ; r<sidelength ; r++)
			Kmatrix[r] = new float [sidelength] ;
		for(int i=0; i<sidelength; i++)
			for(int j=0; j<sidelength; j++)
			{
				Kmatrix[i][j] = 0.0;
			}
		for(int i=0;i<K.size();i++)
		{
			Kmatrix[K[i].x][K[i].y] = K[i].value;
		}*/

		/********************************************/
		/*			稀疏矩阵vector表示计算			*/
		/*int sidelength = (FC->rows)*(FC->cols);
		for(int i=0;i<sidelength;i++)
		{
			float temp = 0.0;
			for(int j=0;j<pick.size();j++)
			{
				for(int k=0;k<K.size();k++)
				{
					if(K[k].x==(pick[j]-1) && K[k].y==i)
					{
						if( temp < K[k].value )
							temp = K[k].value;
					}
				}
			}
			f.push_back(temp);
		}*/

		/********************************************/
		/*		稀疏矩阵map化表示计算第2部分		*/
		time1=clock();
		for(int i=0;i<sidelength;i++)
		{
			float temp = 0.0;
			if(xy_map.count(i))
			{
				for(int j=0;j<pick.size();j++)
				{
					if(xy_map[i].count(pick[j]-1))
					{
						if( temp < xy_map[i][pick[j]-1] )
							temp = xy_map[i][pick[j]-1];
					}
				}
			}
			f.push_back(temp);
		}
		time2=clock();
		time = time +( time2- time1);
		/********************************************/

		for(int i=0;i<f.size();i++)
		{
			if( f[i] > fc )
				f[i] = fc;
		}
		//////////////////////////////////////
		/*printf("%f\n",fc);
		for(int i=0;i<idx.size();i++)
		{
			printf("%d,",idx[i]);
		}
		printf("\n");
		for(int i=0;i<pick.size();i++)
		{
			printf("%d,",pick[i]);
		}
		printf("\n");
		for(int i=0;i<Q.size();i++)
		{
			printf("%d,",Q[i]);
		}
		printf("\n");
		for(int i=0;i<f.size();i++)
		{
			printf("%f,",f[i]);
		}
		printf("\n");printf("\n");*/
		///////////////////////////////////////
		/*for(int i=0;i<f.size();i++)
		{
			printf("%f\n",f[i]);
		}
		cvWaitKey(0);*/

		/* idx=find(f>FC(:)); */
		idx.clear();
		iidx = 1;
		for(int i=0;i<f.size();i++)
		{
			if( CV_MAT_ELEM( *FC, float, i%(FC->rows), i/(FC->rows)) < f[i] )
			{
				idx.push_back(iidx);
			}
			iidx++;
		}
		/*for(int i=0;i<FC->cols;i++)
			for(int j=0;j<FC->rows;j++)
			{
				if( CV_MAT_ELEM( *FC, float, i, j) < f[(FC->rows)*i+j] )
					idx.push_back(iidx);
				iidx++;
			}*/
		/*for(int i=0;i<idx.size();i++)
		{
			printf("%d\n",idx[i]);
		}printf("\n");*/

		/* FC(idx)=f(idx); */
		for(int i=0;i<idx.size();i++)
		{
			CV_MAT_ELEM( *FC, float, (idx[i]-1)%(FC->rows), (idx[i]-1)/(FC->rows)) = f[idx[i]-1];
			//printf("%f\n",f[idx[i]-1]);
		}
		//cvWaitKey(0);
		/* Q=[Q;idx]; */
		for(int i=0;i<idx.size();i++)
		{
			Q.push_back(idx[i]);
		}
	//	for(int i=0;i<idx.size();i++)
	//	{
	//		printf("%d\n",Q[i]);
	//	}
	//	printf("\n");
	//	//cvWaitKey(0);
	}
	//cvWaitKey(0);
	printf("核心计算时间：%d毫秒\n",time);
	return FC;
}

vector<CodNode> remove( vector<CodNode> Q, vector<CodNode> idx)
{
	for(int i=0;i<idx.size();i++)
	{
		for(vector<CodNode>::iterator iter=Q.begin(); iter!=Q.end(); )
		{
			if( ((*iter).x==idx[i].x)&&((*iter).y==idx[i].y) )
					iter = Q.erase(iter);
				else
					iter ++ ;
		}
	}

	return Q;
}

vector<int> remove1( vector<int> Q, vector<int> idx)
{
	vector<int> temp;

	for(int i=0;i<idx.size();i++)
	{
		Q[idx[i]-1]=0;
	}
	for(int i=0;i<Q.size();i++)
	{
		if(Q[i]!=0)
			temp.push_back(Q[i]);
	}
	/*printf("%d",temp.size());
	cvWaitKey(0);*/
	return temp;
}

void Dijkstra( int n, int v, float dist[], int prev[], float * * c)
{
	const float maxint = 9999.9;
	bool s[9999];   // maxint是个非常大的数

    // 其中n指n个节点,v指起点,dist[i]记录源点到i点的最短特殊路径,
	// prev[i]记录在特殊路径当中i点的前一个点,c[][]就是有向图的邻接矩阵
    int i,j;
	
    for (i=1;i<=n;i++)
    {
        dist[i] = c[v][i];
        s[i] = false;
        if (dist[i] > 9000.0)
			prev[i] = 0;   // 将该点的前一个点赋为0,因为它不与v点直接相连
        else
			prev[i] = v;   
	}
	dist[v] = 0.0; s[v] = true;      // 初始时从源点出发
	for (i=1;i<n;i++)
	{
		float temp = maxint;
		int u = v;
		for (j=1;j<=n;j++)
		{
			if ((!s[j]) && (dist[j]<temp-1.0))
			{
				u = j;
				temp = dist[j];
			}
		}
		s[u] = true;
		for (j=1;j<=n;j++)
		{
			if ((!s[j]) && c[u][j]<maxint-1.0)
			{
				float newdist = dist[u] + c[u][j];     //newidist为从源点到该点的最短特殊路径
				if (newdist<dist[j])
				{
					dist[j] = newdist;
					prev[j] = u;
				}
			}
		}
	}
}