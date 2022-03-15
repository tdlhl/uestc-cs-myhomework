// compile command:
// gcc -O3 -Wall -static-libgcc DSCT601_2019270102007.cpp -o DSCT601_2019270102007.exe -fopenmp 

#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include<ctype.h>

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("input error");
		return -1;
	}

	FILE *fp;
	if((fp=fopen(*(argv+1),"r"))==NULL)
	{
		printf("fail to read");
		return -1;
	}
	long long num,vol;
	fscanf(fp,"%lld",&num);
	fscanf(fp,"%lld",&vol);
	long long *weight=(long long *)malloc(sizeof(long long)*(num+1));
	long long *value=(long long *)malloc(sizeof(long long)*(num+1));

	
	for(long long i=1;i<=num;i++)
	{
		fscanf(fp,"%lld",&weight[i]);
		fscanf(fp,"%lld",&value[i]);
	}
	
	long long *dp=(long long *)calloc((vol+1),sizeof(long long));
	/* 
	for(long long i=0;i<=vol;i++)    //初始化正确很重要 ，仅考虑是否能放下第一件物品 
	{
		dp[i]=weight[0]<=i? value[0]:0;
	}
	*/
	for(long long i=1;i<=num;i++)
	{
		for(long long j=vol;j>=weight[i];j--)
		{	
			if((value[i]+dp[j-weight[i]])>dp[j])
			{
				dp[j]=value[i]+dp[j-weight[i]];
			}
		}
	}
	printf("%lld",dp[vol]);
	return 0;
	
}
