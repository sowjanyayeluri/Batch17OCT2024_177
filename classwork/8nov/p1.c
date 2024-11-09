#include <stdio.h>
#include <string.h>
struct Employee
{
    char eName[20];
    char eGender;
    char eAddress[20];
    int eId;
    float eSal;
};

typedef struct Employee EMP;

int main()
{
    EMP e1;
    e1.eId=101;
    e1.eSal=10000.1;
    strcpy(e1.eName,"Sowji");
    strcpy(e1.eAddress,"Vallabhi");
    e1.eGender='F';
    printf("\nEmployee details: \n");
    printf("\nID: %d",e1.eId);
    printf("\nName: %s",e1.eName);
    printf("\nGender: %c",e1.eGender);
    printf("\nAddress: %s",e1.eAddress);
    printf("\nSalary: %f/n",e1.eSal);
    return 0;
}
