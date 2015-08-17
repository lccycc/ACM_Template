#include <iostream>
using namespace std;
int main()
{
	int n=50000;
	int i;
	freopen("in.in","w",stdout);
	cout<<n<<' '<<0<<endl;
	for (i=0;i<n;i++) cout<<-i<<' '<<i<<endl;
	return 0; 
}
