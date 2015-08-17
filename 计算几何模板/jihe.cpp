#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
using namespace std;
const double eps=1e-10;//精度控制
const double maxl=1e10;//全局最大值
const double pi=3.1415926535897932384626433832795;
const int maxv=10100;//多边形顶点最大值
template <class T>
T Max(T a,T b){return a>b?a:b;}
template <class T>
T Min(T a,T b){return a<b?a:b;}
double sqr(double a){return a*a;}
int sgn(double a){if (a>eps) return 1;if (a<-eps) return -1;return 0;}

struct Point {
	double x,y;
	Point(){};
	Point(double dx,double dy){x=dx;y=dy;}
	inline int init(){ return scanf("%lf%lf",&x,&y); }
};
inline double dis(Point a,Point b){return sqrt(sqr(a.x-b.x)+sqr(a.y-b.y));}
inline Point operator -(Point a,Point b){Point c;c.x=a.x-b.x;c.y=a.y-b.y;return c;}
inline Point operator +(Point a,Point b){Point c;c.x=a.x+b.x;c.y=a.y+b.y;return c;}
inline Point operator *(Point a,double d){Point c;c.x=a.x*d;c.y=a.y*d;return c;}
inline Point operator *(double d,Point a){Point c;c.x=a.x*d;c.y=a.y*d;return c;}
inline Point operator /(Point a,double d){Point c;c.x=a.x/d;c.y=a.y/d;return c;}
inline double operator *(Point a,Point b){return a.x*b.x+a.y*b.y;}
inline int operator ==(Point a,Point b){return dis(a,b)<eps;}
inline int operator !=(Point a,Point b){return dis(a,b)>=eps;}
inline bool operator < (const Point &l, const Point &r){return sgn(l.y-r.y)<0 ||( sgn(l.y-r.y)==0 && sgn(l.x-r.x)<0 );}
inline double crossmuti(Point a,Point b){return a.x*b.y-a.y*b.x;}
inline bool crossmuti(Point sp, Point ep, Point op) {
	return sgn((sp.x - op.x) * (ep.y - op.y)
	-(ep.x - op.x) * (sp.y - op.y))>=0;
}
struct Polygon {
	Point vertex[maxv];
	int numv;
	Polygon(){numv=0;}
	Polygon(Point *s,int num){numv=num;memcpy(vertex,s,num*sizeof(Point));}
	Point &operator [](int k){return vertex[k];}
};
int findtubao(Point* pnt, int n, Polygon &res) { //凸包部分 返回凸包顶点个数
	int top = 1;
	sort(pnt, pnt + n);
	res.numv = n;//防止后面三行return的时候没赋顶点数
	for (int i = 0; i<3; i++) res[i]=pnt[i];
	if (n<3) return n;
	for (int i = 2; i < n; i++) {
		while (top && crossmuti(pnt[i], res[top], res[top-1]))
		top--;
		res[++top] = pnt[i];
	}
	int len = top; res[++top] = pnt[n - 2];
	for (int i = n - 3; i >= 0; i--) {
		while (top!=len && crossmuti(pnt[i], res[top], res[top-1])) top--;
			res[++top] = pnt[i];
	}
	res.numv=top;
	return top; // 返回凸包中点的个数
}
int compagu(const void *e,const void *b) { //极角排序 要求极点必须是(0,0) 没用函数
	Point* p=(Point*) e,*q=(Point*) b;
	if (*p==*q) return 0;
	if (sgn(atan2(p->y,p->x)-atan2(q->y,q->x))!=0)
		return sgn(atan2(p->y,p->x)-atan2(q->y,q->x));
	else return sgn(dis(*p,Point(0,0))-dis(*q,Point(0,0)));
}

