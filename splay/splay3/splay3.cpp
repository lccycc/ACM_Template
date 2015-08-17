/*POJ2761*/
#include <cstdio>
#include <algorithm>
#define MAX 100001
#define MAXQ 50001
#define size(x) (x?x->s:0)

using namespace std;
struct Query{
    int s,t,k,id;
}query[MAXQ];
struct Splay{
    int s,p;
    Splay *ls,*rs;
}tree[MAX],*root,*cur;
int a[MAX],r[MAXQ];
bool cmp(const Query &i,const Query &j){return i.s < j.s;}
void output(Splay *t,int d) {
    if (t == NULL) return;
    output(t->rs,d+1);
    for (int i=0; i< d; i++) printf("  ");
    printf("%d(%d)\n", t->p, t->s);
    output(t->ls,d+1);
}
Splay *newNode(int p){
    cur->s = 1,cur->p = p;
    cur->ls = cur->rs = NULL;
    return cur++;
}
Splay *splay(Splay *x,int p){
    if (root == NULL) return root;
    Splay N,*l,*r,*y;
    int lsize,rsize;
    N.ls = N.rs = NULL;
    l = r = &N;
    lsize = rsize = 0;
    while(1){
        if (p < x->p){
            if (x->ls == NULL) break;
            if (p < x->ls->p){/*zig*/
                y = x->ls,x->ls = y->rs,y->rs = x;
                x->s = size(x->ls)+1+size(x->rs);
                x = y;
                if (x->ls == NULL) break;
            }
            r->ls = x,r = x;
            x = x->ls;
            rsize += 1+size(r->rs);
        }else if (p > x->p){
            if (x->rs == NULL) break;
            if (p > x->rs->p){/*zag*/
                y = x->rs,x->rs = y->ls,y->ls = x;
                x->s = size(x->ls)+1+size(x->rs);
                x = y;
                if (x->rs == NULL) break;
            }
            l->rs = x,l = x;
            x = x->rs;
            lsize += size(l->ls)+1;
        }else
            break;
    }
    lsize += size(x->ls),rsize += size(x->rs);/*maintain the size*/
    x->s = lsize+1+rsize;
    l->rs = r->ls = NULL;
    for (y = N.ls;y != NULL;y = y->ls)
        y->s = rsize,rsize -= 1+size(y->rs);
    for (y = N.rs;y != NULL;y = y->rs)
        y->s = lsize,lsize -= 1+size(y->ls);
    l->rs = x->ls,r->ls = x->rs;/*assemble*/
    x->ls = N.rs,x->rs = N.ls;
    return x;
}
Splay *ins(Splay *x,int p){
    if (x == NULL){x = newNode(p);return x;}
    x = splay(x,p);
//    if (p == x->p) return x;
    Splay *y = newNode(p);
    if (p < x->p){
        y->ls = x->ls,y->rs = x,x->ls = NULL;
        x->s = 1+size(x->rs);
    }else{
        y->rs = x->rs,y->ls = x,x->rs = NULL;
        x->s = size(x->ls)+1;
    }
    x = y;
    x->s = size(x->ls)+1+size(x->rs);
    return x;
}
Splay *del(Splay *x,int p){
    if (x == NULL) return NULL;
    x = splay(x,p);
    if (p == x->p){
        Splay *y = x;
        if (x->ls == NULL) x = x->rs;
        else{
            x = splay(x->ls,p);
            x->rs = y->rs;
        }
        if (x != NULL) x->s = y->s-1;
    }
    return x;
}
int findK(Splay *x,int k){
    if (k <= 0 ||  k > size(x)) return -1;
    int d;
    while (1){
        d = size(x->ls)+1;
        if (k < d) x = x->ls;
        else if (k > d) x = x->rs,k -= d;
        else return x->p;
    }
}
int main(){
    int n,m,i,j,s,t;
    root = NULL,cur = &tree[0];
    scanf("%d%d",&n,&m);
    for (i = 1;i <= n;i++)
        scanf("%d",&a[i]);
    for (i = 0;i < m;i++){
        scanf("%d%d%d",&query[i].s,&query[i].t,&query[i].k);
        query[i].id = i;
    }
    sort(query,query+m,cmp);
    s = 1,t = 0;
    for (i = 0;i < m;i++){
        Query &q = query[i];
        for (j = s;j <= t && j < q.s;j++)
            root = del(root,a[j]);
        for (j = t < q.s?q.s:(t+1);j <= q.t;j++)
            root = ins(root,a[j]);
        r[q.id] = findK(root,q.k);
        s = q.s,t = q.t;
    }
    for (i = 0;i < m;i++)
        printf("%d\n",r[i]);
}