

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>
using namespace std;

const int maxn=1502;
typedef long long lld;
///////////////////////////////////
#include <cmath>
const int maxBt=30,maxUt=4,maxMo=pow(10.0,maxUt);
//maxBt是高精度数的长度,maxUt是进制,maxMo是10^maxUt
struct BigInt {
	int s[maxBt];
	int ss;
	BigInt() {
		ss=0;
		memset(s,0,sizeof(s));
	}
	BigInt(int k) { //用整数k初始化
		ss=0;
		memset(s,0,sizeof(s));
		while (k) s[ss++]=k%maxMo,k/=maxMo;
	}
	int& operator [](int k){return s[k];}
	void printBt() { //输出
		if (ss==0) printf("0\n");
		else {
			printf("%d",s[ss-1]);
			for (int i=ss-2;i>=0;i--) printf("%04d",s[i]);
			//注意这里输出的进制格式补0，当进制调整时一起调整
			puts("");
		}
	}
};
BigInt operator + (BigInt a,BigInt b) {
	int i,t=0,code;
	for (i=0;i<a.ss||i<b.ss||t!=0;i++) {
		code=a[i]+b[i]+t;
		a[i]=code%maxMo;
		t=code/maxMo;
	}
	a.ss=i;return a;
}
BigInt operator *(BigInt a,BigInt b) {
	int i,j,t=0,code;
	BigInt c;
	for (i=0;i<a.ss;i++) {
		for (j=t=0;j<b.ss||t>0;j++) {
			code=a[i]*b[j]+t+c[i+j];
			c[i+j]=code%10000;
			t=code/10000;
		}
		if (c.ss<i+j) c.ss=i+j;
	}
	return c;
}



////////////////////////////////////dfa模板
const int maxch=52,maxnode=maxn;//maxch=最大字符集大小,maxnode=最大节点数(无用)
struct DFA {
	DFA* pt[maxch];//指针
	DFA* pre;//前向指针
	int term,id;//是否是终结(这个term是0，表示非终结,否则是终结点的个数,id=这个点的标号
	DFA() {memset(pt,NULL,sizeof(pt)); pre=NULL; term = 0; id = 0;}
};
void addDFA(int* s,int ss,DFA *root,int& nDFA) { //建trie树 nDFA是已有节点数,*s是要插入的串,ss是串长
	for (int i = 0; i<ss; i++) {
		if (root->pt[s[i]] == NULL) {
			root->pt[s[i]] = new DFA;
			root->pt[s[i]]->id = nDFA++;
		}
		root=root->pt[s[i]];
	}
	root->term++;//尾巴的终结个数++
}
DFA* bfs[maxnode];//bfs建立DFA
int head,tail;//bfs的两个指针
void makeDFA(DFA *root,int mch) { //DFA建立前向指针,然后完成所有虚节点的跳转
	DFA* proot=new DFA;//增加一个临时的超父亲,其所有指针指向root
	for (int i = 0; i<mch; i++) proot->pt[i] = root;
	root->pre=proot;
	head=0,tail=1;
	bfs[0]=root;//开始bfs
	while (head<tail) {
		for (int i = 0; i<mch; i++) {
			if (bfs[head]->pt[i]!=NULL) { //扩展bfs
				DFA* p = bfs[head]->pt[i];
				bfs[tail++] = p;//增加新节点
				for (p->pre=bfs[head]->pre;(p->pre)->pt[i]==NULL;p->pre=(p->pre)->pre);//构建该节点的前向指针
				p->pre=(p->pre)->pt[i];
			}
			else {
				bfs[head]->pt[i]=bfs[head]->pre->pt[i];//否则 这个指针是虚指针,完成跳转
			}
		}
		bfs[head]->term+=bfs[head]->pre->term;//注意!终结标记传递
		head++;
	}
}
///////////////////////////////////
int s[100];
char ch[100];
char dic[100];

int n,m,p;
BigInt f[maxn],g[maxn];
void work() {
	int i,j,k;

	DFA *root=new DFA;
	int nDFA=1;
	gets(dic);

	root->id=0;

	while (p--) {
		gets(ch);
		k=strlen(ch);
		for (i=0;i<k;i++) {
			char* p=strchr(dic,ch[i]);
			if (p==NULL) break;
			s[i]=p-dic;
		}

		if (i>=k&&k>0) addDFA(s,k,root,nDFA);
	}
	makeDFA(root,m);
	nDFA=tail;

	for (i=0;i<nDFA;i++) f[i]=0;
	f[0]=1;
	for (k=0;k<n;k++) {
/*		BigInt ret;
		for (j=0;j<nDFA;j++) ret=ret+f[j];
		if (ret.ss==0) printf("0\n");
		else
		{
			for (i=ret.ss-1;i>=0;i--) printf("%d",ret[i]);
			puts("");
		}
*/
		for (j=0;j<nDFA;j++) g[j]=0;

		for (i=0;i<nDFA;i++) {
			if (bfs[i]->term==0&&bfs[i]->pre->term==0)
			for (j=0;j<m;j++) if (bfs[i]->pt[j]->term==0&&bfs[i]->pt[j]->pre->term==0)
				 g[bfs[i]->pt[j]->id]=g[bfs[i]->pt[j]->id]+f[bfs[i]->id];
		}

		for (j=0;j<nDFA;j++) f[j]=g[j];
	}

	BigInt tot;
	for (i=0;i<nDFA;i++) tot=tot+f[i];
	tot.printBt();

}
int main() {
	#ifdef _DEBUG
	freopen("in.in", "r", stdin);
	#endif

	while (	scanf("%d%d%d\n",&m,&n,&p)!=EOF)
		work();
	return 0;
}
