#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
using namespace std;
const int maxn=110000,maxm=210000;
inline int Min(int a,int b){return a<b?a:b;}
struct Edge {
	int y,ne;
};

struct Tarjan_Cut_Point {
	int d[maxn],low[maxn],index;
	int *st,ee;
	Edge *e;
	int n;
	int s[maxn],ss;
	int color[maxn],cc;//双连通分量染色，0 .. cc-1，cc表示连通分量数
	int expt[maxn];//一个割点可能属于很多分块，但是每个分块只有唯一一个“借来”的点，就是最高点。该数组保存这个特殊的最高点
	//注意由于是借来的，在遍历相同颜色分块的时候，要特判这个点，以及遍历完后该点的所有信息要还原！
	bool isCut[maxn];//一个点是否是割点
	void init(Edge* _e,int* _st,int _ee,int _n) { //传进双向边集，以及点数n,点编号0..n-1
		e=_e; st=_st; ee=_ee; n=_n;
		for (int i = 0; i<n; i++) {
			d[i]=low[i]=isCut[i]=0;
			color[i]=-1;
		}
		index=0;ss=-1;
		cc=0;
	}
	void dfs(int u,int fa) { //Tarjan部分
		int i,son=0;
		d[u]=low[u]=++index;
		s[++ss]=u;
		for (i=st[u];i!=-1;i=e[i].ne) if (e[i].y!=fa) {
			if (d[e[i].y]==0) {
				son++;//统计儿子数，树根要用
				dfs(e[i].y, u);
				low[u]=Min(low[u], low[e[i].y]);
				if (low[e[i].y]>=d[u]) {
					isCut[u] = 1;//如果非树根，该点是割点
					while (s[ss]!=e[i].y) {
						color[s[ss--]]=cc;
					}
					color[s[ss--]] = cc;//将该儿子所在一下点全部出栈，这些点，加上u，是一个分块
					color[expt[cc]=u] = cc;//u作为特殊点保存
					cc++;
				}
			}
			else {
				low[u]=Min(low[u],d[e[i].y]);
			}
		}
		if (u==fa) {
			isCut[u] = (son>1);//树根是割点的条件：有两个以上儿子
		}
	}
	void work() {
		for (int i = 0; i<n; i++) if (d[i]==0) {
			ss = -1;
			dfs(i, i);
			if (color[i]==-1) color[i] = cc++;//防止该连通分量只有一个点的情况 这种情况算不算一个分块根据题目要求而定
		}
	}
};
Tarjan_Cut_Point T;
Edge e[maxm];
int st[maxn],ee;
void addedge(int x,int y) {
	e[ee].y=y;e[ee].ne=st[x];st[x]=ee++;
	e[ee].y=x;e[ee].ne=st[y];st[y]=ee++;
}
int color[maxn];
int h[maxn];
int que[maxn],head,tail;
int bfs(int k,int c) {
	que[0]=k;head=0;tail=1;
	while (head<tail) {
		k=que[head++];
		for (int i=st[k]; i!=-1; i=e[i].ne) if (T.color[e[i].y]==c||e[i].y==T.expt[c])
			if (color[e[i].y]==-1) {
				color[e[i].y] = 1-color[k];
				que[tail++] = e[i].y;
			}
			else if (color[e[i].y]==color[k]) return 1;
	}
	return 0;
}

char buf[1048576],*o;
int getint() {
	while (!isdigit(*o)) o++;
	int f = 0;
	while (isdigit(*o)) f=f*10+*o-'0',o++;
	return f;
}

int can[maxn];
int main() {
#ifdef _DEBUG
	freopen("D.in", "r", stdin);
#endif
	int n,m,i,j,k;
	int cass;
	for (scanf("%d\n",&cass);cass--;) {
		o = gets(buf);
		n = getint(); m = getint();
		for (i=0;i<n;i++) st[i]=color[i]=h[i]=-1,can[i]=0;
		ee=0;
		while (m--) {
			o = gets(buf);
			i = getint()-1; j = getint()-1;
			addedge(i,j);
		}

		T.init(e,st,ee,n);
		T.work();


		k=0;
		for (i=0;i<n;i++) {
			if (h[T.color[i]]==-1) {
				color[T.expt[T.color[i]]]=-1;
				color[i]=0;
				h[T.color[i]]=bfs(i,T.color[i]);
				can[T.expt[T.color[i]]]|=h[T.color[i]];
				color[T.expt[T.color[i]]]=-1;
			}
			can[i]|=h[T.color[i]];
		}
		for (i=0;i<n;i++)
			k+=can[i];
		printf("%d\n",k);
	}
	return 0;
}
