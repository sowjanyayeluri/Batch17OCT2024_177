/*************************************************************************
*  
*  FILE NAME    : srvcall.h
*  DESCRIPTION  : Header file for server-side functions. Defines macros, global variables,
*				  enum for log levels, and function declarations related to client
*				  management and call forwarding.
***************************************************************************/

#ifndef SRVCALL_H
#define SRVCALL_H

/* Macros of file Paths for Data and Logs*/
#define FILE_NAME "../data/users.csv"
#define DETAILS_FILE "../data/CFSS.csv"
#define LOG_FILE "../data/logMsgs.txt"

/* Global Variables*/
long int sphno;
long int rphno;
int result;

/* Enum for Log Levels*/
typedef enum
{
	FATAL,
	INFO,
	WARNING,
	DEBUG
}LogLevel;

/* Function Declarations*/
int server(char **);
void add_client_details(long int , char *);
void add_cfss_details(long int, long int, char *);
int is_active(long int, int);
int is_registered(long int);
int is_sphno_exits(long int);
char *check_forwarding_type(long int);
int find_usernumber(long int, char *);
void *main_menu(void *);
void edit_menu(int);
void client_register(int);
void set_password(int);
void client_login(int);
void forwarding_details(int);
void activate_client();
void deactivate_client();
void unregister_client();
void forward_call(int);
void log_changes(LogLevel, const char *);

#endif /* SRVCALL_H */
