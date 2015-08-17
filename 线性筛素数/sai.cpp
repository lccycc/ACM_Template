#include <iostream>
#include <cstring>
using namespace std;
const int maxn=10000;
int n=maxn;
bool IsPrime[maxn];
int Prime[maxn];
int num;
void Eratosthenes () {
	num = 1;
	memset(Prime,0,sizeof(Prime));
	memset(IsPrime,1,sizeof(IsPrime));
	for (int i = 2; i<=n;i++) {
		if (IsPrime[i])
			Prime[num++] = i;
		for (int j = 1; j<num && i*Prime[j]<=n; j++) {
			IsPrime[i*Prime[j]] = 0;
			if (i%Prime[j] == 0) break;
		}
	}
}
int main() {
	Eratosthenes();
	for (int i=0;i<num;i++) if (Prime[i]>=1000) {cout<<Prime[i]<<endl;break;}
	//for (i=2;i<=100;i++) if (1007%i==0) cout<<i<<endl;
	return 0;
}
