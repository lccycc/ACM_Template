#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>
using namespace std;

const int maxn=502;
///////////////////////////////////
struct BigInt
{
	int s[30];
	int ss;
	BigInt()
	{
		ss=0;
		memset(s,0,sizeof(s));
	}
	BigInt(int k)
	{
		ss=0;
		memset(s,0,sizeof(s));
		while (k) 
			s[ss++]=k%10000000,k/=100000000;
	}
	int& operator [](int k){return s[k];}
//	~BigInt(){delete s;}
};
BigInt operator +(BigInt a,BigInt b)
{
	int i,t=0,code;
	for (i=0;i<a.ss||i<b.ss||t!=0;i++)
	{
		code=a[i]+b[i]+t;
		a[i]=code%100000000;
		t=code/100000000;
	}
	a.ss=i;return a;
}
BigInt operator *(BigInt a,BigInt b)
{
	int i,j,t=0,code;
	BigInt c;
	for (i=0;i<a.ss;i++)
	{
		for (j=t=0;j<b.ss||t>0;j++)
		{
			code=a[i]*b[j]+t+c[i+j];
			c[i+j]=code%100000000;
			t=code/100000000;
		}
		if (c.ss<i+j) c.ss=i+j;
	}
	return c;
}

////////////////////////////////////????????????
const int maxch=60;
struct DFA
{
	struct D_Node
	{
		int term,pre,pt[maxch];
	} node[maxn];
	int nDFA;
	DFA()
	{
		nDFA=1;
		memset(node,0,sizeof(node));
		for (int i=0;i<maxch;i++) node[0].pt[i]=1;
	}
	D_Node& operator [](int k){return node[k];}
};
void addDFA(int* s,int ss,DFA &D)
{
	int t=1;
	for (int i=0;i<ss;i++)
	{
		if (D[t].pt[s[i]]==0)
			D[t].pt[s[i]]=++D.nDFA;
		t=D[t].pt[s[i]];
	}
	D[t].term=1;
}
int bfs[maxn],head,tail;
void makeDFA(DFA &D,int mch)
{
	int i,j,k;
	head=1;tail=2;bfs[0]=0;bfs[1]=1;
	D[1].pre=0;
	while (head<tail)
	{
		int k=bfs[head++];
		for (i=0;i<mch;i++) if (D[k].pt[i]!=0)
		{
			int p=D[k].pt[i];
			for (D[p].pre=D[k].pre;D[D[p].pre].pt[i]==0;D[p].pre=D[D[p].pre].pre);
			D[p].pre=D[D[p].pre].pt[i];
			bfs[tail++]=p;
		}
		else
			D[k].pt[i]=D[D[k].pre].pt[i];
		if (D[D[k].pre].term) D[k].term=1;
	}
}

int s[100];
char ch[100];
char dic[100];

int n,m,p;
BigInt f[maxn],g[maxn];
void work()
{
	int i,j,k;

	DFA D;
	gets(dic);


	while (p--)
	{
		gets(ch);
		k=strlen(ch);
		for (i=0;i<k;i++)
		{
			char* p=strchr(dic,ch[i]);
			if (p==NULL) break;
			s[i]=p-dic;
		}

		if (i>=k&&k>0) addDFA(s,k,D);
	}
	makeDFA(D,m);

	for (i=1;i<=D.nDFA;i++) f[i]=0;
	f[1]=1;
	for (k=0;k<n;k++)
	{
	//	BigInt ret;
	//	for (j=1;j<=D.nDFA;j++) ret=ret+f[j];
	//	if (ret.ss==0) printf("0\n");
	//	else
	//	{
	//		for (i=ret.ss-1;i>=0;i--) printf("%d",ret[i]);
	//		puts("");
	//	}

		for (j=1;j<=D.nDFA;j++) g[j]=0;

		for (i=1;i<=D.nDFA;i++)
			if (D[bfs[i]].term==0)
			{
				int ii=bfs[i];
				for (j=0;j<m;j++) if (D[D[ii].pt[j]].term==0)
				{
					int jj=D[ii].pt[j];
					g[jj]=g[jj]+f[ii];
				}
			}

		for (j=1;j<=D.nDFA;j++) f[j]=g[j];
	}


	
	BigInt tot;
	for (i=1;i<=D.nDFA;i++) tot=tot+f[i];
	if (tot.ss==0) printf("0\n");
	else
	{
		printf("%d",tot[tot.ss-1]);
		for (i=tot.ss-2;i>=0;i--) printf("%08d",tot[i]);
		puts("");
	}
}
int main()
{
	#ifdef _DEBUG
	freopen("in.in", "r", stdin);
	#endif

	while (	scanf("%d%d%d\n",&m,&n,&p)!=EOF)
		work();
	return 0;
}

