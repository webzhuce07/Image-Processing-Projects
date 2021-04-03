#include "OptimalPath.h"

using namespace std;

unsigned int time6;
rootsMsg RootsMsg;

void OptimalPath( CvMat* I, int rootX, int rootY, float x)
{
	time6=0;
	unsigned int time1,time2;
	unsigned int time3,time4,time5=0;
	time1 = clock();
	CvMat* FlagMat = cvCreateMat( I->rows, I->cols, CV_32FC1);
	cvZero(FlagMat);

	vector<Coordinate> Q;

	//if(true)
	////if(false)
	//{
	//	FindRoots( I, rootX, rootY, FlagMat, RootsMsg, Q);
	//}
	//else
	{
		Coordinate RootNode;
		RootNode.x = rootX;
		RootNode.y = rootY;
		RootNode.FCvalue = 1.0;
		Q.push_back(RootNode);//加入初始结点
		CV_MAT_ELEM( *FlagMat, float, RootNode.y, RootNode.x) = RootNode.FCvalue;//在标志矩阵上标记初始结点
	}

	vector<Coordinate> neighbors;
	Coordinate MaxFC;

	while(!Q.empty())//Q非空
	{
		time3 = clock();
		FindMaxFC(Q);//删除时要交换到最后
		time4 = clock();
		time5 += (time4-time3);
		//sort( Q.begin(), Q.end(), CompareFCvalue);
		MaxFC = Q[Q.size()-1];
		//cout<<Q.size()<<endl;

		if( MaxFC.FCvalue > x )//阈值分割提前至此
		{
			neighbors.clear();
			FindNeighbor( I, neighbors, MaxFC);
			Q.pop_back();//把MaxFC从Q中删除
			for(unsigned int i=0;i<neighbors.size();i++)//each属于MaxFC邻接点
			{
				if( neighbors[i].FCvalue > MaxFC.FCvalue )
					neighbors[i].FCvalue = MaxFC.FCvalue;
				if( neighbors[i].FCvalue > CV_MAT_ELEM( *FlagMat, float, neighbors[i].y, neighbors[i].x) )//从树中o到MaxFC再到c的路径的模糊连接度>c原来的模糊连接度
				{
					CV_MAT_ELEM( *FlagMat, float, neighbors[i].y, neighbors[i].x) = neighbors[i].FCvalue;//把c的模糊连接度设为新值
					//findAndPopOldPixel( Q, neighbors[i]);
					Q.push_back(neighbors[i]);//把c加入Q
				}
			}
		}
		else
		{
			break;//退出while循环；
		}
	}

	printf("循环时间：%d毫秒\n",time5);
	time2 = clock();
	printf("计算时间：%d毫秒\n",time2-time1);
	time1 = clock();
	ShowMatrix( FlagMat, "分割结果图", 500, 500, 1, false);
	time2 = clock();
	printf("绘图时间：%d毫秒\n",time2-time1);
	printf("找最大值时间：%d毫秒\n",time6);

	Comparison( FlagMat);
}

bool CompareFCvalue( const Coordinate &node1, const Coordinate &node2)
{
	return node1.FCvalue < node2.FCvalue;
}

void FindMaxFC( vector<Coordinate> &Q)
{
	float tempFC = 0.0;
	int i = 0;
	int index = -1;
	unsigned int time7,time8;
	time7 = clock();
	for(i;i<Q.size();i++)
	{
		if(Q[i].FCvalue>tempFC)
		{
			tempFC = Q[i].FCvalue;
			index = i;
		}
	}
	time8 = clock();
	time6 += (time8-time7);
	i--;	
	Coordinate tempNode;
	tempNode.x = Q[index].x;
	tempNode.y = Q[index].y;
	tempNode.FCvalue = Q[index].FCvalue;
	Q[index].x = Q[i].x;
	Q[index].y = Q[i].y;
	Q[index].FCvalue = Q[i].FCvalue;
	Q[i].x = tempNode.x;
	Q[i].y = tempNode.y;
	Q[i].FCvalue = tempNode.FCvalue;
}

