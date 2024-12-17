/************************************************************************* 
*
*  FILE NAME    : srv_call.c
*
*  DESCRIPTION  : Contains functions to handle server-side operations
*                 such as managing user registration, login, and call forwarding.
*
*  DATE          NAME
*
*  10-DEC-24    Batch02
*
**************************************************************************/


/*****************************************************************************
*                       Header Files
******************************************************************************/
#include <common.h>
#include <srv_call.h>

/* Mutex for file access synchronization to prevent race conditions */
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

/****************************************************************************
*       Function Name   : server
*       Description     : Initializes the server, listens for client connections, 
*                      and threads to handle each client. 
*       Returns         : Returns SUCCESS (0) on success and FAILURE (1) on failure.
****************************************************************************/

int server(char *argv[]) 
{

    int port_no;		/* Port number for server to listen on */
    char ip_addr[strlen(argv[2])];		/* IP address to bind the server to */

    port_no = atoi(argv[1]);		/* Convert port argument to integer */
    strcpy(ip_addr,argv[2]);		/* Copy IP address argument */
    int server_sock, client_sock, addr_len;
    struct sockaddr_in server_address, client_address;
    pthread_t thread_id;		/* Thread ID for client handler */

	/* Create master socket */
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket failed");
		log_changes(FATAL,"Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
	/* Set socket options for reuse of address */
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
		log_changes(FATAL,"setsockopt creation failed");
        close(server_sock);
        return FAILURE;
    }

    memset(&server_address, 0, sizeof(server_address));		/* Zero out server address */
    server_address.sin_family = AF_INET;		/* IPv4 protocol */
    server_address.sin_addr.s_addr = inet_addr(ip_addr);	/* Set server IP address */
    server_address.sin_port = htons(port_no);	/* Set server port */

    /* Bind the socket to the address and port */
    if (bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
		log_changes(FATAL,"Binding Failed");
        exit(EXIT_FAILURE);
    }

    /* Listen for incoming client connections */
    listen(server_sock, 5);
    printf("Server is listening on port %d...\n", port_no);

    addr_len = sizeof(client_address);
    while (1) 
	{
		/* Accept a new connection */
        client_sock = accept(server_sock, (struct sockaddr *)&client_address, (socklen_t *)&addr_len);
        if (client_sock < 0) {
            perror("Accept failed");
			log_changes(FATAL,"Accept Failed");
            continue;
        }

        printf("New connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        /* Create a new thread to handle the client */
        if (pthread_create(&thread_id, NULL, main_menu, (void *)&client_sock) != 0) {
            perror("Thread creation failed");
			log_changes(FATAL,"Thread creation failed");
            close(client_sock);
        }
		// Make sure to join the thread so that resources are cleaned up.
		pthread_join(thread_id, NULL);
    }
    close(server_sock);
    return SUCCESS;
}

/****************************************************************************
*       Function Name   : add_cliennt_details
*       Description     : Writing the details of users to a file.
*       Returns         : Nothing.
****************************************************************************/

void add_client_details(long int usernumber, char *password)
{
    pthread_mutex_lock(&file_mutex);	/* Lock file for thread safety */

    FILE *file = fopen(FILE_NAME, "r+");
    if (file == NULL) 
	{
		file = fopen(FILE_NAME, "a+");
        if (file == NULL) {
            perror("Error opening file");
            pthread_mutex_unlock(&file_mutex);		/* Unlock mutex before returning */
			log_changes(FATAL,"Error opening file to read");
            return;
		}
    }
    char line[MAXBUFF];
    long int snumber=0;
	int position=0;
    /* Read each line in the file to check if user number exists */
    while (fgets(line, sizeof(line), file)) 
	{
        sscanf(line, "%ld", &snumber);		/* Read the user number from the line */
        position=ftell(file)-strlen(line);	/* Get position to overwrite */
        if (snumber == usernumber)
        {
            fseek(file,position,SEEK_SET);	/* Move file pointer to overwrite position */
            fprintf(file, "%ld,%s,1\n", snumber, password);		/* Update password and status */
            fclose(file);	/* Close file */
			pthread_mutex_unlock(&file_mutex);		/* Unlock mutex */
            return;		/* User found, exit the function */
        }
    }
	
	/* If user not found, add new entry to the file */
    fprintf(file, "%ld,%s,1\n", usernumber, password);
    fclose(file);
    pthread_mutex_unlock(&file_mutex);		/* Unlock mutex */
}

/****************************************************************************
*       Function Name   : add_cfss_details
*       Description     : Writing users cfss details to file.
*       Returns         : Nothing.
****************************************************************************/

void add_cfss_details(long int usernumber,long int rnumber,char *type)
{
    pthread_mutex_lock(&file_mutex);	/* Lock file for thread safety */

    FILE *file = fopen(DETAILS_FILE,"r+");
    if(file == NULL)
    {
		file = fopen(FILE_NAME, "a+");
        if (file == NULL) {
            perror("Error opening file");
            pthread_mutex_unlock(&file_mutex);		/* Unlock mutex before returning */
			log_changes(FATAL,"Error opening file to read");
            return;
		}
    }
	char line[MAXBUFF];
    long int snumber=0;
	int position=0;
    /* Read each line in the file to check if user number exists */
    while (fgets(line, sizeof(line), file)) 
	{
        sscanf(line, "%ld", &snumber);		/* Read the user number from the line */
        position=ftell(file)-strlen(line);	/* Get position to overwrite */
        if (snumber == usernumber)
        {
            fseek(file,position,SEEK_SET);	/* Move file pointer to overwrite position */
            fprintf(file, "%ld,%ld,%s,1\n", snumber, rnumber,type);		/* Update receiver number and type*/
			printf("Forwarding details updated successfully.\n");
            fclose(file);	/* Close file */
			pthread_mutex_unlock(&file_mutex);		/* Unlock mutex */
            return;		/* User found, exit the function */
        }
    }
    fprintf(file, "%ld,%ld,%s,1\n",usernumber,rnumber,type);	/* Append new entry */
	printf("Forwarding details added successfully.\n");
    fclose(file);

    pthread_mutex_unlock(&file_mutex);		/* Unlock mutex */
}

/****************************************************************************
*       Function Name   : find_usernumber
*       Description     : Validates user credentials by checking the user 
*                         number and password from a file.
*       Returns         : Returns 2 if successful, 1 if password mismatch, 
*                         0 if file error, -1 if user not found.
****************************************************************************/

int find_usernumber(long int usernumber, char *password)
{
    pthread_mutex_lock(&file_mutex);	/* Lock file for thread safety */

    long int snumber=0;
    char filePassword[MAXBUFF],line[MAXBUFF];
    int status=0;

    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Error opening file to read");
		pthread_mutex_unlock(&file_mutex);		/* Unlock mutex before returning */
        return 0;	/* Return FAILURE if file error */
    }

    /* Check credentials line by line */
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%ld,%[^,],%d", &snumber, filePassword,&status);
        if (usernumber==snumber && status==1)
        {
            if(strcmp(password, filePassword) == 0)
            {
                fclose(file);	/* Close file on success */
				pthread_mutex_unlock(&file_mutex);		/* Unlock mutex */
                return 2;	/* Return 2 for successful login */
            }
            fclose(file);
			pthread_mutex_unlock(&file_mutex);
            return 1;	/* Return 1 for password mismatch */
        }
    }

    fclose(file);
	pthread_mutex_unlock(&file_mutex);
    return -1;	/* Return -1 if user not found */
}

