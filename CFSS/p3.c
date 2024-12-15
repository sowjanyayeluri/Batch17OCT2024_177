#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXBUFF 1024

void menu(int, int);
void edit_menu(int);
void login(int);
void regist(int);
void give_details(int);
char buffer[MAXBUFF];
char password[MAXBUFF],confPassword[MAXBUFF];
long int phno;

int main(int argc,char *argv[]) {
    int portNo;
    char ip_addr[strlen(argv[2])];
    if(argc != 3)
    {
        printf("\nEntered less or more arguments <PORT NO> <IP Address>\n");
        return 1;
    }

    portNo = atoi(argv[1]);
    strcpy(ip_addr,argv[2]);

    int sock;
    struct sockaddr_in server_addr;
    int option;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNo);
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }
    
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    scanf("%d", &option);
    getchar();
    send(sock, &option, sizeof(option), 0);
    menu(sock, option);

    close(sock);
    return 0;
}

void menu(int sock, int option)
{
    switch(option)
    {
        case 1:
            regist(sock);
            edit_menu(sock);
            getchar();
            break;
        case 2:
            login(sock);
            edit_menu(sock);
            getchar();
            break;
        case 3:
            //forward_call(sock);
            break;
        default:
            printf("Enter valid");
    }
}

void regist(int sock)
{
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    while(1)
    {
        if(scanf("%ld", &phno) == 1)
        {
            while(getchar() != '\n');
            if(phno >= 100000000 && phno <= 9999999999)
            {
                send(sock, &phno, sizeof(phno), 0);
                break;
            }
            else
            {
                printf("\nEnter 10 digits\n");
            }
        }
        else
        {
            printf("\nEnter only long integers\n");
            while(getchar() != '\n');
        }
    }
    recv(sock, buffer, sizeof(buffer), 0);

    if(strcmp(buffer, "Username already taken.\nPlease log in.\n") == 0)
    {
        login(sock);
        getchar();
        return;
    }
    if (strcmp(buffer, "Enter password: ") == 0)
    {
        while(1)
        {
            printf("%s", buffer);
            fgets(password, MAXBUFF, stdin);
            password[strlen(password)-1] = '\0';
            send(sock, password, sizeof(password), 0);
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s", buffer);
            fgets(confPassword, MAXBUFF, stdin);
            confPassword[strlen(confPassword) - 1] = '\0';

            send(sock, confPassword, sizeof(confPassword), 0);
            recv(sock, buffer, sizeof(buffer), 0);
            if (strcmp(buffer, "Registration successful\n") == 0)
            {
                printf("%s", buffer);
                break;
            }
            else
            {
                printf("\nPassword mismatch! Please re-enter.\n");
                continue;
            }
        }
    }
}

void login(int sock)
{
    printf("Start logging in:");
    getchar();
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    while(1)
    {
        if(scanf("%ld", &phno) == 1)
        {
            while(getchar() != '\n');
            if(phno >= 100000000 && phno <= 9999999999)
            {
                send(sock, &phno, sizeof(phno), 0);
                break;
            }
            else
            {
                printf("\nEnter 10 digits\n");
            }
        }
        else
        {
            printf("\nEnter only long integers\n");
            while(getchar() != '\n');
        }
    }
    recv(sock, buffer, sizeof(buffer), 0);
    if(strcmp(buffer, "User is not registered\n") == 0)
    {
        regist(sock);
        return;
    }
    if(strcmp(buffer, "Enter password: ") == 0)
    {
        while(1)
        {
            printf("%s", buffer);
            fgets(password, MAXBUFF, stdin);
            password[strlen(password)-1] = '\0';
            send(sock, password, sizeof(password), 0);
            recv(sock, buffer, sizeof(buffer), 0);
            if(strcmp(buffer, "Logged in successfully.\n") == 0)
            {
                break;
            }
            else
            {
                continue;
            }
        }
    }
}

void edit_menu(int sock)
{
    int option;
    getchar();
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
    printf("Enter the choice: ");
    scanf("%d", &option);
    getchar();
    send(sock, &option, sizeof(option), 0);
    switch(option)
    {
        case 1:
            give_details(sock);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
    }
}

void give_details(int sock)
{
    long int rphno;
    int type_choice;
    while(1)
    {
        printf("Enter requested client number: ");
        if(scanf("%ld", &rphno) == 1)
        {
            while(getchar() != '\n');
            if(rphno >= 100000000 && rphno <= 9999999999)
            {
                send(sock, &rphno, sizeof(rphno), 0);
                break;
            }
            else
            {
                printf("\nEnter 10 digits\n");
            }
        }
        else
        {
            printf("\nEnter only long integers\n");
            while(getchar() != '\n');
        }
    }
    printf("\nForwarding types\n");
    printf("1.Unconditional\n2.No reply\n3.Busy\n");
    while(1)
    {
        printf("\nEnter the choice: ");
        scanf("%d", &type_choice);
        if(type_choice < 4 && type_choice > 0)
        {
            send(sock, &type_choice, sizeof(type_choice), 0);
            break;
        }
        printf("\nEnter between 1 to 3");
    }
}

void forward_call(int sock)
{
	long int rphno;
    while(1)
    {
        printf("Enter requested client number: ");
        if(scanf("%ld", &rphno) == 1)
        {
            while(getchar() != '\n');
            if(rphno >= 100000000 && rphno <= 9999999999)
            {
                send(sock, &rphno, sizeof(rphno), 0);
                break;
            }
            else
            {
                printf("\nEnter 10 digits\n");
            }
        }
        else
        {
            printf("\nEnter only long integers\n");
            while(getchar() != '\n');
        }
    }
}