void FindRoots( CvMat *I, int rootX, int rootY, CvMat *FlagMat, rootsMsg RootsMsg, vector<Coordinate> &Q)
{
	float l = 0.5;
	int x = rootX;
	int y = rootY;
	float gray_m = 0.0;//平均值
	float gray_d = 0.0;//标准差

	for(int i=-1;i<=1;i++)
		for(int j=-1;j<=1;j++)
		{
			gray_m += CV_MAT_ELEM( *I, float, y+i, x+j);
		}
	gray_m = gray_m/9;

	for(int i=-1;i<=1;i++)
		for(int j=-1;j<=1;j++)
		{
			gray_d += (CV_MAT_ELEM( *I, float, y+i, x+j)-gray_m)*(CV_MAT_ELEM( *I, float, y+i, x+j)-gray_m);
		}
	gray_d = gray_d/9;
	gray_d = sqrt(gray_d);

	for(int i=-1;i<=1;i++)
		for(int j=-1;j<=1;j++)
		{
			CV_MAT_ELEM( *FlagMat, float, y+i, x+j) = 1.0;
				
//			if( (i!=0) && (j!=0) )
//			{
			Coordinate temp;
			temp.x = x+j;
			temp.y = y+i;
			temp.FCvalue = 1.0;
			Q.push_back(temp);
//			}
		}

	RegionGrowing( I, FlagMat, Q, gray_m, gray_d);

	CvMat* sobel = cvCreateMat( I->rows, I->cols, CV_32FC1);//void cvSobel( const CvArr* src, CvArr* dst, int xorder, int yorder, int aperture_size=3 );
	cvZero(sobel);
	cvSobel( I, sobel, 1, 1, 3);
	ShowMatrix( sobel, "梯度图", 780, 200, 1, false);
	ShowMatrix( FlagMat, "标志矩阵图", 220, 500, 1, false);
	/*---------------------------------------------求梯度平均值*/
	float sobel_sum = 0.0;
	int sobel_num = 0;
	float sobel_mean;
	for(int i=0;i<I->rows;i++)
		for(int j=0;j<I->cols;j++)
		{
			if(CV_MAT_ELEM( *FlagMat, float, i, j)>0.9999)
			{
				sobel_sum += CV_MAT_ELEM( *sobel, float, i, j);
				sobel_num++;
			}
		}
	sobel_mean = sobel_sum/sobel_num;
	/*-----------------------------------------------求梯度方差*/
	float sobel_dsum = 0.0;
	float sobel_d;
	for(int i=0;i<I->rows;i++)
		for(int j=0;j<I->cols;j++)
		{
			if(CV_MAT_ELEM( *FlagMat, float, i, j)>0.9999)
			{
				sobel_dsum += ( CV_MAT_ELEM( *sobel, float, i, j) - sobel_mean )*( CV_MAT_ELEM( *sobel, float, i, j) - sobel_mean );
			}
		}
	sobel_d = sqrt(sobel_dsum/sobel_num);
	/*for(int i=0;i<I->cols;i++)
	{
		for(int j=0;j<I->rows;j++)
		{printf("%f",CV_MAT_ELEM( *sobel, float, i, j));}
		printf("\n");
	}*/

	RootsMsg.m1 = gray_m;
	RootsMsg.m2 = sobel_mean;
	RootsMsg.s1 = gray_d;
	RootsMsg.s2 = sobel_d;
	printf("灰度平均值:%f\n",RootsMsg.m1);
	printf("梯度平均值:%f\n",RootsMsg.m2);
	printf("灰度标准差:%f\n",RootsMsg.s1);
	printf("梯度标准差:%f\n",RootsMsg.s2);
	//cvWaitKey(0);
}

