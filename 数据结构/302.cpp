// compile command:
// gcc -O3 -Wall -static-libgcc DSCT302_2019270102007.cpp -o DSCT302_2019270102007.exe

#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>

#define MAXDEPTH 20

long long compute_depth(long long n)     /*求n个节点的avl的最大深度*/ 
{
	long long depth=0;
	long long dep[MAXDEPTH]={0};
	dep[0]=1;
	dep[1]=2;
	for(long long i=2;i<10;i++)
	{
		dep[i]=dep[i-1]+dep[i-2]+1;
	}
	while(n>=dep[depth])
	{
		depth++;
	}
	return depth;
}

int main(int argc, char *argv[])
{
	
	if(argc!=2)
	{
		printf("input error");
		return -1;
	}
	long long n=atoll(argv[1]);
	long long depth,result=0;
	depth=compute_depth(n);
	/*printf("%lld\n",depth);*/
	long long dp[2000][16]={};
	dp[0][0]=1;
	dp[1][1]=1;
	for(long long i=2;i<=n;i++)
	{
		for(long long j=2;j<=depth;j++)
		{
			for(long long k=0;k<i;k++)
			{
				dp[i][j]+=dp[i-k-1][j-1]*dp[k][j-1];
				dp[i][j]+=2*dp[i-k-1][j-1]*dp[k][j-2];
			}
		}
	}
	for(long long i=1;i<=depth;i++)
	{
		result+=dp[n][i];
	}
	printf("%lld",result);
	return 0;
}
