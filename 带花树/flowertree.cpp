#include <iostream>
using namespace std;
const int maxn=410,maxl=999999999;
int g[maxn][maxn];
int l[maxn][3];
int p[maxn];
int n;
int x[maxn],y[maxn];
int e[maxn][maxn],ee[maxn];
void iin()
{
	int i,j,len;
	memset(g,0,sizeof(g));
	memset(p,0,sizeof(p));
	for (i=1;i<=n;i++) scanf("%d%d",x+i,y+i);
	scanf("%d",&len);
	memset(ee,0,sizeof(ee));
	for (i=1;i<=n;i++) for (j=1;j<=n;j++) if (i!=j) if (abs(x[i]-x[j])+abs(y[i]-y[j])<=len)
	{
		g[i][j]=1;
		e[i][ee[i]++]=j;
	}
}
void findway(int i,int j)
{
	int q=i,k=j;
	p[j]=i;
	while (l[q][2]!=maxl)
	{
		p[q]=k;k=l[q][2];
		p[k]=l[k][1];
		q=l[k][1];
	}
	p[q]=k;
}
int s[maxn]={0},ss=0;
int flowertree()
{
	int i,j,q,k,v;
	int more;
	int et;
	do
	{
		more=0;
		for (i=1;i<=n;i++)
			if (l[i][2]>0)
				for (et=0;et<ee[i];et++)
				{
					j=e[i][et];
					if (g[i][j]>0&&p[i]!=j)
						if (l[j][1]==0&&l[j][2]==0)
							if (p[j]==0)
							{
								findway(i,j);
								return 1;
							}
							else
							{
								g[i][j]*=-1;g[j][i]*=-1;
								l[j][1]=i;l[p[j]][2]=j;
								g[j][p[j]]*=-1;
								g[p[j]][j]*=-1;
								more=1;
							}
						else if (l[j][1]==0&&l[j][2]>0)
						{
							more=1;
							g[i][j]*=-1;g[j][i]*=-1;
							ss++;
							s[i]=ss;
							k=i;v=2;
							while (l[k][v]!=maxl)
							{
								k=l[k][v];
								s[k]=ss;
								v=3-v;
							}
							for(k=j,v=2;s[k]!=ss;v=3-v)
								k=l[k][v];
							if (l[i][1]==0)
							{
								l[i][1]=j;q=i;v=2;
								while (q!=k)
								{
									l[l[q][v]][v]=q;
									q=l[q][v];
									v=3-v;
								}
							}
							l[j][1]=i;q=j;v=2;
							while (q!=k)
							{
								l[l[q][v]][v]=q;q=l[q][v];v=3-v;
							}
						}
				}
	}
	while (more);
	return 0;
}
int workmain()
{
	int i,j,k,tot=0;
	int b;
	do
	{
		i=0;
		do
		{
			i++;
			if (p[i]==0)
			{
				memset(l,0,sizeof(l));
				l[i][2]=maxl;
				b=flowertree();
				for (j=1;j<=n;j++) for (k=1;k<=n;k++) g[j][k]=abs(g[j][k]);
			}
			else b=0;
		}while (i<=n&&b==0);
		
		if (i<=n) tot++;
	}
	while (i<=n&&tot!=n/2);
	return tot;
}
int main()
{
	#ifdef _DEBUG
		freopen("in.in", "r", stdin);
	#endif
	while (scanf("%d",&n)!=EOF)
	{
		iin();
		if (n%2||workmain()*2!=n) puts("NO");
		else puts("YES");
	}
	return 0;
}