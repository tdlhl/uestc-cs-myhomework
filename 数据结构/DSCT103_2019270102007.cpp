// compile command:
// gcc -O3 -Wall -static-libgcc DSCT103_2019270102007.cpp -o DSCT103_2019270102007.exe
#include<stdio.h>
#include<ctype.h>
#include<stdbool.h>
#include<stdint.h>
#include<math.h>
#include<stdlib.h>
#define ELE_COUNT 4

typedef struct
{
	double x1_coordinate;
	double x2_coordinate;
	double(*func)(double,double*);
}Interval;
double integration(Interval*i,double accuracy,double* coef)
{
	double sum=0,temp_coordinate=i->x1_coordinate;
	int c=0;//counter
	c=(i->x2_coordinate-i->x1_coordinate)/accuracy;
	for(int c1=0; c1<c; c1++)
	{
		temp_coordinate+=accuracy;
		sum+=(i->func(temp_coordinate,coef))*accuracy;
	}
	return sum;
}

/*------------------------------------------------------------
METHOD:  polyfit
INPUTS:  dependentValues[0..(countOfElements-1)] 	//即xData
         independentValues[0...(countOfElements-1)] //即yData
         countOfElements							//即元素个数
         order - Order of the polynomial fitting	//要拟合的最高阶数
OUTPUTS: coefficients[0..order] - indexed by term	
               (the (coef*x^3) is coefficients[3])
         //拟合的各项系数的结果，x^3 的系数为coefficients[3]
-------------------------------------------------------------*/
int polyfit(const double* const dependentValues ,
            const double* const independentValues ,
            unsigned int        countOfElements ,
            unsigned int        order ,
            double* coefficients)
{
    // Declarations...
    // ----------------------------------
    enum { maxOrder = 5 };
    double B[maxOrder + 1] = {0.0f};
    double P[((maxOrder + 1) * 2) + 1] = {0.0f};
    double A[(maxOrder + 1) * 2 * (maxOrder + 1)] = {0.0f};
    double x , y , powx;
    unsigned int ii , jj , kk;
    // Verify initial conditions....
    // ----------------------------------
    // This method requires that the countOfElements > 
    // (order+1) 
    if (countOfElements <= order)
        return -1;
    // This method has imposed an arbitrary bound of
    // order <= maxOrder.  Increase maxOrder if necessary.
    if (order > maxOrder)
        return -1;
    // Begin Code...
    // ----------------------------------
    // Identify the column vector
    for (ii = 0; ii < countOfElements; ii++)
    {
        x = dependentValues[ii];
        y = independentValues[ii];
        powx = 1;

        for (jj = 0; jj < (order + 1); jj++)
        {
            B[jj] = B[jj] + (y * powx);
            powx = powx * x;
        }
    }
    // Initialize the PowX array
    P[0] = countOfElements;
    // Compute the sum of the Powers of X
    for (ii = 0; ii < countOfElements; ii++)
    {
        x = dependentValues[ii];
        powx = dependentValues[ii];

        for (jj = 1; jj < ((2 * (order + 1)) + 1); jj++)
        {
            P[jj] = P[jj] + powx;
            powx = powx * x;
        }
    }
    // Initialize the reduction matrix
    //
    for (ii = 0; ii < (order + 1); ii++)
    {
        for (jj = 0; jj < (order + 1); jj++)
        {
            A[(ii * (2 * (order + 1))) + jj] = P[ii + jj];
        }

        A[(ii * (2 * (order + 1))) + (ii + (order + 1))] = 1;
    }
    // Move the Identity matrix portion of the redux matrix
    // to the left side (find the inverse of the left side
    // of the redux matrix
    for (ii = 0; ii < (order + 1); ii++)
    {
        x = A[(ii * (2 * (order + 1))) + ii];
        if (x != 0)
        {
            for (kk = 0; kk < (2 * (order + 1)); kk++)
            {
                A[(ii * (2 * (order + 1))) + kk] =
                    A[(ii * (2 * (order + 1))) + kk] / x;
            }

            for (jj = 0; jj < (order + 1); jj++)
            {
                if ((jj - ii) != 0)
                {
                    y = A[(jj * (2 * (order + 1))) + ii];
                    for (kk = 0; kk < (2 * (order + 1)); kk++)
                    {
                        A[(jj * (2 * (order + 1))) + kk] =
                            A[(jj * (2 * (order + 1))) + kk] -
                            y * A[(ii * (2 * (order + 1))) + kk];
                    }
                }
            }
        }
        else
        {
            // Cannot work with singular matrices
            return -1;
        }
    }
    // Calculate and Identify the coefficients
    for (ii = 0; ii < (order + 1); ii++)
    {
        for (jj = 0; jj < (order + 1); jj++)
        {
            x = 0;
            for (kk = 0; kk < (order + 1); kk++)
            {
                x = x + (A[(ii * (2 * (order + 1))) + (kk + (order + 1))] *
                    B[kk]);
            }
            coefficients[ii] = x;
        }
    }
    return 0;
}

double dxs(double a,double* coef)
{
	double result=0;
	int cs=1;
	for(int i=0;i<4;i++)
	{
		result+=cs*coef[i];
		cs*=a;
	}
	return result;
}

int main(int argc, char *argv[])
{
	const unsigned int order = 3;
	double coef1[order+1],coef0[order+1],coef[order+1];
	if(argc != 16 + 1)
	{
		printf("input error");
		return -1;
	}
	double x0[4], y0[4],x1[4],y1[4];
	double left,right,diff,base,output;
	int i,j=0,k=0,m=0,n=0,res0,res1;
	for(i=0;i<16;i++) 
	{
		if(i<8)
		{
			if(i%2==0)
			{
				x0[j++]=atoll(argv[i+1]);
			}
			else
			{
				y0[k++]=atoll(argv[i+1]);
			}
		}
		else
		{
			if(i%2==0)
			{
				x1[m++]=atoll(argv[i+1]);
			}
			else
			{
				y1[n++]=atoll(argv[i+1]);
			}
		}
	}
	res0=polyfit(x0,
                 y0,
                 ELE_COUNT,
                 order,
                 coef0);
	res1=polyfit(x1,
                 y1,
                 ELE_COUNT,
                 order,
                 coef1);
    for(i=0;i<order+1;i++)
    {
    	coef[i]=coef0[i]-coef1[i];
	}
    right=fmax(x0[3],x1[3]);
    left=fmin(x0[0],x1[0]);
    Interval jf;
	jf.x1_coordinate=left;
	jf.x2_coordinate=right;
	jf.func=dxs;
	diff=integration(&jf, 0.0001,coef);	
	Interval ori;
	ori.x1_coordinate=x1[0];
	ori.x2_coordinate=x1[3];
	ori.func=dxs;
	base=integration(&ori, 0.0001,coef1);
	output=abs((diff/base)*100);
	printf("%f%%",output);
	return 0;
}

