/*
	FILE NAME	: Question4.c
	DESCRIPTION : rotating array every kth element
	DATE		NAME
	7-NOV-24	Sowjanya
*/


A4. rotate array every kth element

#include <stdio.h>
#define CAP 9
void rotateArray(int [],int,int);
void sArrReverse(int [],int,int);
int main()
{
    int arr[CAP];
    int arrValue;
    int key;
    for(int i=0;i<CAP;i++)
    {
        scanf("%d",&arrValue);
        arr[i]=arrValue;
    }
    scanf("%d",&key);
    rotateArray(arr,CAP,key);
    for(int i=0;i<CAP;i++)
    {
        printf("%d ",arr[i]);
    }
    return 0;
}
void rotateArray(int a[],int size,int key)
{
    int i;
    int end;
    for(i=0;i<size;)
    {
        if(i+key-1<size)
            end = i+key-1;
        else
            size-1;
        sArrReverse(a,i,end);
        i+=key;
    }
}
void sArrReverse(int arr[],int start,int end)
{
    int temp;
    while(start<end)
    {
        temp=arr[end];
        arr[end]=arr[start];
        arr[start]=temp;
        start++,end--;
    }