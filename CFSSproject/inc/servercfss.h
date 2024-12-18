#ifndef SERVERCFSS_H
#define SERVERCFSS_H


int server(int,char **);
void add_client(long int , char *);
void add_details(long int,long int,char *);
int isactive(long int,int);
int isregistered(long int);
int find_usernumber(long int, char *);
void *menu(void *);
void edit_menu(int);
void register_client(int);
void client_login(int);
void forwarding_details(int);
void activate_client();
void deactivate_client();
void forward_call(int);
int is_sphno_exist(long int);
void unregister_client(int);
void set_password(int);
char *check_forwarding_type(long int);
FILE *open_file(char *,char *);
void log_changes(LogLevel, const char *);

#endif