struct LineSegment {
	Point pt1,pt2;
	LineSegment(){};
	LineSegment(Point p1,Point p2){pt1=p1;pt2=p2;}
};
struct Line {
	double a,b,c;		//ax+by+c==0
	double f;    //正向法向量atan2值 注意到ax+by+c>=0的正向法向量恰好是(a,b)
	Line(){a=b=c=0;};
	Line(double p,double q,double r) {
		a=p;b=q;c=r;
		f=atan2(b,a);
		double one=sqrt(a*a+b*b);
		a/=one;b/=one;c/=one;
	}
	Line(Point p,Point q)	{ //将点q-〉p转成半平面 直线和半平面正向法向量成右手螺旋
		a=q.y-p.y;
		b=p.x-q.x;
		c=p.y*q.x-p.x*q.y;
		f=atan2(b,a);
		double one=sqrt(a*a+b*b);
		a/=one;b/=one;c/=one;
	}
	Line(LineSegment l) {
		Line ls(l.pt1,l.pt2);
		a=ls.a;b=ls.b;c=ls.c;
		f=atan2(b,a);
		double one=sqrt(a*a+b*b);
		a/=one;b/=one;c/=one;
	}
	double func(const Point &p){return a*p.x+b*p.y+c;}
};

double countarea(Polygon &p)	{ //计算面积
	double tot=crossmuti(p[p.numv-1],p[0]);
	for (int i = 0; i+1<p.numv; i++) {
		tot+=crossmuti(p[i], p[i+1]);
	}
	return fabs(tot/2.0);
}
double disl(Line l,Point p){ //点到直线距离
	return fabs(l.a*p.x+l.b*p.y+l.c)/sqrt(sqr(l.a)+sqr(l.b));
}
double disls(LineSegment b,Point a) { //点到线段距离
	if (sgn((a-b.pt1)*(b.pt2-b.pt1))>=0&&sgn((a-b.pt2)*(b.pt1-b.pt2))>=0)
		return disl(b,a);
	else return Min(dis(a,b.pt1),dis(a,b.pt2));
}
int ondiffside(Line l,Point a,Point b) {//判断ab是否在l两侧
	return sgn(l.func(a))*sgn(l.func(b))<0;
}
int PntonSeg(LineSegment ls,Point p) {//判断点是否在线段上
	Line l(ls.pt1,ls.pt2);
	if (ls.pt1==p||ls.pt2==p) return 1;
	if (sgn(l.func(p))==0&&((ls.pt1<p&&p<ls.pt2)||(ls.pt2<p&&p<ls.pt1))) return 1;
	return 0;
}
int Intersect(LineSegment L1, LineSegment L2) {
  //判断两条线段是否相交 若规范相交 返回1，不规范,返回-1，不相交返回0
	if (ondiffside(L1,L2.pt1,L2.pt2)&&ondiffside(L2,L1.pt1,L1.pt2))
		return 1;
	if (PntonSeg(L1,L2.pt1)||PntonSeg(L1,L2.pt2)||PntonSeg(L2,L1.pt1)||PntonSeg(L2,L1.pt2))
		return -1;
	return 0;
}
inline int intersection(const Line &p,const Line &q,Point &s) {
  //求两直线的交点存入s 返回是否成功
	if (sgn(p.a*q.b-p.b*q.a)==0) return 0;
	s.y=(p.a*q.c-p.c*q.a)/(q.a*p.b-p.a*q.b);
	s.x=(p.b*q.c-p.c*q.b)/(p.a*q.b-p.b*q.a);
	return 1;
}

Point projection(Point p,const Line &l) {
	if (sgn(disl(l,p))==0) return p;
	Line r;
	r.a=l.b;r.b=-l.a;r.c=-r.func(p);
	intersection(l,r,p);
	return p;
}
int PntinPol(Point a,Polygon &p) {
	int tot=0;
	if (p.numv==1) return a==p[0];
	p[p.numv]=p[0];
	LineSegment l = LineSegment(a, Point(maxl,a.y));
	for (int i = 0; i<p.numv; i++)
	{
		if (PntonSeg(LineSegment(p[i],p[i+1]),a)) return 1;
		double y1 = Max(p[i].y, p[i+1].y), y2 = Min(p[i].y, p[i+1].y);
		if (sgn(y1-y2)==0) continue;
		if (sgn(a.y-y1)<=0&&sgn(a.y-y2)>0&&Intersect(LineSegment(p[i],p[i+1]),l)) tot++;
	}
	return tot%2;
}
struct Circle {
	Point c;
	double r;
	Circle(){}
	Circle(Point _h,double _r){c=_h; r=_r;}
};
void intersection_line_circle(const Circle &o, const Point &l1, const Point &l2,
															Point &p1, Point &p2) {
    Point p = o.c;
    p.x += l1.y - l2.y;
    p.y += l2.x - l1.x;
    intersection(Line(p, o.c), Line(l1, l2),p);
    double t = sqrt(o.r * o.r - dis(p, o.c) * dis(p, o.c)) / dis(l1, l2);
    p1 = p + (l2 - l1) * t;
    p2 = p - (l2 - l1) * t;
}

