#include <iostream>
using namespace std;
const int maxn=1000,maxm=maxn*8*4;//最大顶点数和边数
const int maxl=999999999;
inline int Min(int a,int b){return a<b?a:b;}
inline int Max(int a,int b){return a>b?a:b;}
struct st
{
	int y,d;
	int ne;
	int bro;
	int f;
} e[maxm];
int ee;
int st[maxn];
int n,m;
void addedge(int x,int y,int d,int f)
{//给顶点x和y间添加一条费用d,流量f的边
	e[ee].y=y;e[ee].d=d;e[ee].ne=st[x];e[ee].f=f;st[x]=ee++;
	e[ee].y=x;e[ee].d=-1*d;e[ee].ne=st[y];e[ee].f=0;st[y]=ee++;
	e[ee-2].bro=ee-1;e[ee-1].bro=ee-2;
}
int d[maxn],p[maxn];
//spfa所用到起点的最短距离(这里距离相当于cost)和路径记录之前的一个节点
int c[maxn];//spfa所用数组:是否在队列中
int que[maxn],head,tail;//spfa专用队列
int spfa(int sx,int ex)//求sx到ex的一次费用增广
{//如果没有增广路就返回maxl 否则返回费用
	int i,j,k;
	for (i=0;i<maxn;i++) d[i]=maxl;//初始化d为maxl
	memset(c,0,sizeof(c));//初始化都没进
	d[sx]=0;
	que[head=0]=sx;tail=1;
	c[sx]=1;
	while (head!=tail)//spfa开始
	{
		k=que[head++];head%=maxn;
		c[k]=0;
		for (i=st[k];i!=-1;i=e[i].ne) if (e[i].f)
			if (d[k]+e[i].d<d[e[i].y])
			{
				d[e[i].y]=d[k]+e[i].d;
				p[e[i].y]=i;
				if (c[e[i].y]==0)
				{
					c[e[i].y]=1;
					if (e[i].d<0){head=(head-1+maxn)%maxn;que[head]=e[i].y;}
					else 
					{que[tail++]=e[i].y;tail%=maxn;}
				}
			}
	}
	if (d[ex]==maxl) return maxl;//如果无法到达终点返回maxl
	k=maxl;
	for (i=ex;i!=sx;i=e[e[p[i]].bro].y)	k=Min(k,e[p[i]].f);//计算流
	for (i=ex;i!=sx;i=e[e[p[i]].bro].y)//增加反向边
	{
		e[p[i]].f-=k;
		e[e[p[i]].bro].f+=k;
	}
	return d[ex]*k;//返回费用为流大小*路径长度(cost累加)
}
