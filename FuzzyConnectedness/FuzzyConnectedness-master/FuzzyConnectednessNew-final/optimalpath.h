#pragma once

#include"common.h"
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include <queue>

using namespace std;

struct Coordinate
{
	int x;
	int y;
	float FCvalue;
};

struct rootsMsg
{
	float m1;
	float m2;
	float s1;
	float s2;
};

struct OptimalPathNode
{
	OptimalPathNode* leftchild;
	OptimalPathNode* rightchild;
	int x;
	int y;
	float FCvalue;
};

CvMat* OptimalPath( CvMat *I, int rootX, int rootY, float x);
void FindRoots( CvMat *I, int rootX, int rootY, CvMat *FlagMat, vector<Coordinate> &Q);
void RegionGrowing( CvMat *I, CvMat *FlagMat, vector<Coordinate> &Q, float gray_m,	float gray_d);//ÇøÓòÔö³¤
bool CompareFCvalue( const Coordinate &node1, const Coordinate &node2);
float Method1( CvMat* I, const Coordinate &node1, const Coordinate &node2);
float Method2( CvMat* I, const Coordinate &node1, const Coordinate &node2);
void FindMaxFC( vector<Coordinate> &Q);
float ComputeFCvalue( CvMat* I, const Coordinate &node1, const Coordinate &node2);
void FindNeighbor( CvMat *I, vector<Coordinate> &neighbors, Coordinate &MaxFC);



inline void findAndPopOldPixel( vector<Coordinate>&Q, Coordinate &a)
{	
	for(int i=0;i<Q.size();i++)
	{
		if( Q[i].x==a.x && Q[i].y==a.y )
		{
			Q[i].FCvalue = a.FCvalue;
			break;
		}
	}
}