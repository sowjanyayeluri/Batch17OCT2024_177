/*
	FILE NAME	: Question5.c
	DESCRIPTION : printing alphabet and numeric triangle.
	DATE		NAME
	7-NOV-24	Sowjanya
*/


A5. Write a c program to print alphabet triangle.

// alphabet triangle

#include <stdio.h>
void alpha_pattern(int );
void spaces(int,int);
int main()
{
    int input;
    scanf("%d",&input);
    alpha_pattern(input);
    return 0;
}
void alpha_pattern(int n)
{
    int b;
    for(int i=0;i<n;i++)
    {
        spaces(n,i);
        char ch='A';
        b = (2*i+1)/2;
        for(int j=1;j<=(2*i+1);j++)
        {
            
            printf("%c",ch);
            if(j<=b)
                ch++;
            else
                ch--;
        }
        spaces(n,i);
        printf("\n");
        
    }
}
void spaces(int n, int i)
{
    for(int j=0;j<=n-i-1;j++)
    {
            
        printf(" ");
    }
}

//numeric triangle

#include <stdio.h>
void numeric_pattern(int );
void spaces(int,int);
int main()
{
    int input;
    scanf("%d",&input);
    alpha_pattern(input);
    return 0;
}
void numeric_pattern(int n)
{
    int b;
    for(int i=0;i<n;i++)
    {
        spaces(n,i);
        int num=1;
        b = (2*i+1)/2;
        for(int j=1;j<=(2*i+1);j++)
        {
            
            printf("%d",num);
            if(j<=b)
                num++;
            else
                num--;
        }
        spaces(n,i);
        printf("\n");
        
    }
}
void spaces(int n, int i)
{
    for(int j=0;j<=n-i-1;j++)
    {
            
        printf(" ");
    }
}