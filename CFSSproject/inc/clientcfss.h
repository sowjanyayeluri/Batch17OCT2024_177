#ifndef CLIENTCFSS_H
#define CLIENTCFSS_H

int client(int ,char **);
void menu(int , int);
int validate_phno(long int,char *);
void regist(int);
void login(int );
void edit_menu(int );
void give_details(int );
void forward_call(int );
long int phno;

char buffer[MAXBUFF],confPassword[MAXBUFF],password[MAXBUFF];
#endif
