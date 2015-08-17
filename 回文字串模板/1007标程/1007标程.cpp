#include<iostream>
#include<string>
#include<algorithm>
using namespace std;

#define M 210001
#define Min(x,y) x<y?x:y
char Str[M],str[M*2+3];
int len[M*2+3];
int l;

void cal(int pos)
{
    int i,ans;
    bool flag;
    flag = true;
    ans=0;
    for(i=1;flag;i++)
    {
        if(pos-i>=0 && pos+i<l)
        {
            if(str[pos-i]==str[pos+i])
            {
                i+=Min(len[pos-i],len[pos+i]);
                ans=i;
            }
            else flag=false;
        }
        else flag=false;
    }
    len[pos]=ans;
}

void dfs(int s,int e)
{
    if(s>e) return ;
    int mid = (s+e)/2;
    cal(mid);
    dfs(s,mid-1);
    dfs(mid+1,e);
}

int main()
{
	char ch;
    int i, j, res;
	int x,y;
	int start, end;
	freopen("e: \\pro.in","r",stdin);
	freopen("e: \\pro.out","w",stdout);

    while(scanf("%c",&ch)!=EOF)
    {
		getchar();
		gets(Str);
		x = ch-'a';
        l=strlen(Str);
		for(i=0;i<l;i++)
		{
			y = Str[i]-'a';
			Str[i] = (y-x+26)%26+'a';
		}
        memset(len,0,sizeof(len));
        j=0;
        for(i=0;i<l;i++)
        {
		    str[j]=Str[i];
            j++;
            str[j]=' ';
            j++;
        }
        j--;
        str[j]='\0';
        l=j;
        dfs(0,l-1);
        int m = 0 ;
		start = -1;
		end = -1;
        for(i=0;i<l;i++)
        {
            if(str[i]==' ')
            {
                if(len[i]%2!=0)
                {
                    res=len[i]/2+1;
					res*=2;
                    if(res>m)
					{
						m=res;
						start = (i+1)/2-res/2;
						end = start + res -1;
					}					
                }
                else
                {
                    if(len[i]>m)
					{
						m=len[i];
						start = (i+1)/2-len[i]/2;
						end = start + len[i] -1;
					}
                }
            }
            else
            {
                if(len[i]%2==0)
                    res=len[i]+1;
                else res=len[i];
                if(res>m)
				{
					m=res;
					start = i/2-res/2;
					end = start + res -1;
				}
            }
        }
		if(m==1) printf("No solution!\n");
		else 
		{
			printf("%d %d\n",start,end);
			for(i=start;i<=end;i++)
				printf("%c",Str[i]);
			printf("\n");
		}
    }
}