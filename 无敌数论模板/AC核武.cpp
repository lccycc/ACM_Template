#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<ctime>
#include<map>
#include<string.h>
#include<cmath>
#include<cstring>
using namespace std;
typedef __int64 LL;

/*
 *Last Updated: 2009-12-31
*数论(NumberTheory)类
 *作者: AekdyCoin
 *日期: 2009-12
 *邮箱: chenh1989@gmail.com
 */


const int Plimit = 1000010;//素数表极限大小...
const int MatSize = 2;//矩阵类大小...
template<class T> class Matrix//矩阵类...
    {
        private:
            int size;
            int Moudle;
        public:
            T mat[MatSize][MatSize];
            Matrix():size(MatSize){memset(mat,0,sizeof(mat));}
            Matrix(int _size):size(_size){memset(mat,0,sizeof(mat));}
            Matrix(int _size,int _Moudle):size(_size),Moudle(_Moudle){memset(mat,0,sizeof(mat));}
            inline void MatOut()
            {
                int i,j;
                for(i=0;i<size;++i,cout<<endl)
                    for(j=0;j<size;++j)
                        cout<<mat[i][j]<<' ';
            }
            inline Matrix operator*(const Matrix&M)
            {
                Matrix<T> ret(size,Moudle);
                int i,j,k;
                for(i=0;i<size;++i)
                    for(j=0;j<size;++j)
                        for(k=0;k<size;++k)
                        ret.mat[i][j]+=mat[i][k]*M.mat[k][j]%Moudle,ret.mat[i][j]%=Moudle;
                return ret;
            }
            template<class TT>inline Matrix operator^(TT k)
            {
                Matrix<T> ret=*this,x=*this;
                if(k==0)
                {
                    int i;
                    for(i=0;i<size;++i)ret.mat[i][i]=1%Moudle;
                    return ret;
                }
                if(k==1)return *this;
                --k;
                while(k)
                {
                    if(k&0x1)ret=ret*x;
                    x=x*x;
                    k>>=1;
                }
                return ret;
            }
    };
class NumberTheory 
{
private:
    int MaxPrime;//素数表中最大素数...
    bool IsPlistCreated;//是否已经创建素数表...
    bool IsP[Plimit>>1];//素数静态表...
    int Plist[Plimit>>1];
    int Plen;//素数表大小...
    LL buf_p[105];//为了保证安全,故使用位...
    int i_buf_p[105],buf_cnt[105],buf_len;//随机分解素因子相关...
    template<class T> inline T Phi_Process(T n,T p[],int len)
    {
        T ret = n;
        int i;
        for(i=0;i<len;++i)ret-=ret/p[i];
        return ret;
    }
    template<class T>inline T Pollard_rho(T c, T num)
    {
        int i(1), k(2);
        T x = rand() % num;
        T y = x, comDiv;
        do
        {
            ++i;
            if ((x=MulMod(x,x,num)-c)<0)x+=num;
            if (x==y)break;
            comDiv=GCD((y-x+num)%num,num);
            if (comDiv>1&&comDiv<num )return comDiv;
            if (i==k)
            {
                y=x;
                k<<=1;
            }
        }while(1);
        return num;
    }
    template<class T> void RanGetFac(T num)
    {
        if (IsPrime(num))
        {
            buf_p[buf_len++] = num; 
            return;
        }
        T factor;
        do
        {
            factor=Pollard_rho(rand()%(num-1) + 1, num);
        }while (factor>=num);
        RanGetFac(factor);
        RanGetFac(num/factor);
    }
    LL lrand(){return LL(rand())*rand();}
    template<class T>inline T ABS(T n){return n<0?n:-n;}
    template<class T>inline T DIV(T a,T b){if(a>0||(ABS(a)%ABS(b)==0))return a/b;return a/b+1;}
    template<class T>inline void SWAP(T &A,T &B){T C=A;A=B;B=C;}
    template<class T>inline void SwapVec(T a[],int len){int i;for(i=0;i<len/2;++i)SWAP(a[i],a[len-1-i]);}
    inline int Inval(int a,int b,int n){
        int x,y,e;
        EXT_GCD(a,n,x,y);
        e=(LL)x*b%n;
        return e<0?e+n:e;
    }
public:
    NumberTheory()
    {
        MaxPrime=0;
        IsPlistCreated=false;
        memset(IsP,false,sizeof(IsP));
        Plen=0;
    }
    inline int GetMaxPrime(){return MaxPrime;}//获取最大素数...
    inline int GetPlen(){return Plen;}//获取素数表大小...
 
