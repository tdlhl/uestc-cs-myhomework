// compile command:
// gcc -O3 -Wall -static-libgcc DSCT102_2019270102007.cpp -o DSCT102_2019270102007.exe
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

long long a[12]={0};

long long allsame(long long num)
{
	long long result;
	if(num==3)
	{
		result=5;
		return result;
	}
	else if(num==4)
	{
		result=8;
		return result;
	}
	else if(num==5)
	{
		result=12;
		return result;
	}
	else if(num>=6)
	{
		result=num+allsame(num/2)+allsame((num-num/2));
		return result;
	}
}

void swap(long long *x, long long *y) 
{
    long long tmp = *x;
    *x = *y;
    *y = tmp;
}

long int patition(long long *a, long long left,long long right) {
    long long j = left;  
    long long i = j - 1; 
    long long key = a[right];  
    for (; j < right; ++j) {
        if (a[j] <= key)
            swap(&a[j], &a[++i]);
    }

    swap(&a[right], &a[++i]);

    return i;
}

void quickSort(long long *a,long long left,long long right) {
    if (left>=right)
        return;
    long long mid = patition(a,left,right);
    quickSort(a, left, mid - 1);
    quickSort(a, mid + 1, right);
}

int main(int argc, char *argv[])
{
	long long result=0,L=0,i,j=0;
	for ( j = 1;j <= argc;j++){
		a[j-1]=atoll(argv[j]);
	}
	j=j-3;
	quickSort(a, 0, j);
	if(j>=3&&a[0]==a[j])
	{
		j++;
		long long times=allsame(j);
		result=a[0]*times;
		printf("%lld",result) ;
		return 0;
	}
	L=a[0]+a[1];
	for(i=2;i<=j;i++)
	{
		result+=L;
		L+=a[i];	
	}
	result+=L;
	/*printf("%lld\n",j);
	for(i=0;i<=j;i++)
	{
		printf("%lld ",a[i]);
	}*/
	printf("%lld",result) ;
	return 0;
}
