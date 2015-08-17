#include <iostream>
#include <cstdio>
#include <time.h>
#include <cstdlib>
#include <cmath>
using namespace std;
typedef unsigned long long ulld;
ulld chen(ulld x,ulld y,ulld n) { //计算x*y%n
	if ((x>>32)==0&&(y>>32)==0) return x*y%n;
	ulld a1,b1,a2,b2;
	a1=(x>>32);
	b1=((x<<32)>>32);
	a2=(y>>32);
	b2=((y<<32)>>32);
	ulld t1,t2,t3,t4;
	t1=b1*b2%n;t2=b1*a2%n;t3=a1*b2%n;t4=a1*a2%n;
	int i;
	for (i=0;i<32;i++){ t2=(t2<<1)%n;t3=(t3<<1)%n;}
	for (i=0;i<64;i++) t4=(t4<<1)%n;
	return ((t1+t2)%n+(t3+t4)%n)%n;
}
ulld mood(ulld a,ulld u,ulld n) { //计算a^u%n
	ulld b=1;
	while (u) {
		if (u & 1) b =chen(b,a,n);
		if (u >>= 1) a=chen(a,a,n);
	}
	return b;
}
ulld witness(ulld a, ulld n) { //计算a^(n-1)%n 其中如果遇到1的非平凡平方根 就直接返回合数
	ulld i,x,y,t,u;
	u=n-1;t=0;
	while ((u&1)==0) {
	   u>>=1;
	   t++;
	}
	x=mood(a,u,n);
	for (i=0;i<t;i++) {
	   y=chen(x,x,n);
	   if (y==1&&x!=1&&x!=n-1)
		return x;
	   x=y;
	}
	return x;
}
ulld miller(ulld n) { //miller判素数部分 多测几次
	if (n==2) return 1;
	if (n<2 || n%2==0) return 0;
	for (int i=0; i<50; i++) {
		ulld j = rand()%(n-2)+1;
		ulld k = witness(j,n);
		if (k != 1) return k;
	}
	return 1;
}
ulld gcd(ulld a,ulld b) { //最大公约数
	while (b!=0) {
	   ulld c=a;a=b;b=c%b;
	}
	return a;
}
ulld pollard(ulld n,ulld x) { //pollard tot用于卡时防止死掉
	ulld i = 1,k = 2,y = x;
	ulld tot=1<<20;
	while (1) {
		i++;
		tot--;
		x=(chen(x,x,n)-1+n)%n;
		ulld d=gcd((y-x+n)%n,n);
		if (d!=1&&d!=n) return d;
		if (i==k) {
			y=x;
			k=(k<<1);
		}
		if (tot==0) return 1;
	}
}
int main()
{
	ulld n,m;
	int cass;
	srand(time(0));
	for (cin>>cass;cass>0;cass--) {
		scanf("%I64d",&n);
		if (n==2)
		goto isprim;
		if ((n&1)==0) {
			cout<<2<<endl;
			continue;
		}
		if (miller(n)==1) {
			isprim:    cout<<"Prime"<<endl;
			continue;
		}
		ulld d=pollard(n,2);
		n/=d;
		ulld mind=d;
		if (mind==1) mind=99999999999999;
		while (miller(n)!=1) {
			m=rand()%(n-1)+1;
			d=pollard(n,m);
			if (d>1&&mind>d) mind=d;
			n/=d;
		}
		if (mind>n) mind=n;
		printf("%I64d\n",mind);
	}
	return 0;
}

