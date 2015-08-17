/*
究极多路邻接表网络流模板
n个点 m条边 源点为0 汇点为n-1
加边函数adddege(x,y,a,b) 为x到y添加一条容量为a的边 一条容量为b的反向边 
*/ 
#include <iostream>
#include <cstdio>
#define MAXN 20000 + 10
#define MAXM 1000000 + 10
#define INF (long long)1<<60
using namespace std;

struct edge
{
    int next,p,v;
    long long c;
};
edge e[MAXM*2];
int h[MAXN],vh[MAXN],n,m,hd[MAXN],idx=0;
long long flow;

long long aug(const int m,const long long t)
{
    int i,minh;
    long long l,d;   
    if(m== n -1) return t;    
    l = t;
    minh = n - 1;
    for(i=hd[m];i!=0;i=e[i].next) if(e[i].c>0){
        if(h[e[i].v]+1==h[m]){
            if(l<e[i].c) d=l; else d=e[i].c;
            d = aug(e[i].v,d);
            e[i].c-=d;
            e[e[i].p].c+=d;
            l-=d;
            if(h[0]>=n) return t-l;
            if (l==0) break;
        }
        if(minh>h[e[i].v]) minh=h[e[i].v];
    }
    if(l==t)
    {
        vh[h[m]]--;
        if(vh[h[m]]==0) h[0]=n;
        h[m] = minh + 1;
        vh[h[m]]++;
    }
    return t-l;
}
void add(int x,int y,long long a,long long b)
{
    idx++;
    e[idx].c=a;
    e[idx].v=y;
    e[idx].next=hd[x];
    e[idx].p = idx+1;
    hd[x]=idx;
    idx++;
    e[idx].c=b;
    e[idx].v=x;
    e[idx].next=hd[y];
    e[idx].p = idx-1;
    hd[y]=idx;
}
int main()
{
    int i,t,x,y,cost[MAXN],w;
    long long sigma,min;
    scanf("%d%d",&n,&m);
    flow = 0;
    
    for(i=0;i<m;i++)
    {
        scanf("%d%d%d",&x,&y,&w);
        add(x,y,w,w);
    }
    
    memset(h,0,sizeof(h));
    memset(vh,0,sizeof(vh));
    vh[0] = n;
    while( h[0]<n )
    {
        flow += aug(0,INF);
    }
    cout<<flow<<endl;

    return 0;
}