void RegionGrowing( CvMat *I, CvMat *FlagMat, vector<Coordinate> &Q, float gray_m,	float gray_d)
{
	queue<Coordinate> q;
	int RootsNum = 0;
	for(int i=0;i<FlagMat->rows;i++)
		for(int j=0;j<FlagMat->cols;j++)
		{
			if(CV_MAT_ELEM( *FlagMat, float, i, j)>0.01)
			{
				RootsNum++;
				Coordinate temp;
				temp.x = j;
				temp.y = i;
				temp.FCvalue = 1.0;
				q.push(temp);
			}
		}
	//--------------------------------------------------------------------------
	Coordinate tempNood;
	float nX;
	gray_d = gray_d * 1.0;//范围系数
	while(!q.empty())
	{
		tempNood = q.front();
		if( (tempNood.y-1)>=0 && CV_MAT_ELEM( *FlagMat, float, tempNood.y-1, tempNood.x)<0.1)
		{
			nX = CV_MAT_ELEM( *I, float, tempNood.y-1, tempNood.x);
			if( nX>=(gray_m - gray_d) && nX<=(gray_m + gray_d) )
			{
				CV_MAT_ELEM( *FlagMat, float, tempNood.y-1, tempNood.x) = 1.0;
				Coordinate tempNood1;
				tempNood1.x = tempNood.x;
				tempNood1.y = tempNood.y-1;
				tempNood1.FCvalue = 1.0;
				q.push(tempNood1);
				RootsNum++;
				gray_m = (gray_m * (RootsNum-1) + nX) / RootsNum;
				gray_d = (gray_d * gray_d * (RootsNum-1) + (nX - gray_m)*(nX - gray_m)) / RootsNum;
				gray_d = sqrt(gray_d);
			}
		}
		if( (tempNood.x+1)<FlagMat->cols && CV_MAT_ELEM( *FlagMat, float, tempNood.y, tempNood.x+1)<0.1)
		{
			nX = CV_MAT_ELEM( *I, float, tempNood.y, tempNood.x+1);
			if( nX>=(gray_m - gray_d) && nX<=(gray_m + gray_d) )
			{
				CV_MAT_ELEM( *FlagMat, float, tempNood.y, tempNood.x+1) = 1.0;
				Coordinate tempNood1;
				tempNood1.x = tempNood.x+1;
				tempNood1.y = tempNood.y;
				tempNood1.FCvalue = 1.0;
				q.push(tempNood1);
				RootsNum++;
				gray_m = (gray_m * (RootsNum-1) + nX) / RootsNum;
				gray_d = (gray_d * gray_d * (RootsNum-1) + (nX - gray_m)*(nX - gray_m)) / RootsNum;
				gray_d = sqrt(gray_d);
			}
		}
		if( (tempNood.y+1)<FlagMat->rows && CV_MAT_ELEM( *FlagMat, float, tempNood.y+1, tempNood.x)<0.1)
		{
			nX = CV_MAT_ELEM( *I, float, tempNood.y+1, tempNood.x);
			if( nX>=(gray_m - gray_d) && nX<=(gray_m + gray_d) )
			{
				CV_MAT_ELEM( *FlagMat, float, tempNood.y+1, tempNood.x) = 1.0;
				Coordinate tempNood1;
				tempNood1.x = tempNood.x;
				tempNood1.y = tempNood.y+1;
				tempNood1.FCvalue = 1.0;
				q.push(tempNood1);
				RootsNum++;
				gray_m = (gray_m * (RootsNum-1) + nX) / RootsNum;
				gray_d = (gray_d * gray_d * (RootsNum-1) + (nX - gray_m)*(nX - gray_m)) / RootsNum;
				gray_d = sqrt(gray_d);
			}
		}
		if( (tempNood.x-1)>=0 && CV_MAT_ELEM( *FlagMat, float, tempNood.y, tempNood.x-1)<0.1)
		{
			nX = CV_MAT_ELEM( *I, float, tempNood.y, tempNood.x-1);
			if( nX>=(gray_m - gray_d) && nX<=(gray_m + gray_d) )
			{
				CV_MAT_ELEM( *FlagMat, float, tempNood.y, tempNood.x-1) = 1.0;
				Coordinate tempNood1;
				tempNood1.x = tempNood.x-1;
				tempNood1.y = tempNood.y;
				tempNood1.FCvalue = 1.0;
				q.push(tempNood1);
				RootsNum++;
				gray_m = (gray_m * (RootsNum-1) + nX) / RootsNum;
				gray_d = (gray_d * gray_d * (RootsNum-1) + (nX - gray_m)*(nX - gray_m)) / RootsNum;
				gray_d = sqrt(gray_d);
			}
		}
		q.pop();
	}
	//---------------------------------------------------------------------------
	for(int i=1;i<(FlagMat->rows)-1;i++)
		for(int j=1;j<(FlagMat->cols)-1;j++)
		{
			if(CV_MAT_ELEM( *FlagMat, float, i, j)>0.9)
			{
				Coordinate temp;
				temp.x = j;
				temp.y = i;
				temp.FCvalue = 1.0;
				Q.push_back(temp);
			}
		}
	//ShowMatrix1( FlagMat, "结果", 680, 600, 2, false);
	//cvWaitKey(0);
}

