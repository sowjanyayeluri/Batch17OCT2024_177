#include <stdio.h>
int main()
{
	int num;
	printf("Enter the num: ");
	scanf("%d",&num);
	for(;;)
	{
		printf("statatements in For loop");
		if(num<5)
		{
			printf("num is less than 5");
		}
		else
		{
			break;
		}
		num++;
	}

}
