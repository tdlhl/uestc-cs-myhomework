// compile command:
// gcc -O3 -Wall -static-libgcc DSCT601_2019270102007.cpp -o DSCT601_2019270102007.exe

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<malloc.h>
#include<ctype.h>

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("input error");
		return -1;
	}
	long long num=atoll(argv[1]);
	for(long long i=2;i<=sqrt(num);i++)     
    {
     	while(num!=i)       
     	{
       		if(num%i==0)
     		{ 
			 	printf("%lld*",i);   
        		num/=i;
     		}
       		else
        		break;
    	}
	}
	printf("%lld",num);
	return 0; 
}
