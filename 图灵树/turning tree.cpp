#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
inline void read(int &x){scanf("%d",&x);}
inline void write(int x) {printf("%d",x);}
const int maxn=100000;
const int maxs=1800000;

int num[maxs],val[maxs],ptr,n;
int A[maxn],B[maxn],tree[maxn+maxn];
#define addr(l,r) (((l)+(r))|((l)!=(r)))
bool cmp(int a,int b) {
    return A[a]>A[b];
}
void build(int l,int r) {
	if(l==r) {
		val[ptr]=B[l],num[ptr]=1;
		tree[addr(l,r)]=ptr++;
		return;
	}
	int mid=l+r>>1, cnt=0;
	build(l,mid), build(mid+1,r);
	int li = tree[addr(l,mid)], le = li+mid-l;
	int ri = tree[addr(mid+1,r)], re = ri+r-mid-1;
	tree[addr(l,r)] = ptr;
	while(li<=le || ri<=re) {
		if(ri>re || li<=le && val[li]<= val[ri]) {
			val[ptr]=val[li++];
			num[ptr++]=++cnt;
		}
		else {
			val[ptr] = val[ri++];
			num[ptr++] = cnt;
		}
	}
}
int get(int l,int r,int a,int b,int k){
	if(l==r) return B[l];
	int mid = l+r>>1;
	int cl = (a==0?0:num[tree[addr(l,r)]+a-1]);
	int cr = num[tree[addr(l,r)]+b];
	if(cr-cl >= k)
		return get(l, mid, cl, cr-1, k);
	else
		return get(mid+1, r, a-cl, b+1-cr-1, k-cr+cl);
}
void init(int n) {
	 for(int i=0;i<n;i++) B[i]=i;
	 sort(B,B+n,cmp);
	 ptr=0;
	 build(0,n-1);
}
int main(){
	freopen("in.in","r",stdin);
	int q;
	read(n),read(q);
	for(int i=0;i<n;i++) {
        read(A[i]);
        A[i]=-A[i];
	}
	init(n);
	while(q--){
		int a,b,k;
		read(a),read(b),read(k);
		write(-A[get(0,n-1,a-1,b-1,k)]);
		putchar(10);
  }//while(1);
  return 0;
}
