#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <cmath>
using namespace std;
const int maxn=220;
const int maxl=9999999;
const double eps=1e-6;
double map[maxn][maxn];
double x[maxn],y[maxn],sl[maxn];
int up[maxn];
int cx[maxn],cy[maxn];
int m,n;
int dfs(int k)
{
	int i;
	double j;
	cx[k]=1;
	for (i=1;i<=n;i++) if (!cy[i])
	{
		j=map[k][i]-x[k]-y[i];
		if (fabs(j)<eps)
		{
			cy[i]=1;
			if (!up[i]||dfs(up[i]))
			{
				up[i]=k;
				return 1;
			}
		}
		else if (sl[i]>j) sl[i]=j;
	}
	return 0;
}

int km()
{
	int i,k;
	double j;
	memset(up,0,sizeof(up));
	for (k=1;k<=n;k++)
	{
		memset(sl,maxl,sizeof(sl));
		memset(cx,0,sizeof(cx));
		memset(cy,0,sizeof(cy));
		while (!dfs(k))
		{
			j=maxl;
			for (i=1;i<=n;i++) 
				if (!cy[i]&&sl[i]<j)
					j=sl[i];
			for (i=1;i<=n;i++) 
			{
				if (cx[i]) x[i]+=j;
				if (cy[i]) y[i]-=j;
			}
			memset(sl,maxl,sizeof(sl));
			memset(cx,0,sizeof(cx));
			memset(cy,0,sizeof(cy));
		}
	}
	double tot=0;
	for (i=1;i<=n;i++) tot+=map[up[i]][i];
	return tot;
}
	
void iin()
{	
	
	int p,q;
	int i,j;
	char c;
	double xm[maxn],ym[maxn],xh[maxn],yh[maxn];

	while (cin>>n)
	{
		memset(map,0,sizeof(map));
		for (i=1;i<=n;i++) cin>>xm[i]>>ym[i];
		for (i=1;i<=n;i++) cin>>xh[i]>>yh[i];
		for (i=1;i<=n;i++)for (j=1;j<=n;j++) map[i][j]=sqrt((xm[i]-xh[j])*(xm[i]-xh[j])+(ym[i]-yh[j])*(ym[i]-yh[j]));

		for (i=1;i<=n;i++)
		{
			x[i]=maxl;
			for (j=1;j<=n;j++)
				if (x[i]>map[i][j]) 
					x[i]=map[i][j];
			y[i]=0;
		}
		km();
		for (i=1;i<=n;i++)	for (j=1;j<=n;j++) if (up[j]==i) cout<<j<<endl;
		break;
	}
	return ;
}
int main()
{
 
    iin();
	return 0;
}
