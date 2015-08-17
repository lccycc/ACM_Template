/*
凸包模板
点存在p数组里面
使用凸包算法后，求出的凸包上的点在 p数组 中的 下标 按顺时针储存在 stack数组 里
共有top个，数组均从0开始 
*/ 
#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace std;

#define M 1000
typedef struct
{
	int x,y ;
}point;

int n,top;
int stack[M];
point p[M];

int dblcmp(int d)
{
	if (!d) return 0 ;
	if ( d> 0) return 1 ;else return -1;
}
int cross(point c,point a,point b)
{
	return (a.x-c.x)*(b.y-c.y)-(b.x-c.x)*(a.y-c.y);
}
int cmp(point a,point b)
{
	if (a.y==b.y) return a.x<b.x;
	else return a.y<b.y;
}

void graham()
{
	int i,j,temtop;
	if (n<=1){
		top = 0;
		return;
	}
	sort(p,p+n,cmp);
	top = -1;
	stack[++top] = 0 ; stack[++top] = 1;
	for ( i = 2 ; i< n ; i ++ ){
		while ( top >= 1 && dblcmp(cross(p[stack[top-1]],p[i],p[stack[top]]) ) >= 0 ) top --;
		stack[++top] = i;
	}
	temtop = top ;
	stack[++top] = n - 2;
	for ( i = n - 3 ; i>= 0 ; i -- ){
		while ( top >= temtop +1 && dblcmp(cross(p[stack[top-1]],p[i],p[stack[top]]) ) >= 0 ) top --;
		stack[++top] = i;
	}
	return ;
}
int main()
{
	int t,i;
	n = 0;
	while(cin>>p[n].x>>p[n].y) n++ ;
	graham();
	for(i = 0 ; i < top ; i ++ ) if (p[stack[i]].x==0 && p[stack[i]].y == 0 ) break;
	t = i;
	for ( i = 0 ; i < top ; i ++ ) cout<<"("<<p[stack[(t+i)%top]].x<<","<<p[stack[(t+i)%top]].y<<")"<<endl;
	return 0;
}