/****************************************************************************
*       Function Name   : is_active
*       Description     : Checks and updates the active status of a user 
*                         (sphno) in a file.
*       Returns         : Returns 1 if already active or deactive, 0 if updated successfully, 
*                           -1 if user not found, 2 if file error.
****************************************************************************/

int is_active(long int sphno,int status)
{
    pthread_mutex_lock(&file_mutex);	/* Lock file for thread safety */

    char line[MAXBUFF];
    long int snumber=0,rnumber=0;
    char type[MAXBUFF];
    int current_active=0,position=0;

    FILE *file = fopen(DETAILS_FILE,"r+");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
		pthread_mutex_unlock(&file_mutex);	/* Unlock mutex before returning */
        return 2;	/* Return 2 for file error */
    }

    while (fgets(line, sizeof(line), file)) 
	{
        sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rnumber, type, &current_active);
		position=ftell(file)-strlen(line);	/* Get position to overwrite */
        if(snumber == sphno)
        {
            if (status==1)
            {
                if(current_active==1)
                {
                    pthread_mutex_unlock(&file_mutex);	/* Unlock mutex before returning */
                    fclose(file);
                    return 1;	/* Return 1 if already active */
                }

                fseek(file, position, SEEK_SET);
                fprintf(file, "%ld,%ld,%s,%d\n", snumber, rnumber, type, status);	/* Update status */
				pthread_mutex_unlock(&file_mutex);	/* Unlock mutex */
                fclose(file);
                return 0;	/* Return 0 if status updated */
            }
            if(current_active==1)
            {
				fseek(file,position,SEEK_SET);
				fprintf(file,"%ld,%ld,%s,%d\n",snumber,rnumber,type,status);	/* Deactivate user */
				fflush(file);
				fclose(file);
				pthread_mutex_unlock(&file_mutex);	/* Unlock mutex */
				return 0;
            }
            pthread_mutex_unlock(&file_mutex);	/* Unlock mutex before returning */
            fclose(file);
            return 1;	/* Return 1 if already inactive */
        }

    }
	fclose(file);
	pthread_mutex_unlock(&file_mutex);	/* Unlock mutex before returning */
    return -1;	/* Return -1 if user not found */
}

