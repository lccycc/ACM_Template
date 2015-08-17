/*
最大流模板
SAP算法 共n个点 m条边 邻接矩阵存储 流量存储在flow里面
边数组为c 多次调用时 需将c数组置零重建 vh数组和h数组也需重置 vh[0]=n,其他为0
图中默认 0点 为 源点 n-1为 汇点 ，可修改 
*/
#include <iostream>
#include <cmath>
#define MAXN 300
#define MAXM 1100
using namespace std;

int n,m,c[MAXN][MAXN],h[MAXN],vh[MAXN],augc,flow;

void aug(const int m)
{
	int i,augco,minh;
	minh = n - 1;
	augco = augc;
	if ( m == n -1)
	{
		flow += augc;
		found = 1;
		return;
	}
	for( i=0;i<n;i++)if (c[m][i]>0){
		if (h[i]+1 == h[m]){
			if ( c[m][i] < augc ) augc = c[m][i];
			aug(i);
			if (h[0] >= n) return;
			if (found) break;
			augc = augco;
		}
		if ( minh > h[i] ) minh = h[i];
	}
	if (!found){
		vh[h[m]]--;
		if (vh[h[m]] == 0) h[0] = n;
		h[m] = minh + 1;
		vh[h[m]] ++ ;
	}
	else
	{
		c[m][i] -=augc;
		c[i][m] += augc;
	}
}


int main()
{
	int i,j,k;
	{}//input and build map;
	
	memset(h,0,sizeof(h));
	memset(vh,0,sizeof(vh));vh[0] = n;	 
	flow = 0;
	while ( h[0] < n )
	{
		augc = 0x7FFFFFFF;
		found = 0;
		aug(0);
	}
	
	
	return 0;
}
	