    /*
     *功能: 创建[1,n]内的静态素数表
     *注意: 使用了压缩优化，时空效率均是普通筛法的倍
     *传入: n --所需要创建的素数表的上界(注意不能超过Plimit)
     *返回: 无
     */

    inline void CreatPlist(int n)
    {
        if(IsPlistCreated)return;
        int i,j,buf(1),M(1),L((n-1)>>1);
        while(buf<n)++M,buf=(M-1)<<1|1,buf*=buf;--M;
        Plist[Plen++]=2;
        for(i=1;i<M;++i)
            if(!IsP[i])
                for(buf=i<<1|1,j=i+buf;j<L;IsP[j]=true,j+=buf);
        for(i=1;i<L;++i)if(!IsP[i])Plist[Plen++]=i<<1|1;
        IsPlistCreated=true;
    }

    /*
     *功能: 获取素数表中下标为x的元素的值
     *注意: 若使用使忘记使用CreatPlist,则会默认执行CreatPlist(Plimit)
     *传入: x --所需要返回的素数的下标(x>=0)
     *返回: 下标为x的元素的值,-1表示不存在
     */

    inline int operator[](const int x){if(!IsPlistCreated)CreatPlist(Plimit);return x<0||x>=Plen?-1:Plist[x];}

    template<class T>inline T MOD(T a,T b){if(a>=b)a%=b;return a<0?a+b:a;}//计算a%b,支持a<=0

    /*
     *功能: 二分计算a^b mod c
     *注意: 无法处理c*c溢出可是c+c不溢出的情况
     *传入: a,b,c(a,b可以小于,可是c不能小于等于,否则返回无解)
     *返回: a^b mod c的值,-1表示不存在
     */

    template<class T,class Tb> inline T PowMod(Tb a,T b,T c)
    {
        if(c==1)return 0;
        if(c<=0)return -1;
        a=MOD(a,(Tb)c);
        Tb ret(1L%c);
        while(b){if(b&0x1)ret=ret*a%c;a=a*a%c;b>>=1;}
        return (T)ret;
    }

    /*
     *功能: 二分计算a*b mod c
     *注意: 可以处理c+c不溢出的情况
     *传入: a,b,c(a,b可以小于,可是c不能小于等于,否则返回无解)
     *返回: a*b mod c的值,-1表示不存在
     */

    template<class T>inline T MulMod(T a,T b,T c)
    {
        if(c==1)return 0;
        if(c<=0)return -1;
        T ret=0,tmp;
        tmp=a=MOD(a,c);
        b=MOD(b,c);
        while(b)
        {
            if(b&0x1)if((ret+=tmp)>=c)ret-=c;
            if((tmp<<=1)>=c)tmp-=c;
            b>>=1;
        }
        return ret;
    }

    /*
     *功能: 二分计算a^b mod c
     *注意: 可以处理c*c溢出可是c+c不溢出的情况(效率相比PowMod低,注意取舍)
     *传入: a,b,c(a,b可以小于,可是c不能小于等于,否则返回无解)
     *返回: a^b mod c的值,-1表示不存在
     */

    template<class T> inline T HPowMod(T a,T b,T c)
    {
        if(c==1)return 0;
        if(c<=0)return -1;
        a=MOD(a,c);
        T ret(1L%c);
        while(b){if(b&0x1)ret=MulMod(ret,a,c);a=MulMod(a,a,c);b>>=1;}
        return ret;
    }

    /*
     *功能: 判断素数
     *注意: 若数字<Plimit则迅速查询，否则使用米勒测试
     *传入: n
     *返回: n是否为素数
     */

    template<class T> inline bool IsPrime(T n)
    {
        if(n<2)return false;
        if(n==2)return true;
        if(!(n&0x1))return false;
        if(n==3||n==5||n==7||n==11||n==13||n==17||n==19||n==23)return true;
        if(n%3==0||n%5==0||n%7==0||n%11==0||n%13==0||n%17==0||n%19==0||n%23==0)return false;
        if(n<Plimit)
        {
            if(!IsPlistCreated)CreatPlist(Plimit);
            return !IsP[(n-1)>>1];
        }
        T x,m;
        int k,i,t=2;
        while(t--)
        {
            k=0;
            for(m=n-1;!(m&0x1);m>>=1,++k);
            x=HPowMod((T)lrand()%(n-2)+2,m,n);
            if(x!=1)
            {
                for(i=0;i<k&&x!=n-1;++i)x=MulMod(x,x,n);
                if(i==k)return false;
            }
        }
        return true;
    }

