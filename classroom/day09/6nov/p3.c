#include <stdio.h>
#include <math.h>
int main()
{
	int num=0;
    int num1=2;
    int num2=0;
    while(num<=64)
    {
		num=pow(num1,num2);
        if(num==64)
        	printf("%d",num);
         
        else
            printf("%d, ",num);
        num2++;
	}
	return 0;
}
