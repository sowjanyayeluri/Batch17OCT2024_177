#ifndef EMP_H
#define EMP_H

typedef struct Employee
{
	int id;
	int phno;
	int sal;
	char Name[50];
	char g;
}EMP;

int getDetails(EMP *, char []);
int printDetails(EMP *);

#endif