    /*
     *功能: 分解素因子
     *注意: 若Plimit^2>=n,则扫描素数表，否则使用pollard_rho随机分解素因子
     *传入: n,p[],cnt[],len,mode
     *(mode = 0-普通模式-强制使用扫描素数表法(2+)-强制使用pollard_rho)
     *返回: p[]-素因子值,cnt[]-素因子数,len-素因子种类数
     */

    template<class T> inline void Split(T n,T p[],int cnt[],int &len,int mode)
    {
        len=0;
        int i,j;
        if(!IsPlistCreated)CreatPlist(Plimit);
        if(mode==1||(!mode&&n/Plimit<=Plimit))
        {
            for(i=0;i<Plen&&n>=Plist[i];++i)
                if(n%Plist[i]==0)
                {
                    p[len]=Plist[i];
                    cnt[len]=0;
                    while(n%p[len]==0)++cnt[len],n/=p[len];
                    ++len;
                }
            if(n!=1)p[len]=n,cnt[len]=1,++len;
            return;
        }
        buf_len=0;
        RanGetFac(n);
        len=0;
        sort(buf_p,buf_p+buf_len);
        for(i=0;i<buf_len;++i)
        {
            p[len++]=(T)buf_p[i];
            for(j=i+1;j<buf_len&&buf_p[j]==buf_p[i];++j);
            i=j-1;
        }
        for(i=0;i<len;++i)cnt[i]=0;
        for(i=0;i<len;++i)
            while(n%p[i]==0)
                n/=p[i],++cnt[i];
        return;
    }

    /*
     *功能: 获取一个数的因子数
     *注意: n<=Plimit*Plimit
     *传入: n
     *返回: 一个数的因子数
     */

    template<class T> inline int FacCnt(T n)
    {
        int ret=1,i;
        Split(n,i_buf_p,buf_cnt,buf_len,0);
        for(i=0;i<buf_len;++i)ret*=(buf_cnt[i]+1);
        return ret;
    }

    /*
     *功能: 获取一个数的因子和
     *注意: n<=Plimit*Plimit
     *传入: n
     *返回: 一个数的因子和
     */

    template<class T> inline int FacSum(T n)
    {
        int ret=1,i,j,tmp;
        Split(n,i_buf_p,buf_cnt,buf_len,0);
        for(i=0;i<buf_len;++i)
        {
            for(tmp=0,j=0;j<=buf_cnt[i];++j,tmp=tmp*i_buf_p[i]+1);
            ret*=tmp;
        }
        return ret;
    }
    /*
     *功能: 得到[1..Limit]内数的所有因子数
     *注意: Limit至多为百万级别
     *传入: cnt[]-存放结果Limit-极限值
     *返回: cnt[]
     */

    inline void SegFacCnt(int cnt[],int Limit)
    {
        memset(cnt,0,sizeof(cnt));
        int i,j;
        for(i=1;i<=Limit;++i)
            for(j=i;j<=Limit;j+=i)
                ++cnt[j];
    }

    /*
     *功能: 得到[1..Limit]内数的所有因子和
     *注意: Limit至多为百万级别
     *传入: cnt[]-存放结果Limit-极限值
     *返回: cnt[]
     */

    inline void SegFacSum(int cnt[],int Limit)
    {
        memset(cnt,0,sizeof(cnt));
        int i,j;
        for(i=1;i<=Limit;++i)
            for(j=i;j<=Limit;j+=i)
                cnt[j]+=i;
    }

    /*
     *功能: 判断一个数是否是SquareFree数
     *注意: 不能处理Long Long
     *传入: n
     *返回: n是否是SquareFree数
     */

    inline bool IsSquareFree(int n)
    {
        Split(n,i_buf_p,buf_cnt,buf_len,1);
        int i;
        for(i=0;i<buf_len;++i)if(buf_cnt[i]!=1)return false;
        return true;
    }

    /*
     *功能: 返回个数的最大公约数
     *注意: A,B类型必须一致
     *传入: A,B
     *返回: A,B的最大公约数
     */    

    template <class T>inline T GCD(T A,T B){T C;while(B!=0)C=B,B=A%B,A=C;return A;}

