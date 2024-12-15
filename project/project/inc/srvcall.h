#ifndef SERVER_H
#define SERVER_H

#define FILE_NAME "../data/user1s.csv"
#define DETAILS_FILE "../data/CFSS.csv"
#define LOG_FILE "../data/logMsgs.txt"

long int sphno;
long int rphno;
int result;

typedef enum
{
	FATAL,
	INFO,
	WARNING,
	DEBUG
}LogLevel;

int server(int, char **);
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

#endif
