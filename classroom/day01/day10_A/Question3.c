/*
	FILE NAME	: Question3.c
	DESCRIPTION : Finding the  n-th number made of prime digits
	DATE		NAME
	7-NOV-24	Sowjanya
*/
A3. Write a program to find the  n-th number made of prime digits

#include <stdio.h>
int findNthNumber(int [],int);
int isPrime(int [],int);
int main()
{
    int testCases,input;
    scanf("%d",&testCases);
    for(int i=0;i<testCases;i++)
    {
        scanf("%d",&input);
           int prime_digits[]={2,3,5,7};
        //printf("%d",isPrime(prime_digits,3));
        printf("%d",findNthNumber(prime_digits,input));
    }
    return 0;
}
int findNthNumber(int primes[],int n)
{
    int num=2;
    while(n>0)
    {
        if(num==isPrime(primes,num))
        {
            n--;
        }
        num++;
    }
    return num;
}
int isPrime(int prime[],int n)
{
    int temp=n;
    int count;
    while(n>0)
    {
        int result=n%10;
        for(int i=0;i<4;i++)
        {
            if(result==prime[i])
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