#include <common.h>
#include <servercfss.h>

int main(int argc, char *argv[]) 
{
    
    if(argc!=3)
    {
        printf("\nEntered less or more arguments <PORT NO> <IP Address>\n");
        return 1;
    }
    server(argc,argv);
}
