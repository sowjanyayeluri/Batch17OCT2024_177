#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUFF 1024

int main()
{
	int nTC=0;
	char date1[BUFF],date2[BUFF];
	int day1,day2,month1,month2,year1,year2;
	char *token=NULL;
	scanf("%d",&nTC);
	while(nTC--)
	{
		scanf("%s%s",date1,date2);
		//tokenize and get the date,mm and year seperated
		// for each date
		token = strtok(date1,"/");
		month1 = atoi(token);
		token = strtok(NULL,"/");
		day1 = atoi(token);
		token = strtok(NULL,"/");
		year1 = atoi(token); 

		// printf("\n%d %d %d\n",day1,month1,year1);

		token = strtok(date2,"/");
		month2 = atoi(token);
		token = strtok(NULL,"/");
		day2 = atoi(token);
		token = strtok(NULL,"/");
		year2 = atoi(token); 

		// printf("\n%d %d %d\n",day2,month2,year2);
		

		if(year1<year2)
		{
			printf("E\n");
		}
		else if(year1 > year2)
		{
			printf("L\n");
		}
		else
		{
			//both years are same
			//now check for month in this block
			if(month1<month2)
			{
				printf("E\n");
			}
			else if(month1 > month2)
			{
				printf("L\n");
			}
			else
			{
				// printf("\nDates compared now %d %d\n",day1,day2);
				//both month is equal not check for day
				if(day1<day2)
				{
					printf("E\n");
				}
				else if(day1 > day2)
				{
					printf("L\n");
				}
				else
				{
					printf("S\n");	
				}
			}
		} 
		
	}
}