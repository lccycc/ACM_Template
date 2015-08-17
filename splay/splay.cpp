#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
typedef long long lld;
class Splaytree {
public:
	struct Node {
		lld key;
		int data;
		Node *left,*right,*father;
		Node(Node* fa){key = 0; father = fa; left = right = NULL;}
		Node(){key = 0; father = left = right = NULL;}
	};
	Node* root;
	Splaytree(){root = NULL;}	//注意以下&root必须全部是实树指针 不能是临时指针
	void splay(Node* &root,Node* x);//将x提到根
	Node* splayfind(lld keyf);//查找第一个key不小于keyf的节点
	void splayzig(Node* &root,Node* x);//右旋
	void splayzag(Node* &root,Node* x);//左旋
	void splaydelete(Node* &root,Node* x);//将x转到root 干掉
	Node* splayMax(Node* &root);//将最大的转到根
	Node* splayinsert(lld key);//插入key 返回那个点 已经存在就直接返回
};
	void Splaytree::splayzig(Node* &root,Node* x) { //对x做左旋 将x旋下去 x的左儿子旋上来
		if (x == NULL) return ;
		Node* fa = x->father;//保存x的父亲
		Node* y = x->left;	//x的左儿子
		if (y == NULL) return ;//如果y是空 没法旋
		x->left = y->right;	//y的右给x的左
		if (y->right != NULL) y->right->father = x;//修改y右的父亲
		y->right = x;	//x接到y右
		x->father = y;//修改x父亲
		if (x == root) root = y; else//判断根是否改变
		if (fa->left == x) fa->left = y; else fa->right = y;//修改父亲指针
		y->father = fa;//修改y父亲
	}
	void Splaytree::splayzag(Node* &root,Node* x) {
		if (x == NULL) return;
		Node* fa = x->father;
		Node* y = x->right;
		if (y == NULL) return ;
		x->right = y->left;
		if (y->left != NULL) y->left->father = x;
		y->left = x;
		x->father = y;
		if (x == root) root = y; else
		if (fa->left == x) fa->left = y; else fa->right = y;
		y->father = fa;
	}
	void Splaytree::splay(Node* &root,Node* x) { //将x旋到根
		if (x == NULL) return ;
		while (1) {
			if (x == root) return ;//如果x就是根 停了
			if (x->father == root) {	//如果x的父亲是根
				if (root->left == x) splayzig(root,root);
				else splayzag(root,root);
				return ;
			}
			Node *fa = x->father;//父亲
			Node* fa2 = fa->father;//父亲的父亲
			if (fa2->left == fa&&fa->left == x) { //一条线 先转fa2
				splayzig(root,fa2);
				splayzig(root,fa);
			}
			else if (fa2->right == fa&&fa->right == x) {
				splayzag(root,fa2);
				splayzag(root,fa);
			}
			else if (fa2->left == fa&&fa->right == x) { //叉 先转fa
				splayzag(root,fa);
				splayzig(root,fa2);
			}
			else {
				splayzig(root,fa);
				splayzag(root,fa2);
			}
		}
	}
	Splaytree::Node* Splaytree::splayfind(lld keyf) { //查找不小于keyf的第一个
		Node *x = root,*last = NULL;//保存最靠近x且不小于x的
		while (x != NULL) {
			if (keyf == x->key) { last = x; break;}//刚好找到
			if (keyf<x->key) {	//如果在左子树 那么x可能成为结果 用last保留
				last = x;
				x = x->left;
			}
			else {
				x = x->right;	//在右子树 x没用 不用保留
			}
		}
		x = last;
		splay(root,x);		//将x转到根
		return x;
	}
	Splaytree::Node* Splaytree::splayinsert(lld key) {//插入key的节点
		if (root == NULL)	{		//空树直接建
			root = new Node;
			root->key = key;
			return root;
		}
		Node* cut = splayfind(key);	//先找到比key大的
		if (cut == NULL) { //如果全小于key
			Node* p = new Node;
			p->key = key;
			p->left = root;			//root接p左
			root->father = p;			//修改父指针
			root = p;
			return root;
		}else	{ //cut有东西 这时候cut相当于root
			if (cut->key == key) return cut;	//已经存在的话 不插入返回指针
			else	{  //插 cut的左=p左 cut=p右
				Node* p = new Node;
				p->key = key;
				p->left = cut->left;		//p左=cut左
				if (cut->left != NULL) cut->left->father = p;	//修改cut左父指针
				p->right = cut;			//p右=cut
				cut->father = p;			//修改cut父指针
				cut->left = NULL;			//断开cut左(这个已经是p左了)
				root = p;					//p成为新根
				return p;
			}
		}
	}
	Splaytree::Node* Splaytree::splayMax(Node* &root) { //将root子树中最大的旋到root
		Node* p = root;
		if (p == NULL) return NULL;
		while (p->right != NULL) {
			p = p->right;//向右儿子找到底
		}
		splay(root,p);//旋之
		return p;
	}
	void Splaytree::splaydelete(Node* &root,Node* x) { //删除x
		if (x == NULL) return ;
		splay(root,x);			//先旋到根
		if (x->left == NULL) { //如果x左子树为空 直接干
			root = x->right;		//x的右儿子变成根
			if (root != NULL)	{	//断开
				root->father = x->father;//接上rootfather
			}
		}
		else {		//有左儿子 那么将左儿子的最大的提到根 这时候这个点必然没有右子树 将root的右接上去
			Node* p = splayMax(x->left);	//提左儿子的最大
			p->right = x->right;			//接右子树
			if (x->right != NULL) x->right->father = p;		//修改父指针
			p->father = x->father;				//p成为新根
			root = p;
		}
		delete x;
	}
////////////////////////////////////////////////////////////////////////////////////////
inline int Min(int a,int b){return a<b?a:b;}
inline int Abs(int a){return a>0?a:-a;}
const int maxn=60000,maxl=99999999;
struct Qujian {
	int l,r;
} a[maxn];
Splaytree tree;
int n,start;
void iin() {
	int i;
	scanf("%d%d",&n,&start);
	for (i=n-1;i>=0;i--) scanf("%d%d",&a[i].l,&a[i].r);
}
void dp() {
	int i,j,k;
//	a[n].l=-maxl;a[n].r=0;
//	a[n+1].l=0;a[n+1].r=maxl;
	tree.splayinsert(start);
	tree.root->data=0;
//	Splaytree::Node *q=tree.root;
	for (i=0;i<n;i++) {
		lld lmin=maxl,rmin=maxl;
		Splaytree::Node *p;
		while (1) {
			p=tree.splayfind(a[i].l);
			if (p == NULL||p->key>a[i].r) break;
			lmin=Min(lmin,Abs(p->key-a[i].l)+p->data);
			rmin=Min(rmin,Abs(a[i].r-p->key)+p->data);
			tree.splaydelete(tree.root,p);
		}
		p=tree.splayinsert(a[i].l);
		p->data=lmin;
		p=tree.splayinsert(a[i].r);
		p->data=rmin;
	}
	//tree.splayfind(0);
	//printf("%d\n",tree.root->data);
	lld res=maxl;
	int tot=0;
	while (tree.root != NULL) {
		tot++;
		res=Min(res,tree.root->data+Abs(tree.root->key));
		tree.splaydelete(tree.root,tree.root);
	}
	//cout<<tot<<endl;
	printf("%d\n",res);

}
int main() {
	string s("obstacle.7.in "),s2=s;s2[11]='o';s2[12]='u';s2[13]='t';
//	string s="in.in";
	freopen(s.c_str(),"r",stdin);
	iin();
	dp();
	system(s2.c_str());
	return 0;
}
