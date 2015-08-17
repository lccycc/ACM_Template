#include <iostream>
using namespace std;
const int maxn=100;
typedef __int64 lld;
inline lld Max(lld a,lld b){return a>b?a:b;}
lld minl;
struct Matrix
{
	lld mat[maxn][maxn];
	int n,m;
	Matrix(){n=m=maxn;memset(mat,0,sizeof(mat));}
	lld* operator [] (int k) {return mat[k];}
	void printM(){for (int i=0;i<n;i++){ for (int j=0;j<m;j++) cout<<(int)mat[i][j]<<' ';cout<<endl;}}
};
lld mod;
Matrix operator*(Matrix &a,Matrix &b)
{
	Matrix c;
	c.n=a.n;c.m=b.m;
	for (int i=0;i<a.n;i++)
	{
		for (int j=0;j<a.m;j++)
		{
			if (a.mat[i][j]==0)continue;
			for (int k=0;k<b.m;k++)
			{
				c[i][k]+=a[i][j]*b[j][k];
				c[i][k]%=mod;
			}
		}
	}
	return c;
}

Matrix operator ^(Matrix &a,int m)
{
	int j;
	Matrix b,c = a;
	b.n=a.n;b.m=a.m;
	for (j=0;j<b.n;j++) b[j][j]=1;
	//for (i=0;i<b.n;i++) for (j=0;j<b.m;j++) b[i][j]=minl;//如果是floyed形式的用这两行初始化b
	//for (j=0;j<b.n;j++) b[j][j]=0;
	while (m)
	{
		if (m & 1) b = b*c;
		if (m >>= 1) c = c*c;
	}
	return b;
}
