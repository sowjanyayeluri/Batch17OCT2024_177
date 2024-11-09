#include <stdio.h>
#include <string.h>
#define SIZE 20
void reverseString(char []);
int main()
{
    char input[SIZE];
    scanf("%s",input);
    reverseString(input);
    printf("Reversed string: %s",input);
   return 0;
}
void reverseString(char string[])
{
    int length=strlen(string);
    int i=0;
    int mid=0;
    if(length%2==0)
        mid=length/2;
    else
        mid=(length/2)+1;
    while(i<mid)
    {
        char temp=string[length-i-1]; 
        string[length-i-1]=string[i];
        string[i]=temp;
        i++;
    }
}


