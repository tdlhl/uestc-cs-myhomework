// compile command:
// gcc -O3 -Wall -static-libgcc DSCT101_2019270102007.cpp -o DSCT101_2019270102007.exe

#include<stdio.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("input error");
		return -1;
	}
	long x=1,y=0,num,output,evennum=0;
	num=atol(argv[1]);
	while(num!=0)
	{
		if(num%2!=0)
		{
			x+=y;
			y+=evennum*x;
			evennum=0;
		}
		else
		{
			evennum++;
		}
		num=num/2;
	}
	output=x+y;
	printf("%ld",output);
	return 0;
}