float ComputeFCvalue( CvMat* I, const Coordinate &node1, const Coordinate &node2)//基础FC求解方式
{
	return Method1( I, node1, node2);
	//return Method2( I, node1, node2, RootsMsg);
}

float Method1( CvMat* I, const Coordinate &node1, const Coordinate &node2)//基础FC求解方式
{
	float k = 50;
	float node1value = CV_MAT_ELEM( *I, float, node1.y, node1.x);
	float node2value = CV_MAT_ELEM( *I, float, node2.y, node2.x);
	return float(1.0/(1.0+k*abs(node1value-node2value)));
}

//h1:图像亮度的高斯量度;h2:图像梯度的高斯量度
//m1:灰度平均值;m2:梯度平均值
//s1:灰度标准差;s2:梯度标准差
float Method2( CvMat* I, const Coordinate &node1, const Coordinate &node2, const rootsMsg RootsMsg)
{
	float h1,h2,x1,x2;
	x1 = (((CV_MAT_ELEM( *I, float, node1.y, node1.x)+CV_MAT_ELEM( *I, float, node2.y, node2.x))/2)-RootsMsg.m1)/RootsMsg.s1;
	h1 = float(exp(-0.5 * x1 * x1));
	x2 = ((abs(CV_MAT_ELEM( *I, float, node1.y, node1.x)-CV_MAT_ELEM( *I, float, node2.y, node2.x))/2)-RootsMsg.m2)/RootsMsg.s2;
	h2 = float(exp(-0.5 * x2 * x2));
	float w1,w2;
	w1 = h1/(h1+h2);
	w2 = 1 - w1;
	return w1*h1+w2*h2;
}

void FindNeighbor( CvMat *I, vector<Coordinate> &neighbors, Coordinate &MaxFC)
{
	int x = MaxFC.x;
	int y = MaxFC.y;
	
	if( (y-1)>=0 )
	{
		Coordinate node0;
		node0.x = x;
		node0.y = y-1;
		node0.FCvalue = ComputeFCvalue(I,MaxFC,node0);
		neighbors.push_back(node0);
	}
	if( (x+1)<I->cols )
	{
		Coordinate node1;
		node1.x = x+1;
		node1.y = y;
		node1.FCvalue = ComputeFCvalue(I,MaxFC,node1);
		neighbors.push_back(node1);
	}
	if( (y+1)<I->rows )
	{
		Coordinate node2;
		node2.x = x;
		node2.y = y+1;
		node2.FCvalue = ComputeFCvalue(I,MaxFC,node2);
		neighbors.push_back(node2);
	}
	if( (x-1)>=0 )
	{
		Coordinate node3;
		node3.x = x-1;
		node3.y = y;
		node3.FCvalue = ComputeFCvalue(I,MaxFC,node3);
		neighbors.push_back(node3);
	}
}