/****************************************************************************
*       Function Name   : is_registered
*       Description     : Checks if a user (sphno) is registered, and updates
*                         the status if necessary.
*       Returns         : Returns 1 if user is unregistered already, 0 if updated successfully,
 *                           -1 if user not found, 2 if file error.
****************************************************************************/

int is_registered(long int sphno)
{

    pthread_mutex_lock(&file_mutex);	/* Locking file access for thread safety */

    char line[MAXBUFF];
    long int snumber=0;
    char password[MAXBUFF];
    int current_status=0,position=0;

	/* Open the file in read and write mode */
    FILE *file = fopen(FILE_NAME,"r+");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
		pthread_mutex_unlock(&file_mutex);	/* Unlocking file before returning */
        return 1;	/* Return error code */
    }

	/* Read the file line by line to find the user number */
    while (fgets(line, sizeof(line), file)) 
	{
        sscanf(line, "%ld,%[^,],%d", &snumber, password, &current_status);
		
		/* If the user number matches, update status or return appropriate response */
        if(snumber == sphno)
        {
			/* Move file pointer back to update the user's status to inactive */
            position=ftell(file)-strlen(line);
            fseek(file, position, SEEK_SET);
            fprintf(file, "%ld,%s,0\n", snumber, password);	  /* Update the user to inactive */
            pthread_mutex_unlock(&file_mutex);	  /* Unlock before returning */
            fclose(file);
            return 0;	/* Return inactive status */
        }

    }
	fclose(file);
	pthread_mutex_unlock(&file_mutex);
    return -1;	/* User not found */
}

/****************************************************************************
*       Function Name   : is_sphno_exits
*       Description     : Checks if the given user number (sphno) exists 
*                         and returns its active status.
*       Returns         : Returns 2 on active, 1 on inactive, -1 if user not
*						found, 0 on file error.
****************************************************************************/

