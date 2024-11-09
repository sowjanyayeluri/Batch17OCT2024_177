#include <stdio.h>
int main()
{
	int n;
	printf("Enter the n value: ");
	scanf("%d",&n);
	for(int i=1;i<=n;i++)
	{
		int result= (i*i)-1;
		if(i==n)
			printf("%d",result);
		else
			printf("%d, ",result);
	}
	printf("\n\n");
	return 0;
}
