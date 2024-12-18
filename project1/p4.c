#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>

#define FILE_NAME "users.csv"
#define DETAILS_FILE "CFSS.csv"
#define MAXBUFF 1024

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client_to_file(long int, char *);
void add_details_to_file(long int,long int,char *,int);
int isactive(long int,int);
int isregistered(long int);
int find_username(long int, char *);
void *menu(void *);
void edit_menu(int);
void client_register(int);
void password(int);
void client_login(int);
void forwarding_details(int);
void activate_client(int);
void deactivate_client(int);
void unregister_client(int);

long int sphno;

int main(int argc, char *argv[]) {

    int portNo;
    char ip_addr[strlen(argv[2])];
    if(argc!=3)
    {
        printf("\nEntered less or more arguments <PORT NO> <IP Address>\n");
        return 1;
    }

    portNo = atoi(argv[1]);
    strcpy(ip_addr,argv[2]);
    int server_sock, client_sock, addr_len;
    struct sockaddr_in server_address, client_address;
    pthread_t thread_id;

    // Create master socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_sock);
        return 1;
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_addr);
    server_address.sin_port = htons(portNo);

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_sock, 5);
    printf("Server is listening on port %d...\n", portNo);

    addr_len = sizeof(client_address);
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_address, (socklen_t *)&addr_len);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, menu, (void *)&client_sock) != 0) {
            perror("Thread creation failed");
            close(client_sock);
        }
    }
    close(server_sock);
    return 0;
}

void add_client_to_file(long int username, char *password) {
    pthread_mutex_lock(&file_mutex);

    FILE *file = fopen(FILE_NAME, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    long int snumber;
	int position;
    // Read each line in the file to check if sphno exists
    while (fgets(line, sizeof(line), file)) {
        // Read the snumber (assuming it's the first field in each line)
        sscanf(line, "%ld", &snumber);
        position=ftell(file)-strlen(line);
        if (snumber == sphno)
        {
            fseek(file,position,SEEK_SET);
            fprintf(file, "%ld,%s,1\n", snumber, password);
            fclose(file);
            return;  // Found the sphno in the file, return 1
        }
    }

    fprintf(file, "%ld,%s,1\n", username, password);
    fclose(file);

    pthread_mutex_unlock(&file_mutex);
}

void add_details(long int snumber,long int rnumber,char *type,int active) {
    pthread_mutex_lock(&file_mutex);

    FILE *file = fopen(DETAILS_FILE,"a+");
    if(file == NULL)
    {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%ld,%ld,%s,%d\n",snumber,rnumber,type,active);
    fclose(file);

    pthread_mutex_unlock(&file_mutex);
}

int find_username(long int username, char *password) {
    pthread_mutex_lock(&file_mutex);

    long int fileUsername;
    char filePassword[50];
    int status;
    char line[MAXBUFF];
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Check credentials line by line
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%ld,%[^,],%d", &fileUsername, filePassword,&status);
        if (username==fileUsername && status==1)
        {
            if(strcmp(password, filePassword) == 0)
            {
            //printf("Login successful!\n");
                pthread_mutex_unlock(&file_mutex);
                fclose(file);
                return 2;
            }
            pthread_mutex_unlock(&file_mutex);
            fclose(file);
            return 1;
        }
    }

    pthread_mutex_unlock(&file_mutex);
    fclose(file);
    return -1;
}

int isactive(long int sphno,int status) {
    pthread_mutex_lock(&file_mutex);

    char line[MAXBUFF];
    long int snumber,rnumber;
    char type[MAXBUFF];
    int current_active;
    FILE *file = fopen(DETAILS_FILE,"r+");
    if(file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        // Assuming the file format: sphno,rphno,type,active
        sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rnumber, type, &current_active);

        if(snumber == sphno)
        {
            if (status==1)
            {
                if(current_active==1)
                {
                    pthread_mutex_unlock(&file_mutex);
                    fclose(file);
                    return 1;  // Return the active status for the given sphno
                }

                fseek(file, -strlen(line), SEEK_END);
                fprintf(file, "%ld,%ld,%s,%d\n", snumber, rnumber, type, status);
                pthread_mutex_unlock(&file_mutex);
                fclose(file);
                return 0;
            }
            if(current_active==0)
            {
                pthread_mutex_unlock(&file_mutex);
                fclose(file);
                return 1;  // Return the active status for the given sphn
            }
            fseek(file, -strlen(line), SEEK_CUR);
            fprintf(file,"%ld,%ld,%s,%d\n",snumber,rnumber,type,status);
        }
    }
    return -1;
}

int isregistered(long int sphno) {

    pthread_mutex_lock(&file_mutex);

    char line[MAXBUFF];
    long int snumber;
    char password[MAXBUFF];
    int current_status;
    FILE *file = fopen(FILE_NAME,"r+");
    if(file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        // Assuming the file format: sphno,rphno,type,active
        sscanf(line, "%ld,%[^,],%d", &snumber, password, &current_status);

        if(snumber == sphno)
        {
            if(current_status==0)
            {
                pthread_mutex_unlock(&file_mutex);
                fclose(file);
                return 1;  // Return the active status for the given sphno
            }
            int position=ftell(file)-strlen(line);
            fseek(file, position, SEEK_SET);
            fprintf(file, "%ld,%s,0\n", snumber, password);
            pthread_mutex_unlock(&file_mutex);
            fclose(file);
            return 0;
        }
    }
    return -1;
}