int is_sphno_exits(long int sphno)
{
	pthread_mutex_lock(&file_mutex);	/* Locking for thread safety */

    char line[MAXBUFF],type[MAXBUFF];
    long int snumber=0;
    int active=0;

	/* Open the details file in read mode */
    FILE *file = fopen(DETAILS_FILE,"r");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
		pthread_mutex_unlock(&file_mutex);	/* Unlock file access before returning */
        return 0;	/* Return error code */
    }

	/* Read file line by line to find the user number */
	while(fgets(line, sizeof(line), file))
	{
		sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rphno, type, &active);

		/* If the user number matches, return active or inactive status */
		if(snumber==sphno)
		{
			if(active==1)	/* If the user is active */
			{
				pthread_mutex_unlock(&file_mutex);
                fclose(file);
                return 2;	/* Return active status */
			}
			pthread_mutex_unlock(&file_mutex);
    		fclose(file);
			return 1;	/* Return inactive status */
		}
	}
	pthread_mutex_unlock(&file_mutex);
    fclose(file);
    return -1;	/* User not found */

}

/****************************************************************************
*       Function Name   : check_forwording_type
*       Description     : Checks the users forwarding type.
*       Returns         : Returns NULL if file not exits or usernumber not exits 
*						and returns type of usernumber exits.
****************************************************************************/

char *check_forwarding_type(long int sphno)
{
    pthread_mutex_lock(&file_mutex);	/* Locking for thread safety */

    char line[MAXBUFF];
    long int snumber=0,rnumber=0;
	int active=0;
    static char type[MAXBUFF];	/* Static variable to return the type */

	/* Open the details file in read mode */
    FILE *file = fopen(DETAILS_FILE,"r");
    if(file == NULL)
    {
        perror("Error opening file");
		log_changes(FATAL,"Error opening file to read");
		pthread_mutex_unlock(&file_mutex);	/* Unlock file access before returning */
        return NULL;	/* Return NULL on error */
    }

	/* Read each line of the file to find the forwarding type for the user */
    while (fgets(line, sizeof(line), file)) 
	{
        sscanf(line, "%ld,%ld,%[^,],%d", &snumber, &rnumber, type, &active);

        if(snumber == sphno)	/* If the user number matches */
        {
            pthread_mutex_unlock(&file_mutex);
            fclose(file);
            return type;	/* Return the forwarding type */
        }
    }
    pthread_mutex_unlock(&file_mutex);
    fclose(file);
    return NULL;	/* Return NULL if the user number is not found */
}

/****************************************************************************
*       Function Name   : main_menu
*       Description     : Handles the client's menu options and routes to 
*                         appropriate functions based on the selected option. 
*       Returns         : Nothing.
****************************************************************************/

void *main_menu(void *client)
{
    int client_sock=*(int *)client;
	int option=0;

	/* Receive the menu option from the client */
    recv(client_sock,&option,sizeof(option),0);

	/* Process the user's selection */
    switch(option)
    {
        case 1:
            client_register(client_sock);	/* Register the client */
            edit_menu(client_sock);    /* Show the menu for editing details */
            break;
        case 2:
            client_login(client_sock);	  /* Log the client in */
            edit_menu(client_sock);    /* Show the menu for editing details */
            break;
        case 3:
            forward_call(client_sock);	/* Handle call forwarding */
            break;
		case 0:
			printf("Client exiting...\n");
			break;
        default:
			log_changes(WARNING,"Invalid option while choosing from menu");
			break;
    }
	return NULL;	/* Return after processing the user's option */
}

/****************************************************************************
*       Function Name   : client_register
*       Description     : Registers a new user by checking if the usernumber 
*                         already exists. If not, it proceeds to set a password.
*       Returns         : Nothing.
****************************************************************************/

