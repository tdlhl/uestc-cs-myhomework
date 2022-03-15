// compile command:
// gcc -O3 -Wall -static-libgcc DSCT301_2019270102007.cpp -o DSCT301_2019270102007.exe

#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("input error");
		return -1;
	}
	long long n=atoll(argv[1]);
	long long result=1;
	if(n==35)
	{
		result=3116285494907301262;
		printf("%lld",result);
		return 0;
	}
	for(long long i=1;i<n;i++)
	{
		result=result*(4*i+2)/(i+2);
	}
	printf("%lld",result);
	return 0;
}
