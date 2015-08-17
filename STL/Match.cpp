#include <iostream>
using namespace std;
const int maxn=200;
int f[maxn][maxn];
int n,m;
int c[maxn],p[maxn];
void iin()
{
	cin>>n>>m;
	memset(f,0,sizeof(f));
	int i,j,k;
	for (i=0;i<m;i++)
	{
		cin>>j>>k;
		f[j][k]=1;
	}
}
int dfs(int k)
{
	int i;
	for (i=1;i<=n;i++) if (f[k][i])
		if (!c[i])
		{
			c[i]=1;
			if (p[i]==0||dfs(p[i]))
			{
				p[i]=k;
				return 1;
			}
		}
	return 0;
}
int main()
{
	
	int ssss;
	for (cin>>ssss;ssss>0;ssss--)
	{
		iin();
		memset(p,0,sizeof(p));
		int i,j=0;
		for (i=1;i<=n;i++)
		{
			memset(c,0,sizeof(c));
			if (dfs(i)) j++;
		}
		cout<<j<<endl;//j为最大匹配数，若此处改为n-j则为最小支配集=最大独立集
	}
	return 0;
}
