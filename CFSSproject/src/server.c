#include <common.h>
#include <servercfss.h>

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

long int sphno;  // Define the global variables
long int rphno;


int server(int argc, char *argv[]) 
{
	int portNo,opt;
    char ip_addr[strlen(argv[2])];
	int server_sock, client_sock, addr_len;
    struct sockaddr_in server_address, client_address;
    pthread_t thread_id;
 
    portNo = atoi(argv[1]);
    strcpy(ip_addr,argv[2]);
 
    // Create master socket
    server_sock = socket(AF_INET, SOCK_STREAM,0);

    if (server_sock < 0) 
	{
		perror("Socket failed");
        exit(EXIT_FAILURE);
    }
 
    opt = 1;
    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
	{
        perror("setsockopt failed");
        close(server_sock);
        return 0;
    }
 
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(ip_addr);
    server_address.sin_port = htons(portNo);
 
    // Bind the socket
    if(bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) 
	{
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
 
    // Listen for incoming connections
    listen(server_sock, 5);
    printf("Server is listening on port %d...\n", portNo);
 
    addr_len = sizeof(client_address);
    while (1) 
	{
        client_sock = accept(server_sock, (struct sockaddr *)&client_address, (socklen_t *)&addr_len);
        if (client_sock < 0) 
		{
            perror("Accept failed");
            continue;
        }
 
        printf("New connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
		// Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, menu, (void *)&client_sock) !=0 ) 
		{
            perror("Thread creation failed");
            close(client_sock);
        }
 
    }
    close(server_sock);
    return 0;
}

FILE *open_file(char *file_name, char *mode)
{
	FILE *file = open_file(CREDENTIALS_FILE, "a");
    if (file == NULL) 
	{
		perror("Error opening file");
    }
	return file;
}

 
void add_client(long int username, char *password)
{
	pthread_mutex_lock(&file_mutex);

   	FILE *file = open_file(CREDENTIALS_FILE, "a");
   	if (file == NULL)
   	{
       	pthread_mutex_unlock(&file_mutex);
       	return;
   	}
   	char line[MAXBUFF];
   	long int snumber;
   	int position;
   	// Read each line in the file to check if sphno exists
   	while (fgets(line, sizeof(line), file)) 
	{
       // Read the snumber (assuming it's the first field in each line)
	   sscanf(line, "%ld", &snumber);
       position=ftell(file)-strlen(line);
       if (snumber == sphno)
       {
             fseek(file,position,SEEK_SET);
             fprintf(file, "%ld,%s,\n", snumber, password);
             fclose(file);
             return ;  // Found the sphno in the file, return 
         }
     }

     fprintf(file, "%ld,%s,\n", username, password);
     fclose(file);

     pthread_mutex_unlock(&file_mutex);
 }

 void add_details(long int snumber,long int rnumber,char *type)
{
     pthread_mutex_lock(&file_mutex);

     FILE *file = open_file(DETAILS_FILE,"a+");
     if(file == NULL)
     {
         pthread_mutex_unlock(&file_mutex);
         return;
     }

     fprintf(file, "%ld,%ld,%s,0\n",snumber,rnumber,type);
     fclose(file);

     pthread_mutex_unlock(&file_mutex);
 }

 int find_user(long int username, char *password)
 {
     pthread_mutex_lock(&file_mutex);

     long int fileUsername;
     char filePassword[MAXBUFF];
     int status;
     char line[MAXBUFF];
     FILE *file = open_file(CREDENTIALS_FILE, "r");
     if (file == NULL) {
         pthread_mutex_unlock(&file_mutex);
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

 int isactive(long int sphno,int status)
 {
     pthread_mutex_lock(&file_mutex);

     char line[MAXBUFF];
     long int snumber,rnumber;
     char type[MAXBUFF];
     int current_active,position;
     FILE *file = open_file(DETAILS_FILE,"r+");
     if(file == NULL)
     {
	 pthread_mutex_unlock(&file_mutex);
         return -1;
     }

     while (fgets(line, sizeof(line), file)) {
         // Assuming the file format: sphno,rphno,type,active
         sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rnumber, type, &current_active);
	 position=ftell(file)-strlen(line);
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

                 fseek(file, position, SEEK_SET);
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
             fseek(file, position, SEEK_SET);
             fprintf(file,"%ld,%ld,%s,%d\n",snumber,rnumber,type,status);
	     fclose(file);
	     return 0;
         }

     }
     fclose(file);
     return -1;
 }

 int isregistered(long int sphno)
 {

     pthread_mutex_lock(&file_mutex);

     char line[MAXBUFF];
     long int snumber;
     char password[MAXBUFF];
     int current_status,position;
     FILE *file = open_file(CREDENTIALS_FILE,"r+");
     if(file == NULL)
     {
	 pthread_mutex_unlock(&file_mutex);
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
             position=ftell(file)-strlen(line);
             fseek(file, position, SEEK_SET);
             fprintf(file, "%ld,%s,0\n", snumber, password);
             pthread_mutex_unlock(&file_mutex);
             fclose(file);
             return 0;
         }

     }
     return -1;
 }

 int is_sphno_exist(long int sphno)
 {
     pthread_mutex_lock(&file_mutex);

     char line[MAXBUFF],type[MAXBUFF];
     long int snumber,rphno;
     int active;
     FILE *file = open_file(DETAILS_FILE,"r");
     if(file == NULL)
     {
         pthread_mutex_unlock(&file_mutex);
         return 0;
     }

     while (fgets(line, sizeof(line), file)) {
         // Assuming the file format: sphno,rphno,type,active
         sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rphno, type,&active );

         if(snumber == sphno)
         {
		 	if(active==1)
			{
             	pthread_mutex_unlock(&file_mutex);
             	fclose(file);
             	return 2;
			}
			return 1;
         }
     }
     pthread_mutex_unlock(&file_mutex);
     fclose(file);
     return -1;

 }
 char *check_forwarding_type(long int sphno)
 {
     pthread_mutex_lock(&file_mutex);

     char line[MAXBUFF];
     long int snumber,rnumber;
     int active;
     static char type[MAXBUFF];
     FILE *file = open_file(DETAILS_FILE,"r");
     if(file == NULL)
     {
         perror("Error opening file");
         return NULL;
     }

     while (fgets(line, sizeof(line), file)) {
         // Assuming the file format: sphno,rphno,type,active
         sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rnumber, type, &active);

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

 void *menu(void *client)
 {
 	printf("h");
     int client_sock=*(int *)client;
     int option=0;
     recv(client_sock,&option,sizeof(option),0);
     switch(option)
     {
         case 1:
                 register_client(client_sock);
                 edit_menu(client_sock);
                 break;
         case 2:
                 client_login(client_sock);
                 edit_menu(client_sock);
                 break;
         case 3:
                 forward_call(client_sock);
                 break;
         default:
                 printf("\nInvalid option");
     }
 }

// Function to make client Registration with Usernumber and password.

void register_client(int client_sock)
{
    recv(client_sock,&sphno,sizeof(sphno),0); // to get Usernumber form client.
	log_changes(INFO,"User trying to register");
    if (find_user(sphno, "") == 1)
    {
		log_changes(WARNING,"Trying to register, but user already existing");
        send(client_sock, "Username already taken.\nPlease log in.\n",43,0);
        printf("Username already taken.\nPlease log in.\n");
        client_login(client_sock);
        return;  // Username exists, return to login
    }
    set_password(client_sock);

}

// Function to set password for user. And checks whether entered password and confirmed password are matched. And updates UserCredentails.txt file.

void set_password(int client_sock)
{
    char password[MAXBUFF],confPassword[MAXBUFF];
	int attempts = 0; // For number of attempts to enter password
    while (1)
    {
        send(client_sock, "Enter password: ",17,0);
        memset(password,0, sizeof(password));
        recv(client_sock, password, sizeof(password),0);
        password[strcspn(password, "\n")] = '\0';  // Remove newline from password
		memset(confPassword,0, sizeof(confPassword));
        recv(client_sock, confPassword, sizeof(confPassword),0);
        confPassword[strcspn(confPassword, "\n")] = '\0';  // Remove newline
		if (attempts<=3)
		{
        	if (strcmp(password, confPassword) == 0)
         	{
            	 // Add client to the file if passwords match
             	add_client(sphno, password);
            	send(client_sock, "Registration successful\n",26,0);
				printf("Registration successful\n");

				log_changes(DEBUG,"User successfully registered\n");
           		break;  // Registration successful, exit loop
         	}
         	else
         	{
				attempts++;
            	continue;  // Retry password entry
         	}
		}
		else
		{
			log_changes(WARNING,"Maximum attempts exceeded while registering");
			printf("More than three attempts\n");
			break;
		}
     }
}

//Function to login by user with userNumber and password. And checks this deatils in UserCredentials.txt file
void client_login(int client_sock)
{
   char password[MAXBUFF],buffer[MAXBUFF];
	int attempts =0; // for number of attempts to enter password
    recv(client_sock,&sphno,sizeof(sphno),0);//To get userNumber from client
    if (find_user(sphno, "") == -1)
    {
		send(client_sock,"User is not registered\n",25,0 );
		log_changes(WARNING,"Failed to login because User is not registered");
        printf("\nUser is not registered\n");
        register_client(client_sock);
        return;  // Username exists, return to login
    }

    while(1)
    {
		send(client_sock,"Enter password: ",17,0);
        memset(password,0,sizeof(password));
        recv(client_sock,password,sizeof(password),0);
        password[strcspn(password, "\n")] = '\0';
		if(attempts<=3)
		{
        	if(find_user(sphno,password)==2)
        	{
            	send(client_sock,"Logged in successfully.\n",26,0);
				log_changes(DEBUG,"User logged in successfully");
            	printf("Logged in successfully.\n");
            	break;
        	}
        	else
        	{
				log_changes(WARNING,"Password mismatch while logging");
				printf("Password mismatch! Please re-enter.\n");
        	}
		}
		else
		{
			log_changes(WARNING,"Maximum limit reached while logging");
			printf("Maximum limit reached while logging");
			break;
		}
     }
}

 void edit_menu(int client_sock)
 {
     int choice;
     recv(client_sock,&choice,sizeof(choice),0);
     switch(choice)
     {
         case 1:
                 forwarding_details(client_sock);
                 break;
         case 2:
                 activate_client();
                 break;
         case 3:
                 deactivate_client();
                 break;
         case 4:
                 unregister_client(client_sock);
                 break;
         case 0:
                 printf("\nExiting...\n");
                 break;
         default:
                 printf("\nInvalid choice\n");
                 break;
     }

 }

 void forwarding_details(int client_sock)
 {
     long int rphno;
     int choice;
     char type[MAXBUFF];
     recv(client_sock,&rphno,sizeof(rphno),0);
     recv(client_sock,&choice,sizeof(choice),0);
     if(choice==1)
     {
	   strcpy(type,"Unconditional");
     }
     else if(choice==2)
     {
	   strcpy(type,"No reply");
     }
     else
     {
	   strcpy(type,"Busy");
     }
     add_details(sphno,rphno,type);

 }

void activate_client()
{
    if(isactive(sphno,1))
    {
        printf("Already in active state.\n");
	return;
    }
    printf("Activated user.\n");

}

void deactivate_client()
{
    if(isactive(sphno,0))
    {
        printf("Already in deactive state.\n");
	return;
    }
    printf("Deactivated user.\n");

}

void unregister_client(int client_sock)
{
	if(isregistered(sphno))
	{
		printf("User already registered.\n");
		return;
	}
	printf("Unregistered successfully.\n");
}

 void forward_call(int client_sock)
 {
     recv(client_sock,&sphno,sizeof(sphno),0);
     if(is_sphno_exist(sphno)==2)
     {
         if(strcmp(check_forwarding_type(sphno),"Unconditional"))
         {
             printf("Call forwarding to %ld\n",rphno);
         }
         else if(strcmp(check_forwarding_type(sphno),"No reply"))
         {
             printf("Waiting to lift the call\n");
             sleep(2);
             printf("Call forwarding to %ld\n",rphno);
         }
         else if(strcmp(check_forwarding_type(sphno),"Busy"))
         {
             printf("User is busy\n");
             printf("Call forwarding to %ld\n",rphno);
         }
         else
         {
             printf("Not valid type\n");
         }
     }
     else if(is_sphno_exist(sphno)==1)
     {
         printf("User is not active.\n");
     }
     else
		printf("%ld is not registered.\n",sphno);
 }

// To update log file with at what time and date, who is trying to 
void log_changes(LogLevel level, const char *function_name) 
{
	FILE *logfile;
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);  // Get the current time
    localTime = localtime(&currentTime);  // Convert it to local time

    // Open the log file in append mode (creates the file if it doesn't exist)
    logfile = fopen(LOG_FILE, "a");

    // Check if the file opened successfully
    if (logfile == NULL) {
        printf("Error opening the log file.\n");
        return;
    }

    const char *level_str;
    switch(level)
    {
        case FATAL:
            level_str = "FATAL";
            break;
        case INFO:
            level_str = "INFO";
            break;
        case WARNING:
            level_str = "WARNING";
            break;
        case DEBUG:
            level_str = "DEBUG";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }
 // Log the current date, time, and the function where changes are made
    fprintf(logfile, "[%s]\nDate: %02d/%02d/%04d Time: %02d:%02d:%02d userNumber: %ld - %s \n\n",level_str,localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,localTime->tm_hour, localTime->tm_min, localTime->tm_sec,sphno,function_name);
// Close the log file
    fclose(logfile);
}

