#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
const int maxn=1100000,maxm=2*maxn;
struct Edge {
	int y;
	int ne;
} e[maxm];
int st[maxn],ee;
void addedge(int x,int y) {
	e[ee].y=y;e[ee].ne=st[x];st[x]=ee++;
}
int p[maxn],c[maxn];
int aug(int k,int m,int hash) {
	int i;
	if (c[k]==hash) return 0;
	c[k]=hash;
	for (i=st[k];i!=-1;i=e[i].ne) if (e[i].y<m&&p[e[i].y]==-1) {
		p[e[i].y]=k;
		return 1;
	}
	for (i=st[k];i!=-1;i=e[i].ne) if (e[i].y<m&&aug(p[e[i].y],m,hash)) {
		p[e[i].y]=k;
		return 1;
	}
	return 0;
}
int maxmatch(int n,int m) {
	memset(p,-1,sizeof(int)*m);
	memset(c,0,sizeof(int)*n);
	int tot=0;
	for (int i = 0; i<n; i++) {
		tot+=aug(i,m,i+1);
		if (tot>=m) return 1;
	}
	return 0;
}

int main() {
	freopen("in.in","r",stdin);
	int cass;
	for (scanf("%d",&cass);cass--;) {
		int n;
		scanf("%d",&n);
		int i,x,y,z1,z2;
		memset(st,-1,sizeof(st));
		ee=0;
		for (i=0;i<n;i++) {
			scanf("%d.%d %d.%d",&z1,&x,&z2,&y);
			x=z1*1000000+x-1500000;
			y=z2*1000000+y-1500000;
			addedge(i,x);
			addedge(i,y);
		}
		int l=0,r=n+1;
		while (l+1<r) {
			int mid=(l+r)/2;
			if (maxmatch(n,mid)) l=mid;else r=mid;
		}
		printf("%d\n",l);
	}
	return 0;
}
