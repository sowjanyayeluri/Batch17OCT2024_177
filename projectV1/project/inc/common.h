/*************************************************************************
*  
*  FILE NAME    : common.h
*  DESCRIPTION  : Common includes and macro definitions for the project.
*                 Provides standard libraries.
*  
***************************************************************************/

#ifndef COMMON_H
#define COMMON_H

/* Standard Library Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

/* Macro Definitions*/
#define MAXBUFF 1024
#define SUCCESS 0
#define FAILURE 1

/* Global variables */
char password[MAXBUFF],conf_password[MAXBUFF];

#endif /* COMMON_H */
