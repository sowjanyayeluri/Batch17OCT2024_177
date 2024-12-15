#ifndef SERVER_H
#define SERVER_H

#define FILE_NAME "./data/user1s.csv"
#define DETAILS_FILE "./data/CFSS.csv"
#define LOG_FILE "./data/logMsgs.txt"

long int sphno;
long int rphno;

typedef enum
{
	FATAL,
	INFO,
	WARNING,
	DEBUG
}LogLevel;

int server(int,char **);
void add_client_to_file(long int , char *);
void add_details_to_file(long int,long int,char *,int);
int isactive(long int,int);
int isregistered(long int);
int is_sphno_exits(long int);
char *check_forwarding_type(long int);
int find_usernumber(long int, char *);
void *menu(void *);
void edit_menu(int);
void client_register(int);
void password(int);
void client_login(int);
void forwarding_details(int);
void activate_client();
void deactivate_client();
void unregister_client(int);
void forward_call(int);

void log_changes(LogLevel level, const char *function_name);

#endif
