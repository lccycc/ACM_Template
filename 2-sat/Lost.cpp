/* This Code is Submitted by zjfc3 for Problem 1917 at 2009-09-02 19:34:16 */
#include <iostream>
#include <stack>
#include <queue>
using namespace std;
 
typedef struct  
{
    int v,next;
}Edge;
 
typedef struct
{
    int Num;
    int Used;
    int Alive;
    int Low;
    int belong;
	int bid;
    inline void init(int pos)
    {
        Num=Used=Alive=Low=0;
        belong=pos;
    }
}Node;
 
const int MAXN=16010;
const int MANM=40010;
 
int N,M;
 
Edge e[MANM];
int p[MAXN];
 
Edge ee[MANM];
int pp[MAXN];
 
int color[MAXN];
 
Node vec[MAXN];
 
stack<int> s;
 
 
int In[MAXN];
int hash[MAXN];
 
int Permit;
int eid,eeid;
int Bcnt;
 
inline void insert(int from,int to)
{
	e[eid].next=p[from];
	e[eid].v=to;
	p[from]=eid++;
}
 
inline void insert2(int from,int to)
{
	ee[eeid].next=pp[from];
	ee[eeid].v=to;
	pp[from]=eeid++;
}
 
inline void init()
{
	int i;
 
    while (!s.empty())
    {
        s.pop();
    }
 
    Bcnt=eid=eeid=Permit=0;
 
	for (i=0;i<=N;++i)
	{
		vec[i].init(i);
	}
 
    memset(p,-1,sizeof(p));
	memset(pp,-1,sizeof(pp));
}
 
inline void update(int &a,int b)
{
    if(a>b) a=b;
}
 
inline void dfs(int pos)
{
    s.push(pos);
    vec[pos].Low=vec[pos].Num=++Permit;
    vec[pos].Used=vec[pos].Alive=true;
 
    int j;
    for (j=p[pos];j!=-1;j=e[j].next)
    {
        int to=e[j].v;
        if (vec[to].Used)
        {
            if (vec[to].Alive)
            {
                update(vec[pos].Low,vec[to].Num);
            }
        }
        else
        {
            dfs(to);
            update(vec[pos].Low,vec[to].Low);
        }
    }
 
    if (vec[pos].Num==vec[pos].Low)
    {
        int t;
        while ((t=s.top())!=pos)
        {
            vec[t].belong=pos;
			vec[t].bid=Bcnt;
            vec[t].Alive=false;
            s.pop();
        }
        vec[pos].belong=pos;
		vec[pos].bid=Bcnt;
        vec[pos].Alive=false;
        s.pop();
 
		hash[Bcnt]=pos;
 
		++Bcnt;
    }
 
}
 
inline void Tarjan()
{
    int i;
    for (i=0;i<N;++i)
    {
        if (!vec[i].Used)
        {
            dfs(i);
        }
    }
}
 
inline bool isAns()
{
	int i;
 
	for (i=0;i<N;i+=2)
	{
		if (vec[i].belong==vec[i+1].belong)
		{
			return false;
		}
	}
	return true;
}
 
inline void Construct()
{
	int i,j;
 
	memset(In,0,sizeof(In));
		
	for (i=0;i<N;++i)
	{
		for (j=p[i];j!=-1;j=e[j].next)
		{
			int from,to;
			from=vec[i].bid;
			to=vec[e[j].v].bid;
			
			if (from==to)
			{
				continue;
			}
				
			++In[from];
			insert2(to,from);
		}
	}
 
}
const int red=1;
const int blue=2;
queue<int> q,qq;
 
inline void toplogic()
{
 
	while(!q.empty()) q.pop();
 
	memset(color,0,sizeof(color));
 
	int i;
	for (i=0;i<Bcnt;++i)
	{
		if (In[i]==0)
		{
			--In[i];
			q.push(i);
		}
	}
 
	while (!q.empty())
	{
		int t=q.front();
		q.pop();
 
		if (color[t])
		{
			continue;
		}
 
		color[t]=red;
 
		int ag=vec[hash[t]^1].bid;
 
		color[ag]=blue;
 
		while (!qq.empty()) q.pop();
 
		qq.push(ag);
 
		int j;
 
		while (!qq.empty())
		{
			int tt=qq.front();
			qq.pop();
 
			for (j=pp[tt];j!=-1;j=ee[j].next)
			{
				if (color[tt]!=blue)
				{
					color[tt]=blue;
					qq.push(tt);
				}
			}
		}
 
		for (j=pp[t];j!=-1;j=ee[j].next)
		{
			if (--In[ee[j].v]==0)
			{
				q.push(ee[j].v);
			}
		}
 
	}
 
}
 
inline void print()
{
	int i;
	for (i=0;i<N;++i)
	{
		if (color[vec[i].bid]==red)
		{
			printf("%d\n",i+1);
		}
	}
}
 
int main()
{
	while(scanf("%d%d",&N,&M)!=EOF)
	{
		N=N*2;
		
		init();
 
		//以下根据输入安排
		while (M--)
		{
 
			int ff,tt;
 
			scanf("%d%d",&ff,&tt);
			
			--ff,--tt;
 
			insert(ff,tt^1);
			insert(tt,ff^1);
 
		}
 
		Tarjan();
 
		if (isAns())//有解
		{
		//	puts("YES");
 
			//构新图
			Construct();
			//逆拓扑
			toplogic();
			//输出
			print();
 
		}
		else
		{
			puts("NIE");
		}
		
	}
	return 0;
}