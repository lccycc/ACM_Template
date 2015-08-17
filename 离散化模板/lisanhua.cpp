struct Lisan
{
	int x;
	int* idx;
} lisan[maxn];
int ll;
int compx(const void *a,const void *b)
{
	return ((Lisan*)a)->x-((Lisan*)b)->x;
}
void lis()
{
	int i,j,k;
	ll=0;
	for (i=0;i<n;i++) 
	{
		lisan[ll].x=obj[i].l;
		lisan[ll++].idx=&obj[i].l;
		lisan[ll].x=obj[i].r;
		lisan[ll++].idx=&obj[i].r;
	}
	qsort(lisan,ll,sizeof(Lisan),compx);
	for (i=j=0;i<ll;i++)
	{
		if (i>0&&lisan[i].x==lisan[i-1].x) j++;
		*lisan[i].idx=i-j;
		lisan[i-j]=lisan[i];
	}
	ll-=j;
}