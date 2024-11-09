#include <stdio.h>
#include <string.h>
int main()
{
    char s1[400],s2[40];
    char *ptr=NULL;
    printf("\nEnthe the line with delimiter:\n");
    scanf("%[^\n]s",s1);
    printf("%s",s1);
    ptr = strtok(s1,",");
    printf("\nptr=%s",ptr);
    printf("\n\n");
    return 0;
}
