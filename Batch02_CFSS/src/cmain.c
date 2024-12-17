/************************************************************************* 
*
*  FILE NAME    : cmain.c
*
*  DESCRIPTION  : Contains main function for the client application.
*                 It validates the command-line arguments and starts the client
*
*  DATE          NAME
*
*  10-DEC-24    Batch02
*
**************************************************************************/


/*****************************************************************************
*                       Header Files
******************************************************************************/
#include <common.h>
#include <clnt_call.h>

/****************************************************************************
 *  Function Name   : main
 *  Description     : Validates command-line arguments and starts the client.
 *                    It expects exactly two arguments: the port number and IP address.
 *  Returns         : 0 if successful, 1 if incorrect arguments are provided.
 ****************************************************************************/

int main(int argc,char *argv[])
{
    /* Check if the number of command-line arguments is exactly 3 */
    if(argc != 3)
    {
        printf("\nEntered less or more arguments <PORT NO> <IP Address>\n");
        return FAILURE;	   /* Return FAILURE if arguments count is not 3 */
    }
    /* Call the client function with the provided arguments (port and IP address) */
    client(argv);	/* Start the client with the given command-line arguments */
    return SUCCESS;

}