    /*
     *功能: 返回个数的最小公倍数
     *注意: A,B类型必须一致
     *传入: A,B
     *返回: A,B的最小公倍数
     */    

    template <class T>inline T LCM(T A,T B){return A*(B/GCD(A,B));}

    /*
     *功能: 扩展欧几里得
     *传入: a,b,x,y
     *返回: x,y满足ax+by=gcd(a,b)的一组解
     */    

    template <class T>T EXT_GCD(T a,T b,T& x,T& y)
    {
        T t,ret;
        if (!b){
            x=1,y=0;
            return a;
        }
        ret=EXT_GCD(b,a%b,x,y);
        t=x,x=y,y=t-a/b*y;
        return ret;
    }

    /*
     *功能: 返回一个数a对于n的乘法逆元
     *注意: GCD(a,n)=1才有解
     *传入: a,n (a可以为负)
     *返回: a对于n的乘法逆元,不存在则返回-1
     */

    template <class T>inline T InvMod(T a,T n){
        if(n<=0)return -1;
        T d,x,y;
        d=EXT_GCD(a,n,x,y);
        if(d!=1)return -1;
        return MOD(x,n);
    }

    /*
     *功能: 返回一个数的欧拉函数
     *注意: 不能处理Long Long
     *传入: n(n<=Plimit*Plimit)
     *返回: n的欧拉函数
     */

    int Phi(int n)
    {
        int ret(n),i;
        if(!IsPlistCreated)CreatPlist(Plimit);
        for(i=0;i<Plen&&n>=Plist[i];++i)
            if(n%Plist[i]==0)
            {
                ret-=ret/Plist[i];
                while(n%Plist[i]==0)n/=Plist[i];
            }
        if(n!=1)ret-=ret/n;
        return ret;
    }

    /*
     *功能: 返回一个数的欧拉函数
     *注意: 可以处理<2^63,必须先分解
     *传入: n(n<2^63)
     *返回: n的欧拉函数
    */

    template<class T> inline T HPhi(T n)
    {
        T buf_p[105];
        int buf_cnt[105],buf_len;
        Split(n,buf_p,buf_cnt,buf_len,0);
        return Phi_Process(n,buf_p,buf_len);
    }

    /*
     *功能: 将[1..Limit]的数的欧拉函数处理出来
     *传入: phi[]-存放结果的数组Limit-上限(最多几百万级别)
     *返回: phi[]-区间内的欧拉函数
    */

    inline void SegPhi(int phi[],int Limit)
    {
        memset(phi,0,sizeof(phi));
        int i,j;
        for(phi[1]=1,i=2;i<=Limit;++i)
            if(!phi[i])
                for(j=i;j<=Limit;j+=i)
                {
                    if(!phi[j])phi[j]=j;
                    phi[j]-=phi[j]/i;
                }
    }

    /*
     *功能: 将n转化为b进制
     *传入: n,b (|b|>=2,b可以为负)
     *返回: Ans[] -答案,Ans[0]保存最高位,以此类推. len -长
    */

    template<class T> inline void ToBase(T n,int b,T Ans[],int &len)
    {
        if(n==0){Ans[0]=0;len=1;return;}
        len=0;
        if(b>0)
            while(n!=0)
            {
                Ans[len++]=n%b;
                n/=b;
            }
        else
            while(n!=0)
            {
                Ans[len]=n-DIV(n,b)*b;
                n=DIV(n,b);
                ++len;
            }
        SwapVec(Ans,len);
        return;
    }

    /*
     *功能: 将n转化为b进制(进制中最小元素为a,最大为a+b-1)
     *传入: n,a,b
     *返回: Ans[] -答案,Ans[0]保存最高位,以此类推. len -长
    */

    template<class T>inline void HToBase(T n,int a,int b,T Ans[],int &len)
    {
        int cnt,i;
        if(n==0){Ans[0]=0;len=1;return;}
        LL sum;
        cnt=len=sum=0;
        if(a)
        {
            while(sum<n)
            {
                sum=sum*b+a;
                ++cnt;
            }
        }
        if(sum>n)
            sum/=b,--cnt; 
        n-=sum;
        while(n)
        {
            Ans[len++]=n%b;
            n/=b;
        }
        for(i=0;i<cnt&&len-i-1>=0;++i)Ans[len-i-1]+=a;
        cnt=cnt>len?cnt:len;
        SwapVec(Ans,cnt);
        return;
    }

    /*
     *功能: 将Ans[]中内容转化为进制数
     *传入: b -进制数,Ans[] -数组,len-长度
     *返回: Ans[]中内容的进制数
    */