int is_rphno_exist(long int rphno) {
    pthread_mutex_lock(&file_mutex);

    char line[MAXBUFF];
    long int snumber,rnumber;
    FILE *file = fopen(DETAILS_FILE,"r");
    if(file == NULL)
    {
        perror("Error opening file");
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        // Assuming the file format: sphno,rphno,type,active
        sscanf(line, "%ld,%ld,%*[^,],%*d", &snumber, &rnumber);

        if(snumber == sphno && rnumber==rphno)
        {
            pthread_mutex_unlock(&file_mutex);
            fclose(file);
            return 1;
        }
    }
    pthread_mutex_unlock(&file_mutex);
    fclose(file);
    return -1;
}

char *check_forwarding_type(long int sphno) {
    pthread_mutex_lock(&file_mutex);

    char line[MAXBUFF];
    long int snumber,rnumber;
	int active;
	static char type[MAXBUFF];
    FILE *file = fopen(DETAILS_FILE,"r");
    if(file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    while (fgets(line, sizeof(line), file)) {
        // Assuming the file format: sphno,rphno,type,active
        sscanf(line, "%ld,%ld,%[^,],%d", &snumber,&rnumber, type,&active);

        if(snumber == sphno)
        {
            pthread_mutex_unlock(&file_mutex);
            fclose(file);
            return type;
        }
    }
    pthread_mutex_unlock(&file_mutex);
    fclose(file);
    return NULL;
}

void *menu(void *client) {
    int client_sock=*(int *)client;
    int option=0;
    send(client_sock,"1. Register\n2. Login\n3.Forward call\n",40,0);
    recv(client_sock,&option,sizeof(option),0);
    switch(option)
    {
        case 1:
            client_register(client_sock);
            edit_menu(client_sock);
            break;
        case 2:
            client_login(client_sock);
            edit_menu(client_sock);
            break;
        case 3:
           // forward_call(client_sock);
            break;
        default:
            printf("\nInvalid option");
    }
}

void client_register(int client_sock) {
    send(client_sock,"\nREGISTRATION\n\nEnter username: ",33,0);
    memset(&sphno,0,sizeof(sphno));
    recv(client_sock,&sphno,sizeof(sphno),0);
    if (find_username(sphno, "") == 1)
    {
        send(client_sock, "Username already taken.\nPlease log in.\n", 43, 0);
        printf("Username already taken.\nPlease log in.\n");
        client_login(client_sock);
        return;  // Username exists, return to login
    }

    password(client_sock);
}

void password(int client_sock) {
    char password[MAXBUFF],confPassword[MAXBUFF];
    while (1)
    {
        send(client_sock, "Enter password: ", 19, 0);
        memset(password, 0, sizeof(password));
        recv(client_sock, password, sizeof(password), 0);
        password[strcspn(password, "\n")] = '\0';  // Remove newline from password
        send(client_sock, "Confirm password: ", 21, 0);
        memset(confPassword, 0, sizeof(confPassword));
        recv(client_sock, confPassword, sizeof(confPassword), 0);
        confPassword[strcspn(confPassword, "\n")] = '\0';  // Remove newline
        if (strcmp(password, confPassword) == 0)
        {
            // Add client to the file if passwords match
            add_client_to_file(sphno, password);
            send(client_sock, "Registration successful\n", 26, 0);
            break;  // Registration successful, exit loop
        }
        else
        {
            continue;  // Retry password entry
        }
    }
}

void client_login(int client_sock) {
    char password[MAXBUFF],buffer[MAXBUFF];
    send(client_sock,"\nLOGIN\n\nEnter username: ",28,0);
    memset(&sphno,0,sizeof(sphno));
    recv(client_sock,&sphno,sizeof(sphno),0);
    if (find_username(sphno, "") == -1)
    {
        send(client_sock,"User is not registered\n", 25, 0);
        printf("\nUser is not registered\n");
        client_register(client_sock);
        return;  // Username exists, return to login
    }
    while(1)
    {
        send(client_sock,"Enter password: ",18,0);
        memset(password,0,sizeof(password));
        recv(client_sock,password,sizeof(password),0);
        password[strcspn(password, "\n")] = '\0';
        if(find_username(sphno,password)==2)
        {
            send(client_sock,"Logged in successfully.\n",26,0);
            printf("Logged in successfully.\n");
            break;
        }
        else if(find_username(sphno,password)==1)
        {
            printf("Password mismatch! Please re-enter.\n");
            continue;
        }
        else
        {
            printf("\nUser %ld is not registered\n",sphno);
            client_register(client_sock);
            break;
        }
    }
}

void edit_menu(int client_sock) {
    int choice=0;
    send(client_sock,"1.Forwarding details\n2.Activate\n3.Deactivate\n4.Unregister\n0.Exit\n",70,0);
    recv(client_sock,&choice,sizeof(choice),0);
    switch(choice)
    {
        case 1:
            forwarding_details(client_sock);
            break;
        case 2:
            activate_client(client_sock);
            break;
        case 3:
            deactivate_client(client_sock);
            break;
        case 4:
            unregister_client(client_sock);
            break;
        default:
            printf("Invalid choice\n");
    }
}

void forwarding_details(int client_sock) {
    char *type;
    type=check_forwarding_type(sphno);
    send(client_sock,type,strlen(type),0);
}

void activate_client(int client_sock) {
    int status=isactive(sphno,1);
    if(status==1)
    {
        send(client_sock,"Already activated.\n",20,0);
    }
    else
    {
        send(client_sock,"Activated\n",10,0);
    }
}

void deactivate_client(int client_sock) {
    int status=isactive(sphno,0);
    if(status==1)
    {
        send(client_sock,"Already Deactivated.\n",22,0);
    }
    else
    {
        send(client_sock,"Deactivated\n",12,0);
    }
}

void unregister_client(int client_sock) {
    int status=isregistered(sphno);
    if(status==1)
    {
        send(client_sock,"Not Registered.\n",17,0);
    }
    else
    {
        send(client_sock,"Successfully Unregistered.\n",26,0);
    }
}

