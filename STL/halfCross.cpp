#include <iostream>
#include <cmath>
using namespace std;
const int maxn=210;
const double pi=3.1415926535897932384626433832795;
struct point//点
{
	double x,y;
};
struct line//半平面直线 表示为a*x+b*y+c>=0
{
	double a,b,c;
	double f;
	double func(point s)
	{
		return a*s.x+b*s.y+c;
	}
};
point acro(line p,line q)//无视平行情况计算两条直线交点
{
	point s;
//	if (p.a*q.b-p.b*q.a==0) return;
	s.y=(p.a*q.c-p.c*q.a)/(q.a*p.b-p.a*q.b);
	s.x=(p.b*q.c-p.c*q.b)/(p.a*q.b-p.b*q.a);
	return s;
}
line que[maxn];
int head,tail;
point a[maxn];
line b[maxn];
int n;
int needtopop(line s,line a,line b)//是否需要退栈
{
	if (s.func(acro(a,b))<0) return 1;
	return 0;
}
int addone(line s)//增加一个点
{
	int hhead=head,ttail=tail;
	while (head<tail&&needtopop(s,que[tail],que[tail-1])) tail--;
	while (head<tail&&needtopop(s,que[head],que[head+1])) head++;
	tail++;
	que[tail]=s;
	return 1;
}
line linemove(line s,double d)//半平面正向平移d
{
	line c=s;
	c.c-=d;
	return c;
}
int comp(const void *a,const void *b)//排序
{
	line *p=(line*)a,*q=(line*)b;
	if (p->f>q->f) return 1;
	if (p->f<q->f) return -1;
	if (p->c>q->c) return 1;
	if (p->c<q->c) return -1;
	return 0;
}
int banpingmianjiao(double d)//主函数
{
	line c[maxn];
	int i,j;
	for (i=0;i<n;i++) c[i]=linemove(b[i],d);//c储存要做的半平面
	qsort(c,n,sizeof(line),comp);//极角排序
	j=0;
	for (i=1;i<n;i++)
		if (c[i-1].f==c[i].f) j++;
		else c[i-j]=c[i];//干掉极角相同的
	c[n-j]=c[0];
	int m=n-j;
	if (m<=1) return 1;
	head=0;tail=1;
	que[0]=c[0];que[1]=c[1];
	
	for (i=2;i<m;i++) 
		if (addone(c[i])==0) //加线进去
			return 0;
	while (head<tail&&needtopop(que[tail],que[head],que[head+1])) head++;//删掉头多余线
	while (head<tail&&needtopop(que[head],que[tail],que[tail-1])) tail--;//删掉尾巴多余线
	if (tail-head<2) return 0;// 必须有3条边 否则挂
	return 1;
}

line poitoline(point p,point q)//将点q-〉p专成半平面 直线和法向量成右手螺旋
{
	line c;
	c.a=q.y-p.y;
	c.b=p.x-q.x;
	c.c=p.y*q.x-p.x*q.y;
	double one=sqrt(c.a*c.a+c.b*c.b);
	c.a/=one;c.b/=one;c.c/=one;
	c.f=atan2(p.y-q.y,p.x-q.x);
	return c;
}
void prework() //预处理将凸包（顺时针）转出半平面来
{
	int i;
	a[n]=a[0];
	for (i=0;i<n;i++)
	//{
		b[i]=poitoline(a[i],a[i+1]);
	//	b[i].f=atan2(a[i].y-a[i+1].y,a[i].x-a[i+1].x);
	//}
}
void work()//二分
{
	double l,r;
	l=0;r=10000000;
	while (l+0.000001<r)
	{
		double mid=(l+r)/2;
		if (banpingmianjiao(mid)) l=mid;else r=mid;
	}
	printf("%.6lf\n",l);
}
int main()
{
	freopen("in.in","r",stdin);
	while (scanf("%d",&n),n)
	{
		int i;
		for (i=n-1;i>=0;i--) scanf("%lf%lf",&a[i].x,&a[i].y);
		prework();
		work();
	}
	return 0;
}
