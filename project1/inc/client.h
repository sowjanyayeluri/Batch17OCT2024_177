#ifndef CLIENT_H
#define CLIENT_H

char buffer[MAXBUFF];
char password[MAXBUFF],confPassword[MAXBUFF];
long int phno;

int client(int,char **);
void menu(int, int);
void edit_menu(int);
void login(int);
void regist(int);
void give_details(int);
void forward_call(int);

#endif
