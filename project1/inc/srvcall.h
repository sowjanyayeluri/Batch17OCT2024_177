#ifndef SERVER_H
#define SERVER_H

#define FILE_NAME "/home2/user62/project/data/users.csv"
#define DETAILS_FILE "/home2/user62/project/data/CFSS.csv"

long int sphno;
long int rphno;

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

#endif
