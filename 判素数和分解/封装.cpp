/*==================================================*\
* SQUFOF因子分解,10^18以内因子分解的最快算法
* 用法：
* 1、所有case之前调用一次init()
* 2、连续使用analyze(合数N)，每次不需重新初始化
* 返回：合数N的一个因子
* 注意：
* 1、N不能太大，最好<2^54,以防溢出
* 2、TABLE_SIZE的值不能更改
* 3、U32不能用int代替
* 4、不会因为N有小因子而更快，请先试除小因子
\*==================================================*/
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;
typedef unsigned long long U64;
typedef unsigned int U32;
const U32 MAX_INDEEP = 10000;
const U32 TABLE_SIZE = 131071;
U32 sqrt_table[TABLE_SIZE] = {0};
U64 gcd(U64 a,U64 b) {
	if (b==0) return a;
	return gcd(b,a%b);
}
class squfof {
	U32 try_ana(U64 N) {
		U32 sqrt_n = (U32) sqrt((long double) (signed long long )N);
		U32 P1 = sqrt_n, Q2 = 1, Q1 = N - (U64) P1*P1;
		U32 B, P, Q, step = MAX_INDEEP;
		if (Q1 == 0)return P1;
		while (sqrt_table[Q1 % TABLE_SIZE] != Q1) {
			B = (sqrt_n + P1) / Q1;
			P = B * Q1 - P1;
			Q = Q2 + B * (P1 - P);
			P1 = P;
			Q2 = Q1;
			Q1 = Q;
		}
		U32 sqrt_Qi = (U32) sqrt((long double) Q1);
		B = (sqrt_n - P1) / sqrt_Qi;
		P1 = B * sqrt_Qi + P;
		Q2 = sqrt_Qi;
		Q1 = (N - (U64) P1 * P1) / Q2;
		P = P1;
		P1 = 0;
		while (P != P1 && step--) {
			P1 = P;
			B = (sqrt_n + P1) / Q1;
			P = B * Q1 - P1;
			Q = Q2 + B * (P1 - P);
			Q2 = Q1;
			Q1 = Q;
		}
		return P;
	}
public:
	void init() {
		for (U32 i = 0; i < (1 << 16); i++)
			sqrt_table[i * i % TABLE_SIZE] = i * i;
	}
	U32 analyze(U64 N) {
		U32 k, t = 0;
		if (N==1) return 1;
			for (k = 1; t == 0 || t == 1; k++) {
				t = gcd(try_ana(k * N), N);
			}
			return t;
	}
} a;
int main()
{
	a.init();
	U64 n=875623904451;
	while (n>1)
	{
		printf("n=%I64d\n",n);
		U64 d=a.analyze(n);
		printf("d=%I64d\n",d);
		n/=d;
	}
	return 0;
}