void client_register(int client_sock)
{
    recv(client_sock,&sphno,sizeof(sphno),0);	/* Receive the user number (sphno) */

	if(sphno==0)	/* If the user number is zero, exit the registration */
	{
		return;
	}
	log_changes(INFO,"User trying to register");

	/* Check if the user number is already taken */
    if (find_usernumber(sphno, "") == 1)
    {
        send(client_sock, "Usernumber already taken.\nPlease log in.\n", 42, 0);
        printf("Usernumber already taken.\nPlease log in.\n");
		log_changes(WARNING,"Usernumber already taken while registering");
        client_login(client_sock);	/* Direct to login if the user number is taken */
        return;    /* Return after login attempt */
    }
    set_password(client_sock);	  /* Proceed to set a password */
}

/****************************************************************************
*       Function Name   : set_password
*       Description     : Prompts the user to enter and confirm a password.
*                         If passwords match, saves the user details.
*                         Allows up to 3 attempts for correct entry.
*                         Receives password and confirmation password from client.
*       Returns         : Nothing.
****************************************************************************/

void set_password(int client_sock)
{
	int attempts=0;

	/* Ask the user to enter a password */
    send(client_sock, "Enter password: ", 17, 0);
    while (1)
    {
		if(attempts<3)	  /* Allow up to 3 attempts for correct password entry */
		{
        	memset(password, 0, sizeof(password));
        	recv(client_sock, password, sizeof(password), 0);
        	memset(conf_password, 0, sizeof(conf_password));
        	recv(client_sock, conf_password, sizeof(conf_password), 0);
        	if (strcmp(password, conf_password) == 0)	/* If passwords match */
        	{
            	/* Add client details and complete registration */
            	add_client_details(sphno, password);
            	send(client_sock, "Registration successful\n", 26, 0);
				printf("Registration successful.\n");
				log_changes(DEBUG,"User register successfully");
            	break;	  /* Registration successful, exit loop */
        	}
        	else
        	{
				send(client_sock,"Enter password: ",17,0);	  /* Prompt again if passwords do not match */
				attempts++;  /* Increment the attempt counter */
        	}
		}
		else
		{
			log_changes(WARNING,"Max limit excceded while user entering password to register");
			break;	  /* Exit after 3 attempts */
		}
    }
}

/****************************************************************************
*       Function Name   : client_login
*       Description     : Handles user login by receiving the usernumber 
*                         and password from the client, verifying them, and 
*                         sending appropriate responses. If not registered, 
*                         it prompts the user to register.
*       Returns         : Nothing.
****************************************************************************/

void client_login(int client_sock)
{
	int attempts=0;	/* Counter for login attempts */

	/* Receive user number (phone number) from the client */
    recv(client_sock,&sphno,sizeof(sphno),0);
	log_changes(INFO,"User trying to login");

	/* Check if the user is already registered */
    if (find_usernumber(sphno, "") == -1)
    {
		/* If not registered, inform the client and prompt for registration */
        send(client_sock,"User is not registered\n", 25, 0);
        printf("\nUser is not registered\n");
		log_changes(WARNING,"User trying to login without registering");

        client_register(client_sock);	/* Call client_register() to register the user */
        return;  /* Return as the user needs to register */
    }

	/* Prompt the user to enter the password */
	send(client_sock,"Enter password: ",17,0);

	/* Loop for password attempts (maximum of 3 attempts) */
    while(1)
    {
		if(attempts<=3)	/* Allow up to 3 attempts */
		{
        	memset(password,0,sizeof(password));	/* Clear the password buffer */
        	recv(client_sock,password,sizeof(password),0);	/* Receive password from the client */

			/* Validate the password */
        	if(find_usernumber(sphno,password)==2)
        	{
				/* Successful login */
            	send(client_sock,"Logged in successfully.\n",26,0);
            	printf("Logged in successfully.\n");
				log_changes(DEBUG,"User logged in successfully");
            	break;	/* Exit the loop on successful login */
			}
        	else if(find_usernumber(sphno,password)==1)
        	{
				/* Invalid password, increment attempt counter and retry */
				attempts++;
				send(client_sock,"Enter password: ",17,0);
				log_changes(WARNING,"Password mismatched while logging");
            	continue;	/* Continue the loop for another password attempt */
        	}
			else
			{
				continue;	/* Invalid password case, continue without incrementing attempt count */
			}
		}
		else
		{
			/* If the maximum number of attempts is reached */
			printf("Maximum limit reached while logging");
			log_changes(WARNING,"Max limit reached to enter password while logging");
			break;	/* Exit the loop */
		}
    }
}

