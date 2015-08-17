#include <iostream>
using namespace std;
template <typename T> 
class SplayTree 
{ 
public: 
    SplayTree(); 
    ~SplayTree(); 
    void Delete(const T &x); 
    void Insert(const T &x); 
    const T &Max() 
    { 
        node *p = root; 
        while (p->rchild != nullnode) 
            p = p->rchild; 
        Splay(root, p->key); 
        return p->key; 
    } 
    const T &Min() 
    { 
        node *p = root; 
        while (p->lchild != nullnode) 
            p = p->lchild; 
        Splay(root, p->key); 
        return p->key; 
    } 
    struct node 
    { 
        T key;
        node *lchild, *rchild; 
        node() {} 
        node(const T &x) : key(x) {} 
    }*nullnode, *root; 
    void LeftRotate(node *&x); 
    void RightRotate(node *&x); 
    void Splay(node *&root, const T &x); 
}; 

template <typename T> 
SplayTree<T>::SplayTree() 
{ 
    nullnode = new node; 
    nullnode->lchild = nullnode; 
    nullnode->rchild = nullnode; 
    root = nullnode; 
} 

template <typename T> 
SplayTree<T>::~SplayTree() 
{ 
    while (root != nullnode) 
        Delete(Max()); 
} 

template <typename T> 
void SplayTree<T>::Delete(const T &x) 
{ 
    node *newroot; 
    Splay(root, x); 
    if (root->key != x) 
        return; 
    if (root->lchild == nullnode) 
        newroot = root->rchild; 
    else 
    { 
        newroot = root->lchild; 
        Splay(newroot, x); 
        newroot->rchild = root->rchild; 
    } 
    delete root; 
    root = newroot; 
}
template <typename T> 
void SplayTree<T>::Insert(const T &x) 
{ 
    node *newnode = new node(x); 
    if (root == nullnode) 
    { 
        newnode->lchild = nullnode; 
        newnode->rchild = nullnode; 
        root = newnode; 
        return; 
    } 
    Splay(root, x); 
    if (x < root->key) 
    { 
        newnode->lchild = root->lchild; 
        root->lchild = nullnode; 
        newnode->rchild = root; 
        root = newnode; 
    } 
    else 
    { 
        newnode->rchild = root->rchild; 
        root->rchild = nullnode; 
        newnode->lchild = root; 
        root = newnode; 
    } 
} 

template <typename T> 
void SplayTree<T>::LeftRotate(node *&x) 
{ 
    node *y = x->rchild; 
    x->rchild = y->lchild; 
    y->lchild = x; 
    x = y; 
} 

template <typename T> 
void SplayTree<T>::RightRotate(node *&x) 
{ 
    node *y = x->lchild; 
    x->lchild = y->rchild; 
    y->rchild = x; 
    x = y; 
} 

template <typename T> 
void SplayTree<T>::Splay(node *&root, const T &x) 
{ 
    node head, *ltree = &head, *rtree = &head; 
    head.lchild = nullnode; 
    head.rchild = nullnode;
	node *last=NULL;
    while (x != root->key) 
    { 
        if (x < root->key||x==root->key) 
        { 
			last=root;
            if (root->lchild != nullnode && x < root->lchild->key) 
                RightRotate(root); 
            if (root->lchild == nullnode) 
                break; 
            rtree->lchild = root; 
            rtree = root; 
            root = root->lchild; 
        } 
        else 
        { 
            if (root->rchild != nullnode && root->rchild->key < x) 
                LeftRotate(root); 
            if (root->rchild == nullnode) 
                break; 
            ltree->rchild = root; 
            ltree = root; 
            root = root->rchild; 
        } 
    } 
//	if (last==NULL) return ;
//	if (last->key!=x)
//	{
//		Splay(root,last->key);
//		return ;
//	}
    ltree->rchild = root->lchild; 
    root->lchild = head.rchild; 
    rtree->lchild = root->rchild; 
    root->rchild = head.lchild; 
} 
/////////////////////////////////////////////////////////////
inline int Min(int a,int b){return a<b?a:b;}
inline int Abs(int a){return a>0?a:-a;}
const int maxn=60000,maxl=999999999;
struct Qujian
{
	int l,r;
} a[maxn];
struct ST
{
	int key;
	int data;
	ST(int a,int b){key=a;data=b;}
	ST(int a){key=a;}
	ST(){}
};
	int operator >(const ST &a,const ST &b){return a.key>b.key;}
	int operator ==(const ST &a,const ST &b){return a.key==b.key;}
	int operator <(const ST &a,const ST &b){return a.key<b.key;}
	int operator !=(const ST &a,const ST &b){return a.key!=b.key;}
SplayTree<ST> tree;
int n,start;
void iin()
{
	int i;
	scanf("%d%d",&n,&start);
	for (i=0;i<n;i++) scanf("%d%d",&a[i].l,&a[i].r);
}
void dp()
{
	int i,j,k;
	ST st(start,0);
	tree.Insert(st);
	for (i=0;i<n;i++)
	{
		int lmin=maxl,rmin=maxl;
		SplayTree<ST>::node *p;
		while (1)
		{
			tree.Splay(tree.root,ST(a[i].l,0));
			p=tree.root;
			if (p==tree.nullnode||p->key>ST(a[i].r,0)) break;
			lmin=Min(lmin,Abs(p->key.key-a[i].l)+p->key.data);
			rmin=Min(rmin,Abs(a[i].r-p->key.key)+p->key.data);
			tree.Delete(p->key);
		}
		tree.Insert(ST(a[i].l,lmin));
		tree.Insert(ST(a[i].r,rmin));
	}
	int res=maxl;
	int tot=0;
	while (tree.root !=tree.nullnode)
	{
		tot++;
		res=Min(res,tree.root->key.data+Abs(tree.root->key.key));
		tree.Delete(tree.root->key);
	}	
	cout<<tot<<endl;
	printf("%d\n",res);
}
int main()
{
//	string s("obstacle.9.in "),s2=s;s2[11]='o';s2[12]='u';s2[13]='t';
	string s="in.in";
	freopen(s.c_str(),"r",stdin);
	iin();
	dp();
//	system(s2.c_str());
	return 0;
}