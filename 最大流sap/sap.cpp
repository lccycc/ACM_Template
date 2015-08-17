#include <iostream>
#include <cstring>
using namespace std;
const int maxn=20100,maxm=200100*4;
typedef int lld;
struct Edge{
	int y,f,ne;
} e[maxm];
int ee;
int st[maxn];
void addedge(int x,int y,int f,int ff) { //x->y增加一条容量f的正向边和容量ff的反向边
	e[ee].y=y;e[ee].f=f;e[ee].ne=st[x];st[x]=ee++;
	e[ee].y=x;e[ee].f=ff;e[ee].ne=st[y];st[y]=ee++;
}
#define INF (lld)1<<30
int h[maxn],vh[maxn];
lld flow;
lld aug(const int sx,const int m,const int ex,const lld t,int n) { //目标ex t是流量 m是当前所在节点
	int i,minh;
	lld l,d;
	if(m== ex) return t;
	l = t;
	minh = n - 1;
	for(i=st[m];i!=-1;i=e[i].ne) if(e[i].f>0){
		if(h[e[i].y]+1==h[m]){
			if(l<e[i].f) d=l; else d=e[i].f;
			d = aug(sx,e[i].y,ex,d,n);
			e[i].f-=d;
			e[i^1].f+=d;
			l-=d;
			if(h[sx]>=n) return t-l;
			if (l==0) break;
		}
		if(minh>h[e[i].y]) minh=h[e[i].y];
	}
	if(l==t) {
		vh[h[m]]--;
		if(vh[h[m]]==0) h[sx]=n;
		h[m] = minh + 1;
		vh[h[m]]++;
	}
	return t-l;
}
lld maxflow(int sx,int ex,int n) {
	memset(h,0,sizeof(int)*n);
	memset(vh,0,sizeof(int)*n);
	vh[sx] = n;
	flow=0;
  while( h[sx]<n )
    flow += aug(sx,sx,ex,INF,n);
	return flow;
}
