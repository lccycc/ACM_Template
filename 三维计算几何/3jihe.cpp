#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;
const double eps=1e-8;
int sgn(double a){return a>eps?1:(a<-eps?-1:0);}
struct Pt3 {
	double x,y,z;
	Pt3(){x=y=z=0;}
	Pt3(double _x,double _y,double _z){x=_x;y=_y;z=_z;}
	void init(){scanf("%lf%lf%lf",&x,&y,&z);}

};
inline Pt3 operator +(Pt3 a,Pt3 b){a.x+=b.x;a.y+=b.y;a.z+=b.z;return a;}
inline Pt3 operator -(Pt3 a,Pt3 b){a.x-=b.x;a.y-=b.y;a.z-=b.z;return a;}
inline Pt3 operator *(Pt3 a,double d){a.x*=d;a.y*=d;a.z*=d;return a;}
inline Pt3 operator *(double d,Pt3 a){a.x*=d;a.y*=d;a.z*=d;return a;}
inline Pt3 operator /(Pt3 a,double d){a.x/=d;a.y/=d;a.z/=d;return a;}

inline Pt3 mult(Pt3 a,Pt3 b){Pt3 r;r.x=a.y*b.z-a.z*b.y;r.y=a.z*b.x-a.x*b.z;r.z=a.x*b.y-a.y*b.x;return r;}
inline double operator *(Pt3 a,Pt3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}

inline double leng(Pt3 a){return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);}

inline double area(Pt3 a,Pt3 b,Pt3 c){	return leng(mult(a-c,b-c))/2;}

inline double V(Pt3 a,Pt3 b,Pt3 c,Pt3 d){return mult(a-c,b-c)*(d-c)/6.0;}//d 在abc 右手系的时候体积为正

struct Plane {
	double a,b,c,d;
	double one;
	Plane(){a=b=c=d=0;one=1;}
	Plane(Pt3 pa,Pt3 pb,Pt3 pc) {
		a = (pb.y - pa.y) * (pc.z - pa.z) - (pc.y - pa.y) * (pb.z - pa.z);
		b = (pb.z - pa.z) * (pc.x - pa.x) - (pc.z - pa.z) * (pb.x - pa.x);
		c = (pb.x - pa.x) * (pc.y - pa.y) - (pc.x - pa.x) * (pb.y - pa.y);
		d = -(a * pa.x + b * pa.y + c * pa.z);
		one=sqrt(a*a+b*b+c*c);
	}
	double func(Pt3 p){return a*p.x+b*p.y+c*p.z+d;}
	double dist(Pt3 p){return func(p)/one;}
};
void Projection(Pt3 p,Pt3 d,double &x,double &y) {
 //p点在法向量为d的平面上进行投影变换,相当于坐标系转化 未验证慎用
	d = d/leng(d);
	Pt3 u(d.x*d.x-1.0,  d.y*d.x,  d.z*d.x);
	u = u/leng(u);
	Pt3 v = mult(d,u);
	u = u/leng(u);
	x = p*u;
	y = p*v;
}

const int maxpt=501;
struct Convex3 {
	struct Pane {
		int a,b,c;
		int in;
	};

	Pane pane[maxpt*maxpt];
	int npane;
	Pt3 *p;
	int n;

	int edge[maxpt][maxpt];
	double Vol(Pt3 pt,Pane pane) {
		return V(p[pane.a],p[pane.b],p[pane.c],pt);
	}
//	void dfs(int ptx,int ax);
	void deal(int k,int t1,int t2) {
		int t=edge[t1][t2];
		if(pane[t].in) {
			if(Vol(p[k],pane[t])>eps)
				dfs(k,t);
			else {
				Pane add;
				add.a=t2,add.b=t1,add.c=k,add.in=true;
				edge[t2][t1]=edge[t1][k]=edge[k][t2]=npane;
				pane[npane++]=add;
			}
		}

	}
	void dfs(int ptx,int ax) {
		pane[ax].in=0;
		deal(ptx,pane[ax].b,pane[ax].a);
		deal(ptx,pane[ax].c,pane[ax].b);
		deal(ptx,pane[ax].a,pane[ax].c);
	}
	void addpane(int i,int j,int k,int judge) {
		Pane add;
		add.a=i;add.b=j;add.c=k;
		if (Vol(p[judge],add)>eps)
			swap(add.a,add.b);
		edge[add.a][add.b]=edge[add.b][add.c]=edge[add.c][add.a]=npane;
		add.in=1;
		pane[npane++]=add;
	}
	int prework() { //让前4个点不共面
		int i;
		for (i=0;i<n;i++)
			if (sgn(leng(p[i]-p[0]))!=0) break;
		if (i<n) swap(p[1],p[i]);
		else return 0;

		for (i=0;i<n;i++)
			if (sgn(leng(mult(p[1]-p[0],p[2]-p[0])))!=0) break;
		if (i<n) swap(p[2],p[i]);
		else return 0;

		for (i=0;i<n;i++)
			if (sgn(V(p[0],p[1],p[2],p[i]))!=0) break;
		if (i<n) swap(p[3],p[i]);
		else return 0;
	}

	int work(Pt3* _p,int _n) { //传入n个点的数组 调用中p的点可能会被交换
		npane=0;	p=_p;	n=_n;
		if (n==0 || prework()==0) return -1;
		for (int i=0; i<4; i++) {
			addpane(i,(i+1)%4,(i+2)%4,(i+3)%4);
		}
		for (int i = 4; i<n; i++) {
			for (int j=0; j<npane; j++) {
				if (pane[j].in && Vol(p[i],pane[j])>eps) {
					dfs(i,j);
					break;
				}
			}
		}
//把没用的平面压缩掉
    int j = 0;
		for (int i=0; i<npane; i++) {
			if (pane[i].in==0) {
				j++;
			} else {
				pane[i-j] = pane[i];
			}
		}
		npane-=j;
		return npane;
	}

	int same(int i,int j) { //pane[i]和pane[j]是否是同一个平面
		return sgn(V(p[pane[i].a],p[pane[i].b],p[pane[i].c],p[pane[j].a]))==0
			&& sgn(V(p[pane[i].a],p[pane[i].b],p[pane[i].c],p[pane[j].b]))==0
			&& sgn(V(p[pane[i].a],p[pane[i].b],p[pane[i].c],p[pane[j].c]))==0 ;
	}
} C;
Pt3 p[maxpt];
int main() {
#ifdef _DEBUG
//	freopen("in.in","r",stdin);
#endif
	int n;
	scanf("%d",&n);
	int i;
	for (i=0;i<n;i++)
		p[i].init();

	C.work(p,n);
	double tot=0;
	for (i=0;i<C.npane;i++)
		tot+=area(p[C.pane[i].a],p[C.pane[i].b],p[C.pane[i].c]);
	printf("%.3f\n",tot);
	return 0;
}
