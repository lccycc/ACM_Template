#include <stdio.h>
#include <time.h>
#define maxn 100000
#define mod  100000
int a[maxn];
int c[maxn];
int n,cnt;

void MergeSort(int l,int r){  
 int mid,i,j,tmp;
 if (r>l+1)     {      
  mid = (l+r)/2;        
  MergeSort(l,mid);        
  MergeSort(mid,r);     
  tmp = l;
  for (i=l,j=mid;i<mid&&j<r;)    
  {
   if (a[i]>a[j])        
   {             
    c[tmp++] = a[j++];    
    cnt+=mid-i;
	cnt = cnt % mod ;
   }           
   else  c[tmp++] = a[i++];   
  }         
  if (j<r) for (;j<r;++j) c[tmp++] = a[j]; 
  else  for (;i<mid;++i)  c[tmp++] = a[i];
  for (i=l;i<r;++i)      
   a[i] = c[i];  
 }
}

int main()
{
 int i;
 freopen("sequence.in","r",stdin);
 freopen("sequence.out","w",stdout);

 scanf("%d",&n) ;
 for(i=0;i<n;i++) scanf("%d",&a[i]);
  
  cnt=0;
  MergeSort(0,n);
  printf("%d\n",cnt);
  printf( "%d" , c [ 0 ] ) ;
  for(i=1;i<n;i++) printf(" %d",c[i]);
  
  printf("\n");
 

 return 0;
}

