
#include <iostream>
using namespace std;
const int maxn= 410000,maxm=1100;
void count(char* p,int* f,int m) {
	//模板串p,前缀最长循环匹配输出到f,模板串长度m
	for (f[1]=1;f[1]<m&&p[f[1]]==p[0];f[1]++);
	for (int i = 2, k = 1; i<=m; i++)
		if (f[i-k]+k<f[k]) f[i] = f[i-k]+k;
		else
			for (f[i]=f[k],k=i; f[i]<m && p[f[i]]==p[f[i]%i]; f[i]++);
}
void pipei(char* s,int n,char* p,int m,int* f,int* g)
{//匹配串s,长度n,匹配串p,长度m,循环最长前缀f,输出s-p前缀匹配到g
	for (g[0]=0;g[0]<m && g[0]<n && p[g[0]]==s[g[0]];g[0]++);
	for (int i = 1, k = 0; i<n; i++) {
		g[i] = f[i-k]-(i-k);
		if (g[i]+i>=g[k]+k || i>=g[k]+k) {
			g[i] = g[k]-i+k;
			if (g[i]<0) g[i] = 0;
			while (g[i]+i<n && g[i]<n && s[g[i]+i]==p[g[i]]) g[i]++;
			k=i;
		}
	}
}