    template<class T> inline T BaseVal(int b,T Ans[],int len)
    {
        T ret=0;
        int i;
        for(i=0;i<len;++i)ret=ret*b+Ans[i];
        return ret;
    }

    /*
    *功能: 求解x^2 - n* y^2 = 1的最小整数解
    *注意: 如果n为完全平方数,则返回一组平凡解(x=0,y=1)
    *否则返回一组最小正解x,y(x,y>0)
    *注意如果n比较大(几百)那么x,y可能需要高精度
    *返回: 最小整数解x,y
    */

    template<class T>inline int Pell(T n,T& x,T& y)
    {
        T aa=(T)sqrt((double)n),a=aa;
        x=1;
        y=0;
        if(aa*aa==n)return 0;
        T p1=1,p2=0,q1=0,q2=1,g=0,h=1;
        while(true)
        {
            g=-g+a*h;
            h=(n-g*g)/h;
            x=a*p1+p2;
            y=a*q1+q2;
            if(x*x-n*y*y==1)
                return 1;
            p2=p1;
            q2=q1;
            p1=x;
            q1=y;
            a=(g+aa)/h;
        }
        return 1;
    }    

    /*
    *功能: 求Fibonacci数列第N项Mod K的值
    *注意: F[0]=F[1]=1;F[n]=F[n-1]+F[n-2] (n>=2)
    *注意如果n比较小,可以把K取大一点:(不过会慢一点点-_-)
    */

    inline int Fibonacci(int N,int K)
    {
        if(N<=1)return 1%K;
        --N;
        Matrix<LL> X(2,K);
        X.mat[0][0]=X.mat[0][1]=X.mat[1][0]=1;
        X=X^N;
        return (X.mat[0][0]+X.mat[0][1])%K;
    }

    /*
    *功能: 求Fibonacci数列前N项(F[0]+F[1]+...+F[N])和Mod K的值
    *注意: F[0]=F[1]=1;F[n]=F[n-1]+F[n-2] (n>=2)
    *注意如果n比较小,可以把K取大一点:(不过会慢一点点-_-)
    */

    inline int Fibonacci_Sum(int N,int K)
    {
        N+=2;
        return MOD(Fibonacci(N,K)-1,K);
    }

   /*
    *功能: 求解A^x = B (mod C)的最小x(x>=0)
    *注意: 复杂度大约为O(0.5*C^0.5*(logC)^2)，C必须在signed int内(否则可能有溢出,稍微修改就可以)
    *使用 Hash表优化可以达到约O(0.5C^0.5*logC)
    *返回: -1表示无解,否则返回最小的x...
    */

    int BabyStep(int A,int B,int C)
    {
        map<int,int> Hash;
        LL buf=1%C,D=buf,K;
        int i,d=0,tmp;
        for(i=0;i<=100;buf=buf*A%C,++i)if(buf==B)return i;
        while((tmp=GCD(A,C))!=1)
        {
            if(B%tmp)return -1;
            ++d;
            C/=tmp;
            B/=tmp;
            D=D*A/tmp%C;
        }
        Hash.clear();
        int M=(int)sqrt(C+0.0);
        for(buf=1%C,i=0;i<=M;buf=buf*A%C,++i)if(Hash.find((int)buf)==Hash.end())Hash[(int)buf]=i;
        for(i=0,K=PowMod((LL)A,M,C);i<=M;D=D*K%C,++i)
        {
            tmp=Inval((int)D,B,C);
            if(tmp>0&&Hash.find(tmp)!=Hash.end())return i*M+Hash[tmp]+d;
        }
        return -1;
    }

};
NumberTheory T;
int main()
{
    T[-1];
    int k;
    LL x,y,n;
    while(scanf("%I64d%d",&n,&k)!=EOF)
    {
        if(n == 4 || n == 9 || n == 16 || n == 25)
        {
            puts("No answers can meet such conditions");
            continue;
        }
        T.Pell(n,x,y);
        x%=8191;
        y%=8191;
        Matrix<LL> res(2,8191);
        res.mat[0][0] = x;
        res.mat[0][1] = n*y;
        res.mat[1][0] = y;
        res.mat[1][1] = x;
        res=res^(k-1);
        LL ans = res.mat[0][0] * x + res.mat[0][1]*y;
        ans %= 8191;
        cout<<ans<<endl;
    }
    return 0;
}