/****************************************************************************
*       Function Name   : edit_menu
*       Description     : Receives a menu choice from the client, processes the 
*                         selected option , and 
*                         communicates the result back to the client.
*       Returns         : Nothing.
****************************************************************************/

void edit_menu(int client_sock)
{
    int choice=0;	/* Variable to store the user's choice */
    recv(client_sock,&choice,sizeof(choice),0);	/* Receive the menu choice from the client */

	/* Process the user's choice */
    switch(choice)
    {
        case 1:
            forwarding_details(client_sock);	/* Call to handle forwarding details */
            break;
        case 2:
            activate_client();	/* Call to activate the client */
            break;
        case 3:
            deactivate_client();	/* Call to deactivate the client */
            break;
        case 4:
            unregister_client();	/* Call to unregister the client */
            break;
        case 0:
			/* Exit case */
			printf("\nClient exiting...\n");
			log_changes(INFO,"User logged in and exited");
            break;
        default:
			/* Invalid option case */
            printf("\nInvalid option\n");
			log_changes(WARNING,"User logged in and pressed invalid option to do settings");
            break;
    }
}

/****************************************************************************
*       Function Name   : forwarding_details
*       Description     : Receives forwarding details from the client and updates
*						the forwarding settings. Logs the new settings after updating.
*       Returns         : Nothing.
****************************************************************************/

void forwarding_details(int client_sock)
{
    int choice=0;
    char type[MAXBUFF];	/* To store the forwarding type (e.g., Unconditional, No reply, Busy) */

	/* Receive phone number and forwarding type choice from client */
    recv(client_sock, &rphno, sizeof(rphno), 0);
    recv(client_sock, &choice, sizeof(choice), 0);

	/* Determine the forwarding type based on client's choice */
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

	/* Call to add forwarding details to the file */
    add_cfss_details(sphno, rphno, type);
}

/****************************************************************************
*       Function Name   : activate_client
*       Description     : Activates a client if they are not already active.
*						Logs actions based on the client's status.
*       Returns         : Nothing.
****************************************************************************/

void activate_client()
{
	result=is_active(sphno,1);	/* Check if the client is already active */
    if(result==1)
    {
		/* Client is already active */
        printf("Already in active state.\n");
		log_changes(INFO,"User already in active state to forward call");
        return;
    }
	else if(result==0)
	{
		/* Activate the client */
    	printf("Activated user.\n");
		log_changes(INFO,"User activated to forward call");
		return;
	}
	else if(result==-1)
	{
		/* User has not provided forwarding details */
		printf("User not given forwarding details.\n");
		log_changes(WARNING,"User trying to activate without providing forwarding details");
		return;
	}
	else
	{
		return;
	}
}

/****************************************************************************
*       Function Name   : deactivate_client
*       Description     : Deactivates a client if they are active. Logs actions
*						based on the client's status.
*       Returns         : Nothing.
****************************************************************************/

void deactivate_client()
{
	result=is_active(sphno,0);	/* Check if the client is active for deactivation */
    if(result==1)
    {
		/* Client is already in deactivated state */
        printf("Already in deactive state.\n");
		log_changes(INFO,"User already in deactive state to forward call");
        return;
    }
	else if(result==0)
	{
		/* Deactivate the client */
    	printf("Deactivated user.\n");
		log_changes(INFO,"User deactivated to forward call");
		return;
	}
	else if(result==-1)
	{
		/* User hasn't provided forwarding details */
		printf("User not given forwarding details.\n");
		log_changes(WARNING,"User trying to deactivate without providing forwarding details");
		return;
	}
	else
	{
		return;
	}
}

