#include <iostream>
#include <string>
#include <deque>
using namespace std;
struct trienode
{
	trienode * child[100] ,  * pre , * fa ;
	bool end ;
	char myCh;
	int dep ;
	int num ;
};
struct ans
{
	int x , y ;
	char type ;
};
trienode *root , *Viroot ; 
deque<trienode *> queue ;
//int dir[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}} ;
int dir[8][2] = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}} ;
ans key[1050];
int l , c , i , j , m, n ,w;
char map[1010][1010] ;
int find[1050] = {0} ;
string words[1000];
void setup()
{
	root = new trienode ;
	Viroot = new trienode ;
	memset( root , 0 , sizeof(trienode) ) ;
	memset( Viroot , 0 , sizeof(trienode) );
	root->dep = 0 ;
	Viroot->dep = -1 ;
	
}
void build(string & s , int num)
{
	trienode * p = root ;
	for ( int i = 0 ; i < s.size() ; i ++ )
	{
		if ( p -> child[s[i]] == NULL )
		{
			p->child[s[i]] = new trienode;
			memset(p->child[s[i]] , 0 , sizeof(trienode));
			p ->child[s[i]]->fa = p ;
			p -> child[s[i]] -> myCh = s [ i ]  ;
			p -> child[s[i]] -> dep = p -> dep + 1 ;
		}
		p = p -> child[s[i]] ;		
	}
	p->end = 1 ;
	p->num = num ;
}
void buildpre(trienode * p)
{
	trienode * t ;
	if ( p != root ) {
		t = p -> fa ->pre  ;
		while ( t->child[p->myCh] == NULL ) t = t -> pre ;
		p->pre = t->child[p->myCh] ;
	}
	else p ->pre = Viroot ;
	for ( int i = 65 ; i < 91 ; i ++ ) 
		if ( p->child[i] != NULL ) queue.push_back(p->child[i]);
}

void buildAllpre ()
{
	trienode * p ;
	root->pre = Viroot ;
	root ->fa  = Viroot ;
	Viroot->fa = Viroot ;
	Viroot->pre = Viroot ;
	for ( int i = 0 ; i < 100 ; i ++ ) Viroot->child[i] = root ;
	buildpre(root) ;
	while ( !queue.empty() ) {
		buildpre(queue.front());
		queue.pop_front() ;
	}
}
void writeans( int x , int y , int d , int step , int num)
{
	int nx , ny ;
	nx = x - dir[d][0] * step ;
	ny = y - dir[d][1] * step ;
	key[num].x = nx ;
	key[num].y = ny ;
	key[num].type = d + 'A' ;
}

void search( int x , int y)
{
	int i , j , nx , ny;
	trienode * p ;
	for ( i = 0 ; i < 8 ; i ++ ){
		nx = x ; ny = y ;
		nx += dir[i][0];
		ny += dir[i][1];
		p = root ;
		while ( nx < l && nx >= 0  && ny >= 0 && ny < c ){
			while ( p -> child[map[nx][ny]] == NULL ) p = p->pre ;
			p = p -> child [map [ nx ] [ ny ] ] ;
			if (p ->end) writeans( nx , ny , i , p -> dep - 1 , p -> num);
			nx += dir[i][0] ;
			ny += dir[i][1] ;
		}
	}
}


int main()
{
	cin >> l >> c >> w ;
	for ( i = 0 ; i < l ; i ++ ) cin >> map[i] ;
	for ( i = 0 ; i < w ; i ++ ) cin >> words[i] ;
	setup();
	for ( i = 0 ; i < w ; i ++ ) build(words[i] ,i);
	buildAllpre() ;
//	for ( i = 0 ; i < 99 ; i ++ )
//		if ( Viroot -> child[i] == NULL ) Viroot ->child[i] = root ;
	for ( i = -1 ; i <= l ; i ++ ){
		search( i , -1 ) ;
		search( i , c ) ;
	}
	for ( i = 0 ; i < c ; i ++ ){
		search ( -1 , i ) ;
		search ( l , i ) ;
	}
	for ( i = 0 ; i < w ; i ++ ) 
		cout << key[i].x << " " << key[i].y << " " << key[i].type << endl ;
	return 0 ;
}