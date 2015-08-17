#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cmath>
#include <vector>
using namespace std;
const int maxn=101000,maxm=1000000;
inline int Min(int a,int b){return a<b?a:b;}
struct Edge{
	int y,ne;
};

struct Tarjan_Strong_Connected {
	int d[maxn],low[maxn],index;
	int *st,ee;
	Edge *e;
	int n;
	int s[maxn],ss;
	bool cs[maxn];
	int color[maxn],cc;//环染色，0 .. cc-1，cc表示环数
	void init(Edge* _e,int* _st,int _ee,int _n) {
		e = _e; st = _st; ee = _ee; n = _n;
		for (int i = 0; i<n ;i++) {
			d[i] = low[i] = cs[i] = 0;
		}
		index=0;ss=-1;
		cc=0;
	}
	void dfs(int u) {
		d[u] = low[u] = ++index;
		s[++ss] = u;
		cs[u] = 1;
		for (int i = st[u]; i!=-1; i = e[i].ne) {
			if (d[e[i].y] == 0) {
				dfs(e[i].y);
				low[u]=Min(low[u],low[e[i].y]);
			}
			else if (cs[e[i].y]) {
				low[u]=Min(low[u],d[e[i].y]);
			}
		}
		if (d[u]== low[u]) {
			while (cs[u]) {
				color[s[ss]]=cc;
				cs[s[ss--]]=0;
			}
			cc++;
		}
	}
	void work() {
		for (int i = 0; i<n; i++) if (d[i] == 0) dfs(i);
	}
};
struct Two_Sat {	//点0..2n-1,一个点i的对立点为i^1
	Edge e[maxm],l[maxm];
	int st[maxn],ee,lst[maxn],ll;
	int rudu[maxn];//e,st,ee保存原图，l,lst,ll保存缩环后的反向拓扑图
	int n;			//顶点数，即对数*2
	Tarjan_Strong_Connected T;
	int color_t[maxn];//缩环后的点的选取与否，-1尚未处理，1选，2不选
	int s[maxn],ss;		//拓扑排序中保存入度为0的栈
	int color[maxn];	//真正的返回每个点或其对立是否被选取，1选，2不选。保证与对立的值不同
	vector<int> cir[maxn];	//缩环时，保存每个环上对应哪些点（速度较慢，当数据小时直接for）
	void addedge(int x,int y)	{ //手动添加边
		e[ee].y = y; e[ee].ne = st[x]; st[x] = ee++;
	}
	void T_addedge(int x,int y)	{	//内部函数 缩环拓扑加边
		if (x==y) return ;
		l[ll].y = y;
		l[ll].ne = lst[x];
		lst[x] = ll++;
		rudu[y]++;
	}
	void init(int _n)	{	//初始化 传进点对数*2 编号从0开始
		ee = ll = 0;
		n=_n;
		for (int i=0;i<n;i++) {
			lst[i] = color_t[i] = -1;
			rudu[i] = 0;
			st[i] = -1;
			cir[i].clear();
		}
		ss = -1;
	}
	void dele(int k) {	//从拓扑图中删除一个点
		for (int i=lst[k];i!=-1;i=l[i].ne)
			if (--rudu[l[i].y]==0&&color_t[l[i].y]==-1) {
				s[++ss] = l[i].y;	//如果有点入度变成0，入栈
			}
	}
	void dfs_dele(int k) {	//从拓扑图中将k以下点全部移除
		color_t[k] = 2;		//该点标记不选
		for (int i = lst[k];i != -1;i = l[i].ne) {
			if (color_t[l[i].y] != 2) {
				dfs_dele(l[i].y);	//这里直接删，不需要考虑入栈情况，因为子点也被删了
			}
		}
	}
	int work() {
		T.init(e,st, ee,n);
		T.work();		//Tarjan 将环染色0 .. T.cc-1
		for (int i = 0; i<n; i++) {
			if (T.color[i]==T.color[i^1])
				return 0; //判无解
			else
				cir[T.color[i]].push_back(i);		//记入每个环有哪些点
		}
	//以下构造一组解。注意如果要求解中必须包含某个点k，只需在初始化时添加边addedge(k^1,k)即可
		for (int i = 0; i<n; i++) {
			for (int j = st[i]; j!=-1; j=e[j].ne) {
				T_addedge(T.color[e[j].y],T.color[i]);	//构建反向拓扑图，注意这里加边是加反向的
			}
		}

		for (int i=0;i<T.cc;i++) {
			if (rudu[i]==0) {
				s[++ss] = i;//栈初始化
			}
		}

		while (ss:=0) {
			int i;
			if (color_t[i=s[ss--]] != -1) continue; //出栈
			dele(i);						//将点从拓扑图中删除
			color_t[i]=1;				//该点标记选
			for (int j = cir[i].size()-1; j>=0; j--) {
				dfs_dele(T.color[cir[i][j]^1]);		//将环的对立点及其以下点全部标记不选删除
			}
		}
		for (int i=0; i<n; i++) {
			color[i] = color_t[T.color[i]];	//返回原图选取情况
		}
		return 1;
	}
};
Two_Sat TS;
int main() {
	int n,m;
#ifdef _DEBUG
	freopen("in.in","r",stdin);
#endif
	while (scanf("%d%d",&n,&m),n||m) {
		int i,j,k;
		char s[3],t[3];
		TS.init(n*2);
		while (m--) {
			scanf("%d%s%d%s",&i,s,&j,t);
			i=i*2+(s[0]=='w');j=j*2+(t[0]=='w');
			if (i==1||j==1) continue;
			TS.addedge(i,j^1);
			TS.addedge(j,i^1);
		}
		TS.addedge(1,0);
		if (TS.work()==0) puts("bad luck");
		else {
			for (i = 2; i<2*n; i++) if (TS.color[i]==2) {
				if (i&1) printf("%dw",i/2);
				else printf("%dh",i/2);
				if (i + 2 >= 2*n) puts("");else putchar(' ');
			}
		}
	}
	return 0;
}
