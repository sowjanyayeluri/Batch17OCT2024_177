/************************************************************************* 
*
*  FILE NAME    : smain.c
*
*  DESCRIPTION  : Contains the main function for the server application.
*                 Validates command-line arguments and starts the server.
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
#include <srv_call.h>

/****************************************************************************
 *       Function Name   : main
 *       Description     : Validates command-line arguments and starts the server.
 *                         Expects two arguments: PORT NO and IP Address.
 *       Returns         : 0 if successful, 1 if incorrect arguments are provided.
 ****************************************************************************/

int main(int argc,char *argv[])
{
    /* Check if the number of command-line arguments is exactly 3 */
    if(argc != 3)
    {
        printf("\nEntered less or more arguments <PORT NO> <IP Address>\n");
        return FAILURE;	  	/* Return FAILURE if arguments count is not 3 */
    }
    /* Start the server with the provided arguments (port and IP address) */
    server(argv);	/* Calling the server function with the arguments */
    return SUCCESS;

}
