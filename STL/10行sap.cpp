#include <cstdio>
#include <cstring>
const int maxv=56000,maxe=320000,inf=0x3f3f3f3f;
int head[maxv],c[maxe],v[maxe],next[maxe],d[maxv],vh[maxv],now[maxv],
	ss,tt,n,m,l=1;
void add(int _u,int _v,int _c)
{
	v[++l]=_v;c[l]=_c;next[l]=head[_u];head[_u]=l;
	v[++l]=_u;c[l]= 0;next[l]=head[_v];head[_v]=l;
}
int sap(int i,int m)
{
	if (i==tt) return m;
	int l=m,min=tt-1;
	for (int&j=now[i];j;j=next[j]) if (c[j]&&d[i]==d[v[j]]+1)
	{
		int t=sap(v[j],l<c[j]?l:c[j]);
		l-=t;c[j]-=t;c[j^1]+=t;
		if (!l||d[ss]>=tt) return m-l;
	}
	for (int j=now[i]=head[i];j;j=next[j]) if (c[j]&&d[v[j]]<min) min=d[v[j]];
	if (m==l) if (!--vh[d[i]]) d[ss]=tt;else ++vh[d[i]=min+1];
	return m-l;
}
int main()
{
	freopen("profit.in","r",stdin);
	freopen("profit.out","w",stdout);
	int ans=0,t,a,b;
	scanf("%d%d",&n,&m);ss=n+m+1;tt=n+m+2;
	for (int i=1;i<=n;++i){scanf("%d",&t);add(ss,i,t);}
	for (int i=n+1;i<=n+m;++i)
	{
		scanf("%d%d%d",&a,&b,&t);
		add(a,i,inf);add(b,i,inf);add(i,tt,t);ans+=t;
	}
	for (int i=1;i<=tt;++i) now[i]=head[i];
	vh[0]=tt;
	while (d[ss]<tt) ans-=sap(ss,inf);
	printf("%d\n",ans);
}