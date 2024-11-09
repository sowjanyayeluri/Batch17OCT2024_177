/*
	FILE NAME	: Question1.c
	DESCRIPTION : Finding whether number can be expressed as the sum of two prime numbers
	DATE		NAME
	7-NOV-24	Sowjanya
*/


A1. Check whether a given number can be expressed as the sum of two prime number

#include <stdio.h>
int to_check_sum(int );
int main()
{
    int num;
    printf("Enter a positive number: ");
    scanf("%d",&num);
    int count = to_check_sum(num);
    printf("\nNoofWays = %d",count);
    return 0;
}
int to_check_sum(int number)
{
    int count=0;
    int temp=number;
    int j=0;
    for(int i=1;i<=number;i++)
    {
        int flag=0,flag1=0;
        number--;
        for(j=1;j<=number;j++)
        {
            if(i%j==0)
            {
                flag++;
            }
            if(number%j==0)
            {
                flag1++;
            }
        }
        if(flag==2 && flag1==2)
        {
            printf("\n%d = %d + %d",temp,i,number);
            count++;
        }
    }
    return count;
}

