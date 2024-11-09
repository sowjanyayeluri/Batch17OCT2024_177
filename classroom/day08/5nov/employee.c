#include <stdio.h>
int main()
{
	int SlNo;
	int ID;
	char NAME[20];
	char GENDER;
	char ADDRESS[20];
	long PHNO;
	int SALARY;
	char DESG[20];
	printf("\nEnter slno: ");
	scanf("%d",&SlNo);
	printf("\nEnter id: ");
	scanf("%d",&ID);
	printf("\nEnter name: ");
	scanf("%s",NAME);
	printf("\nEnter gender: ");
	scanf(" ");
	scanf("%c",&GENDER);
	printf("\nEnter address: ");
	scanf("%s",ADDRESS);
	printf("\nEnter phno: ");
	scanf("%ld",&PHNO);
	printf("\nEnter salary: ");
	scanf("%d",&SALARY);
	printf("\nEnter desg: ");
	scanf("%s",DESG);
	printf("\n %-3s| %-3s | %-10s | %-s | %-10s | %-10s | %-7s | %-10s","SlNo","ID","NAME","GENDER","ADDRESS","PHNO","SALARY","DESG");
	printf("\n %03d | %03d | %-10s | %c\t | %-10s | %10ld | %7d | %-10s",SlNo,ID,NAME,GENDER,ADDRESS,PHNO,SALARY,DESG);
	
}
