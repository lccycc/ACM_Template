/*
邻接表模板
n个点 m条边 源点为0 汇点为n-1
加边函数adddege(x,y,c) 添加一条从x到y的容量为c的边(同时添加了容量为0的反向边)
*/
#include <iostream>
#define MAXN 5000 + 10
#define MAXM 500000 + 10
#define INF (long long)1<<60
using namespace std;

struct edge
{
    int next,pair,v;
    long long c;
};
edge e[MAXM*2];
int h[MAXN],vh[MAXN],n,m,hd[MAXN],idx=0;
long long augc,flow;
bool found;

void aug(const int m)
{
    int i,minh;
    long long augco;
    augco = augc;
    minh = n - 1;
    if(m== n -1)
    {
        flow += augc;
        found = 1;
        return ;
    }
    for(i=hd[m];i!=0;i=e[i].next) if(e[i].c>0){
        if(h[e[i].v]+1==h[m]){
            if(e[i].c<augc) augc = e[i].c;
            aug(e[i].v);
            if(h[0]>=n) return;
            if(found) break;
            augc = augco;
        }
        if(minh>h[e[i].v]) minh=h[e[i].v];
    }
    if(!found)
    {
        vh[h[m]]--;
        if(vh[h[m]]==0) h[0]=n;
        h[m] = minh + 1;
        vh[h[m]]++;
    }
    else
    {
        e[i].c -=augc;
        e[e[i].pair].c += augc;
    }
}

void add(int x,int y,long long c)
{
    idx++;
    e[idx].c=c;
    e[idx].v=y;
    e[idx].next=hd[x];
    e[idx].pair = idx+1;
    hd[x]=idx;
    idx++;
    e[idx].c=0;
    e[idx].v=x;
    e[idx].next=hd[y];
    e[idx].pair = idx-1;
    hd[y]=idx;
}

int main()
{
    int i,t,x,y,cost[MAXN];
    
    cin>>n>>m;//build map
    
    for(i=0;i<m;i++)
    {
        cin>>x>>y;
        add(x,y,INF);
    }
    
    memset(h,0,sizeof(h));
    memset(vh,0,sizeof(vh));
    vh[0] = n;
    while( h[0]<n )
    {
        augc = INF;
        found = 0;
        aug(0);
    }

    return 0;
}
