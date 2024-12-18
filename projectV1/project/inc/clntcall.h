/*************************************************************************
*  
*  FILE NAME    : client.h
*  DESCRIPTION  : Header file for client-side functions and data structures.
*                 Defines variables for managing client data and function 
*                 declarations related to client actions like login, registration, and call forwarding.
*
***************************************************************************/

#ifndef CLNTCALL_H
#define CLNTCALL_H

/* Global Variables*/
char buffer[MAXBUFF];
long int phno; /* Client's phone number*/

/* Function Declarations*/
int client(char **);
void main_menu(int, int);
void edit_menu(int);
void login(int);
void regist(int); // Register a new client
void give_details(int);
void forward_call(int);

#endif /* CLNTCALL_H */
