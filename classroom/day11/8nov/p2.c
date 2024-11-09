#include <stdio.h>
#include <string.h>
#define CAP 20
void scanEmployees(int [],int);
void displayEmployees(int [],int);
void searchBasedonId(int [],int,int);
void displayRequired(int [],int);
void searchBasedonlName(int [],int,char []);
void searchBasedonSalary(int [],int,float);
void updateEmployee(int [],int);
struct Employee
{
	int Id;
	float Salary;
	char Tiltle[2];
	char fName[CAP];
	char lName[CAP];
}

typedef struct Employee EMP;

int main()
{
	int size;
	printf("Number of emplyees: ");
	scanf("%d",&size);
	EMP e[size];
	return 0;
}

void scanEmployee(int emp[],int n)
{
	for(int i=0;i<n;i++)
	{
		printf("Enter employee details: \n");
		printf("\nID: ");
		scanf("%d",&e[i].Id);
		printf("\nName: ");
		scanf("%d ",&e[i].Tiltle);
		scanf("%d ",&e[i].fName);
		scanf("%d",&e[i].lName);
		printf("\nSalary: ");
		scanf("%d",&e[i].eSalary);
	}
}
void displayEmployees(int emp[],int n)
{
	printf("Details of employees are: ")
	for(int i=0;i<=n;i++)
	{
		printf("Employee %d",i+1);
		printf("\nID: ",e[i].Id);
		printf("\nName: %s %s %s ",e[i].Title,e[i].fName,e[i].lName);
		printf("\nSalary: %f",e[i].Salary);
	}
}
void searchBasedonId(int emp[],int n,int id)
{
	for(int i=0;i<n;i++)
	{
		if(e[i].Id==id)
		{
			displayRequired(emp,i);

		}
	}
}
void searchBasedonName(int emp[],int n,char lname[])
{
	for(int i=0;i<n;i++)
	{
		if(strcmp(e[i].lName,lname)==0)
		{
			displayRequired(emp,i);			
		}
	}
}
void displayRequired(int emp[],int i)
{
	printf("\nID: %d",e[i].Id);
	printf("\nName: %s %s %s",e[i].Title,e[i].fName,e[i].lName);
	printf("\nSalary: %f",e[i].Salary);
	printf("\n");

}
void searchBasedonSalary(int emp[],int n,int salary)
{
	for(int i=0;i<n;i++)
	{
		if(e[i].Salary<6000 && e[i].Salary>10000)
		{
			displayRequired(emp,i);
		}
	}
}
void updateEmployee(int emp[],int i)
{
	printf("Enter the new lName: ");
	scanf("%s",e[i].lName);
	printf("Updated employee lName: ");
	displayRequired(emp,i);

}