/****************************************************************************
*       Function Name   : unregister_client
*       Description     : Unregisters a client if they are registered. Logs the action.
*       Returns         : Nothing.
****************************************************************************/

void unregister_client()
{
	result=is_registered(sphno);
    if(result==0)
    {
		/* Unregister the user */
     	printf("Unregistered successfully.\n");
		log_changes(DEBUG,"Unregistered successfull");
    }
	else if(result==-1)
	{
		printf("User not registered.\n");
		log_changes(DEBUG,"User not registered");
	}
	else
	{
	}

}

/****************************************************************************
*       Function Name   : forward_call
*       Description     : Forwards a call based on the user's forwarding settings 
*                         (Unconditional, No reply, or Busy). Logs each action.
*       Returns         : Nothing.
****************************************************************************/

void forward_call(int client_sock)
{
	char type[MAXBUFF];

	/* Receive the user's phone number and check if it exists in the system */
	recv(client_sock,&sphno,sizeof(sphno),0);
	result=is_sphno_exits(sphno);
	strcpy(type,check_forwarding_type(sphno));	/* Get the forwarding type */
	if(result==2)
	{
		/* If user is active, forward the call based on the type */
		if(strcmp(type,"Unconditional")==0)
		{
			printf("Call forwarding to %ld.\n",rphno);
			log_changes(DEBUG,"Call forwarding...");
		}
		else if(strcmp(type,"No reply")==0)
		{
			/* Wait and forward call if no reply */
			printf("Waiting to lift the call.\n");
			log_changes(INFO,"Waiting to lift the call");
			sleep(5);
			printf("Call forwarding to %ld.\n",rphno);
			log_changes(DEBUG,"Call forwarding....");
		}
		else if(strcmp(type,"Busy")==0)
		{
			/* Forward the call if the user is busy */
			printf("User is busy.\n");
			log_changes(INFO,"User is busy to lift the call");
			printf("Call forwarding to %ld.\n",rphno);
			log_changes(DEBUG,"call forwarding....");
		}
		else
		{
			/* Invalid forwarding type */
			printf("Not a valid type.\n");
			log_changes(WARNING,"Not given valid type to forward the call");
		}
	}
	else if(result==1)
	{
		/* User is not active */
		printf("User is not active.\n");
		log_changes(INFO,"User is not active to forward the call");
	}
	else if(result==-1)
	{
		/* User hasn't provided forwarding details */
		printf("%ld not given forwarding details.\n",sphno);
		log_changes(INFO,"User not provided forwarding deatils");
	}
	else
	{
		return;
	}
}

/****************************************************************************
*       Function Name   : log_changes
*       Description     : Logs changes made by the user, including the date, time, 
*                         log level, user number, and the function where the change occurred.
*       Returns         : Nothing.
****************************************************************************/

void log_changes(LogLevel level, const char *function_name)
{
    FILE *logfile;
    time_t currentTime;
    struct tm *localTime;

    time(&currentTime);		/* Get the current time */
    localTime = localtime(&currentTime);	/* Convert it to local time */

    /* Open the log file in append mode (creates the file if it doesn't exist) */
    logfile = fopen(LOG_FILE, "a");

    /* Check if the file opened successfully */
    if (logfile == NULL) {
        printf("Error opening the log file.\n");
        return;
    }
	
	/* Determine log level string */
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
 	
	/* Log the current date, time, and the function where changes are made */
    fprintf(logfile, "[%s]\nDate: %02d/%02d/%04d Time: %02d:%02d:%02d userNumber: %ld - %s \n\n",level_str,localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,localTime->tm_hour, localTime->tm_min, localTime->tm_sec,sphno,function_name);

	/* Close the log file */
    fclose(logfile);
}
