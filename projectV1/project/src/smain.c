#include <common.h>
#include <srvcall.h>

/****************************************************************************
 *       Function Name   : main
 *       Description     : Validates command-line arguments and starts the server.
 *                         Expects two arguments: PORT NO and IP Address.
 *       Returns         : 0 if successful, 1 if incorrect arguments are provided.
 ****************************************************************************/


int main(int argc,char *argv[])
{
	if(argc != 3)
    {
        printf("\nEntered less or more arguments <PORT NO> <IP Address>\n");
        return FAILURE;
    }
	server(argv);

}