void intersection_circle_circle(const Circle &c1,const Circle &c2,Point& p1,Point& p2) {
    Point u, v;
    double t = (1+(sqr(c1.r)-sqr(c2.r))/dis(c1.c,c2.c)/dis(c1.c,c2.c))/2;
	u=c1.c+(c2.c-c1.c)*t;
    v.x = u.x+c1.c.y-c2.c.y;
    v.y = u.y-c1.c.x+c2.c.x;
    intersection_line_circle(c1,u,v,p1,p2);
}

double area_circle_halfsegment(const Circle &c,const Point &h,const Point &p) {
	double d1=dis(c.c,h),d2=dis(c.c,p),l1;
	if (sgn(dis(p,c.c)-c.r)<=0) return d1*dis(h,p)/2;
	if (sgn(d1-c.r)>=0) l1=0;else
	l1=sqrt(sqr(c.r)-sqr(d1));
	return d1*l1/2+(acos(d1/d2)-asin(l1/c.r))*sqr(c.r)/2;
}
double area_circle_segment(const Circle &c,const Point &p1,const Point &p2) {
	if (p1==p2) return 0;
	Point h=projection(c.c,Line(p1,p2));
	int sg=(sgn((p1-h)*(h-p2))>=0?1:-1);
	return fabs(area_circle_halfsegment(c,h,p1)*sg+area_circle_halfsegment(c,h,p2));
}
double area_circle_Polygon(Circle c,Polygon &p) {
	double tot=0;
	p[p.numv]=p[0];
	for (int i=0;i<p.numv;i++)
		tot+=area_circle_segment(c,p[i],p[i+1])*sgn(crossmuti(p[i]-c.c,p[i+1]-c.c));
	return fabs(tot);
}
double countangular(Point a,Point b,Point c) {
  //计算向量a->b和a->c的夹角,返回pi进制
	double l1,l2,l3;
	l1=dis(a,b);l2=dis(a,c);l3=dis(b,c);
	if (sgn(l1-l2)==0&&sgn(l3)==0) return 0;
	if (sgn(l1+l2-l3)==0) return pi;
	return acos((sqr(l1)+sqr(l2)-sqr(l3))/2.0/l1/l2);
}
Point circumcenter(Point A,Point B,Point C) { //外心
	Point a=C-B,b=A-C,c=B-A;
	double af,bf,cf,s;
	s=2*sqr(fabs(crossmuti(c,a)));
	af=-(a*a)*(c*b)/s;
	bf=-(b*b)*(c*a)/s;
	cf=-(c*c)*(b*a)/s;
	Point res=af*A+bf*B+cf*C;
	return res;
}
Point orthocenter(Point A,Point B,Point C) { //垂心
	Point a=C-B,b=A-C,c=B-A;
	double af,bf,cf;
	af=(a*b)*(a*c);
	bf=(b*c)*(b*a);
	cf=(c*a)*(c*b);
	Point res=(af*A+bf*B+cf*C)/(af+bf+cf);
	return res;
}
Point incenter(Point A,Point B,Point C) { //内心
	double a=dis(B,C),b=dis(C,A),c=dis(A,B);
	return (a*A+b*B+c*C)/(a+b+c);
}

