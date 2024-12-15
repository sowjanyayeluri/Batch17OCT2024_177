#include <common.h>
#include <server.h>

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

int server(int argc, char *argv[]) {

    int portNo;
    char ip_addr[strlen(argv[2])];

    portNo = atoi(argv[1]);
    strcpy(ip_addr,argv[2]);
    int server_sock, client_sock, addr_len;
    struct sockaddr_in server_address, client_address;
    pthread_t thread_id;

    // Create master socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket failed");
		log_changes(FATAL,"Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
		log_changes(FATAL,"SetSocket creation failed");
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
		log_changes(FATAL,"Binding Failed");
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
			log_changes(FATAL,"Accept Failed");
            continue;
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, menu, (void *)&client_sock) != 0) {
            perror("Thread creation failed");
			log_changes(FATAL,"Thread creation failed");
            close(client_sock);
        }

    }
    close(server_sock);
    return 0;
}

void add_client_to_file(long int username, char *password)
{
    pthread_mutex_lock(&file_mutex);

    FILE *file = fopen(FILE_NAME, "r+");
    if (file == NULL) {

		log_changes(FATAL,"Error opening file to read");
        perror("Error opening file");
		//log_changes(FATAL,"Error opening file to read");
        return;
    }
    char line[MAXBUFF];
    long int snumber;
	int position;
    // Read each line in the file to check if sphno exists
    while (fgets(line, sizeof(line), file)) {
        // Read the snumber (assuming it's the first field in each line)
        sscanf(line, "%ld", &snumber);
        position=ftell(file)-strlen(line);
        if (snumber == username)
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

void add_details(long int snumber,long int rnumber,char *type)
{
    pthread_mutex_lock(&file_mutex);

    FILE *file = fopen(DETAILS_FILE,"a+");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Eroor opening file to append");
        return;
    }
    fprintf(file, "%ld,%ld,%s,0\n",snumber,rnumber,type);
    fclose(file);

    pthread_mutex_unlock(&file_mutex);
}

int find_usernumber(long int username, char *password)
{
    pthread_mutex_lock(&file_mutex);
    long int fileUsername;
    char filePassword[MAXBUFF],line[MAXBUFF];
    int status;
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Error opening file to read");
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

int isactive(long int sphno,int status)
{
    pthread_mutex_lock(&file_mutex);

    char line[MAXBUFF];
    long int snumber,rnumber;
    char type[MAXBUFF];
    int current_active,position;
    FILE *file = fopen(DETAILS_FILE,"r+");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
        return 2;
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
			fseek(file,position,SEEK_SET);
			fprintf(file,"%ld,%ld,%s,%d\n",snumber,rnumber,type,status);
			fflush(file);
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
    FILE *file = fopen(FILE_NAME,"r+");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
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
	fclose(file);
    return -1;
}

int is_sphno_exits(long int sphno)
{
	pthread_mutex_lock(&file_mutex);

    char line[MAXBUFF],type[MAXBUFF];
    long int snumber;
    int active;
    FILE *file = fopen(DETAILS_FILE,"r");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
        return 0;
    }
	while(fgets(line, sizeof(line), file))
	{
		sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rphno, type, &active);
		if(snumber==sphno)
		{
			if(active==1)
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

char *check_forwarding_type(long int sphno)
{
    pthread_mutex_lock(&file_mutex);

    char line[MAXBUFF];
    long int snumber,rnumber;
	int active;
    static char type[MAXBUFF];
    FILE *file = fopen(DETAILS_FILE,"r");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
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
    int client_sock=*(int *)client;
    int option=0;
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
            forward_call(client_sock);
            break;
        default:
            printf("\nInvalid option");
			log_changes(WARNING,"Invalid option while choosing from menu");
			break;
    }
	return NULL;
}

void client_register(int client_sock)
{
    recv(client_sock,&sphno,sizeof(sphno),0);
	log_changes(INFO,"User trying to register");
    if (find_usernumber(sphno, "") == 1)
    {
        send(client_sock, "Username already taken.\nPlease log in.\n", 42, 0);
        printf("Username already taken.\nPlease log in.\n");
		log_changes(WARNING,"Username already taken while registering");
        client_login(client_sock);
        return;  // Username exists, return to login
    }
    password(client_sock);
}

void password(int client_sock)
{
    char password[MAXBUFF],confPassword[MAXBUFF];
	int attempts=0;
    while (1)
    {
		if(attempts<3)
		{
    		send(client_sock, "Enter password: ", 17, 0);
        	memset(password, 0, sizeof(password));
        	recv(client_sock, password, sizeof(password), 0);
        	memset(confPassword, 0, sizeof(confPassword));
        	recv(client_sock, confPassword, sizeof(confPassword), 0);
        	if (strcmp(password, confPassword) == 0)
        	{
            	// Add client to the file if passwords match
            	add_client_to_file(sphno, password);
            	send(client_sock, "Registration successful\n", 26, 0);
				printf("Registration successful.\n");
				log_changes(DEBUG,"User register successfully");
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
			send(client_sock,"Limit reaches.\n",17,0);
			printf("More than three attempts\n");
			log_changes(WARNING,"Max limit excceded while user entering password to register");
			break;
		}
    }
}

void client_login(int client_sock)
{
    char password[MAXBUFF];
	int attempts=0;
    recv(client_sock,&sphno,sizeof(sphno),0);
	log_changes(INFO,"User trying to login");

    if (find_usernumber(sphno, "") == -1)
    {
        send(client_sock,"User is not registered\n", 25, 0);
        printf("\nUser is not registered\n");
		log_changes(WARNING,"User trying to login without registering");
        client_register(client_sock);
        return;  // Username exists, return to login
    }
	send(client_sock,"Enter password: ",17,0);
    while(1)
    {
        memset(password,0,sizeof(password));
        recv(client_sock,password,sizeof(password),0);
		if(attempts<=3)
		{
        	if(find_usernumber(sphno,password)==2)
        	{
            	send(client_sock,"Logged in successfully.\n",26,0);
            	printf("Logged in successfully.\n");
				log_changes(DEBUG,"User logged in successfully");
            	break;
			}
        	else if(find_usernumber(sphno,password)==1)
        	{
				attempts++;
				send(client_sock,"Password mismatch.\n",21,0);
            	printf("Password mismatch! Please re-enter.\n");
				log_changes(WARNING,"Password mismatched while logging");
            	continue;
        	}
        	else
       	 	{
            	printf("\nUser %ld is not registered\n",sphno);
				log_changes(WARNING,"User trying to login without registering");
            	client_register(client_sock);
            	break;
        	}
		}
		else
		{
			printf("Maximum limit reached while logging");
			log_changes(WARNING,"Max limit reached to enter password while logging");
			break;
		}
    }
}

void edit_menu(int client_sock)
{
    int choice=0;
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
			log_changes(INFO,"User logged in and exited");
            break;
        default:
            printf("\nInvalid option\n");
			log_changes(WARNING,"User logged in and pressed invalid option to do settings");
            break;
    }
}

void forwarding_details(int client_sock)
{
    long int rphno;
    int choice;
    char type[MAXBUFF];
    recv(client_sock, &rphno, sizeof(rphno), 0);
    recv(client_sock, &choice, sizeof(choice), 0);
    if(choice == 1)
    {
        strcpy(type, "Unconditional");
    }
    else if(choice == 2)
    {
        strcpy(type, "No reply");
    }
    else
    {
        strcpy(type, "Busy");
    }
    add_details(sphno, rphno, type);
}

void activate_client()
{
	int result=0;
	result=isactive(sphno,1);
    if(result==1)
    {
        printf("Already in active state.\n");
		log_changes(INFO,"User already in active state to forward call");
        return;
    }
	else if(result==0)
	{
    	printf("Activated user.\n");
		log_changes(INFO,"User activated to forward call");
		return;
	}
	else if(result==-1)
	{
		printf("User not given forwarding details.\n");
		log_changes(WARNING,"User trying to activate without providing forwarding details");
		return;
	}
	else
	{
		return;
	}
}

void deactivate_client()
{
	int result=0;
	result=isactive(sphno,0);
    if(result==1)
    {
        printf("Already in deactive state.\n");
		log_changes(INFO,"User already in deactive state to forward call");
        return;
    }
	else if(result==0)
	{
    	printf("Deactivated user.\n");
		log_changes(INFO,"User deactivated to forward call");
		return;
	}
	else if(result==-1)
	{
		printf("User not given forwarding details.\n");
		log_changes(WARNING,"User trying to deactivate without providing forwarding details");
		return;
	}
	else
	{
		return;
	}
}

void unregister_client(int client_sock)
{
    if(isregistered(sphno))
    {
        printf("User already registered.\n");
		log_changes(INFO,"User already registered");
        return;
    }
    printf("Unregistered successfully.\n");
	log_changes(DEBUG,"Unregistered successfull");
}

void forward_call(int client_sock)
{
	int result;
	char type[MAXBUFF];
	recv(client_sock,&sphno,sizeof(sphno),0);
	result=is_sphno_exits(sphno);
	printf("rphno after is_sphno_exits: %ld\n", rphno);
	strcpy(type,check_forwarding_type(sphno));
	if(result==2)
	{
		if(strcmp(type,"Unconditional")==0)
		{
			printf("Call forwarding to %ld.\n",rphno);
			log_changes(DEBUG,"Call forwarding...");
		}
		else if(strcmp(type,"No reply")==0)
		{
			printf("Waiting to lift the call.\n");
			log_changes(INFO,"Waiting to lift the call");
			sleep(5);
			printf("Call forwarding to %ld.\n",rphno);
			log_changes(DEBUG,"Call forwarding....");
		}
		else if(strcmp(type,"Busy")==0)
		{
			printf("User is busy.\n");
			log_changes(INFO,"User is busy to lift the call");
			printf("Call forwarding to %ld.\n",rphno);
			log_changes(DEBUG,"call forwarding....");
		}
		else
		{
			printf("Not a valid type.\n");
			log_changes(WARNING,"Not given valid type to forward the call");
		}
	}
	else if(result==1)
	{
		printf("User is not active.\n");
		log_changes(INFO,"User is not active to forward the call");
	}
	else if(result==-1)
	{
		printf("%ld not given forwarding details.\n",sphno);
		log_changes(INFO,"User not provided forwarding deatils");
	}
	else
	{
		return;
	}
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

