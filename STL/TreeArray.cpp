int lowbit(int x)
{
	return x&(x^(x-1)); //»òreturn (-x)&x;

}
void add(int x,int delta)
{
	for (;x<=n;x+=lowbit(x))
		c[x]+=delta;
}
int sum(int x)
{
	int ans;
	for (ans=0;x;x-=lowbit(x))
		ans+=c[x];
	return ans;
}