int RotatingCalipers(Polygon &p1,int k1,Polygon &p2,int k2,Line &l) { //逆时针转 返回贴着的那条线 如果第一条贴 返回1 如果第二条 返回2 如果两条都贴 返回3
																//注意 当求两个凸多边形的最近距离时 小心相交情况判多边形交 如果两个多边形套一起 起始射线方向同向
	int k11=(k1+1)%p1.numv,k22=(k2+1)%p2.numv;
	Line l1(p1[k1],p1[k11]),l2(p2[k2],p2[k22]);
	double aug1=atan2(l1.a,-l1.b)-atan2(l.a,-l.b),aug2=atan2(l2.a,-l2.b)-atan2(-l.a,l.b);
	if (aug1<0) aug1+=2*pi;
	if (aug2<0) aug2+=2*pi;
	if (sgn(aug1-aug2)==0) {k1=k11;k2=k22;l=l1;return 3;}
	if (aug1<aug2){l=l1;return 1;}
	if (aug1>aug2){l=l2;return 2;}
}
//////////////////以下为半平面交部分
Line linemove(Line s,double d) { //半平面正向平移d
	s.c-=d*sqrt(sqr(s.a)+sqr(s.b));
	return s;
}
int needtopop(Line s,Line a,Line b) { //是否需要退栈 看a,b的交点是否在s外
	Point cro;
	intersection(a,b,cro);
	return sgn(s.func(cro))<=0;
}
int compf(const void *a,const void *b) { //正法向排序 如果相同 按c排序
	return sgn(((Line*)a)->f-((Line*)b)->f)*10+sgn(((Line*)a)->c-((Line*)b)->c);
}
int banpingmianjiao(Line* c,int n,Line* que,int& head,int& tail) {
  //半平面交 注意传入的c必须有外围边框保证封闭 否则手动加 最后返回在head和tail之间那段
	int i,j;
	qsort(c,n,sizeof(Line),compf);//极角排序
	for (i=1,j=0;i<n;i++)
	   if (sgn(c[i-1].f-c[i].f)==0) j++;
	   else c[i-j]=c[i];//干掉极角相同的
	n-=j;
	head=0;tail=1;
	que[0]=c[0];que[1]=c[1];
	for (i=2;i<n;i++) { //加线进去
		while (head<tail&&needtopop(c[i],que[tail],que[tail-1])) tail--;//注意这两句不能写反!
		while (head<tail&&needtopop(c[i],que[head],que[head+1])) head++;//注意这两句不能写反!
		que[++tail]=c[i];
	}
	while (head<tail&&needtopop(que[head],que[tail],que[tail-1])) tail--;//删掉尾部多余线
	while (head<tail&&needtopop(que[tail],que[head],que[head+1])) head++;//删掉头部多余线
	return tail-head>=2;// 必须有3条边 否则挂
}
//////////////////////////////////////////////////////////////////////////
//旋转卡壳样例:求俩凸包距离
/*
int n,m;
Polygon p1,p2,pp1,pp2;
void work() {
	int i,j,k1,k2;
	double mint;
	Line l(Point(0,0),Point(1,0));
	for (i=0;i<n;i++) p1.vertex[i].init();
	for (i=0;i<m;i++) p2.vertex[i].init();
	p1.numv=n;p2.numv=m;
	pp1=p1;pp2=p2;
	findtubao(pp1.vertex,pp1.numv,p1);
	findtubao(pp2.vertex,pp2.numv,p2);
	n=p1.numv;m=p2.numv;
	k1=k2=0;
	for (i=0;i<n;i++) if (sgn(p1.vertex[i].y-p1.vertex[k1].y)<0) k1=i;
	for (i=0;i<m;i++) if (sgn(p2.vertex[i].y-p2.vertex[k2].y)>0) k2=i;
	mint=dis(p1.vertex[k1],p2.vertex[k2]);
	int tot1=0,tot2=0;
	do {
		int k=RotatingCalipers(p1,k1,p2,k2,l);
		int kp1=(k1+1)%n,kp2=(k2+1)%m;
		mint=Min(mint,disls(LineSegment(p1.vertex[k1],p1.vertex[kp1]),p2.vertex[k2]));
		mint=Min(mint,disls(LineSegment(p2.vertex[k2],p2.vertex[kp2]),p1.vertex[k1]));
		mint=Min(mint,disls(LineSegment(p1.vertex[k1],p1.vertex[kp1]),p2.vertex[kp2]));
		mint=Min(mint,disls(LineSegment(p2.vertex[k2],p2.vertex[kp2]),p1.vertex[kp1]));
		if (k&1) {k1=kp1;tot1++;}
		if (k&2) {k2=kp2;tot2++;}
	} while (tot1<n||tot2<m);//这里是或!!!!!以前的代码错了
	printf("%.5lf\n",mint);
}
*/
char ot[100];
char* o;
void getf(double &a) {
	int f=1,dot=0;
	if (*o=='-') {f=-1;o++;}
	for (a=0;*o&&*o!=' ';o++)
		if (*o=='.') dot++;
		else {
			if (dot) dot++;
			a=a*10+*o-48;
		}
	o++;
	if (dot)
		while (--dot) a/=10;
	a*=f;
}
