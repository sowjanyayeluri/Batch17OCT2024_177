/*
	FILE NAME	: Question2.c
	DESCRIPTION : Finding nth number made of given four digits 1, 4,6 and 9 as the only digits.
	DATE		NAME
	7-NOV-24	Sowjanya
*/


A2.Program to find nth number made of given four digits 1, 4,6 and 9 as the only digits..

#include <stdio.h>
int NthNumber(int [],int);
int containDigits(int [],int);
int main()
{
    int input;
    scanf("%d",&input);
    int digits[]={1,4,6,9};
    printf("%d",NthNumber(digits,input));
    return 0;
}
int NthNumber(int digits[],int n)
{
    int num=0;
    while(n>0)
    {
        num++;
        if(num==containDigits(digits,num))
        {
            n--;
        }
    }
    return num;
}
int containDigits(int digits[],int n)
{
    int temp=n;
    int count=0;
    while(n>0)
    {
        int result=n%10;
        for(int i=0;i<4;i++)
        {
            if(result==digits[i])
                count++;
        }
        n=n/10;
    }
    if(temp>=10)
    {
        if(count==2)
            return temp;
    }
    else
    {
        if(count==1)
            return temp;
    }
    return 0;
    
}