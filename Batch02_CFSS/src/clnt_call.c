/************************************************************************* 
*
*  FILE NAME    : clnt_call.c
*
*  DESCRIPTION  : Contains functions to handle client-side operations
*                 like user registration, login, and call forwarding.
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
#include <clnt_call.h>

/****************************************************************************
*       Function Name   : client
*       Description     : Creates a client, connects to a server, presents 
*                       a menu for user interaction (register, login, etc.), 
*						takes input from user and then sends the data to the server.
*       Returns         : returns 0 on success, 1 on failure (socket/connection error)
****************************************************************************/

int client(char *argv[]) {

    int port_no=0,sock=0,option=0;

	/*ip_addr: A character array that stores the server's IP addres*/
    char ip_addr[strlen(argv[2])];

	/* Extract port number and IP address from arguments */
    port_no = atoi(argv[1]);
    strcpy(ip_addr,argv[2]);
	
	/* server_addr: Struct to store server address (IP, port, etc.) */
    struct sockaddr_in server_addr;

    /* Create socket with IPv4 and TCP protocol */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return FAILURE;
    }
	
	/* Initialize the server_addr structure to zero*/
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);

    /* Connect to server using the address stored in server_addr*/
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return FAILURE;
    }
    
	/* Display the main menu for user to choose an option */
	printf("=============== MAIN MENU ===============");
	printf("\n1. Register\n2. Login\n3. Forward call\n0. Exit\n");
	printf("Enter the choice: ");

	/* Get user choice and send it to the server */
    scanf("%d", &option);
    getchar();
    send(sock, &option, sizeof(option), 0);

	/* Process the option chosen by the user */
    main_menu(sock, option);

	/* Close the socket after interaction */
    close(sock);
    return SUCCESS;
}

/****************************************************************************
*       Function Name   : main_menu
*       Description     : Executes actions based on user menu choice (register, 
*                         login, forward call, etc.).
*       Returns         : Nothing.
****************************************************************************/


void main_menu(int sock, int option)
{
	/* Switch case to handle user choice from the menu */
    switch(option)
    {
        case 1:
            regist(sock);	/* Call the registration function */
            edit_menu(sock);	/* Edit menu after registration */
            break;
        case 2:
            login(sock);	/* Call the login function */
            edit_menu(sock);	/* Edit menu after login */
            break;
        case 3:
            forward_call(sock);	   /* Call the forward call function */
            break;
		case 0:
			break;	/* Exit if option is 0 */
        default:
            printf("\nInvalid option\n");	/* Handle invalid menu option */
			break;
    }
}

/****************************************************************************
*       Function Name   : regist
*       Description     : Manages user registration, validates input, and 
*                         communicates with the server.
*       Returns         : Nothing.
****************************************************************************/

void regist(int sock)
{
	int attempts=0;	  /*For number of failed registration attempts*/
	printf("\n============ REGISTRATION PAGE ===============\n");
	printf("Start registering\n");
    while(1)
    {
		if(attempts<3)	  /* Allow up to 3 failed attempts */
		{
			printf("Enter the usernumber: ");
        	if(scanf("%ld", &phno) == 1)	/* Input user number (phone number) */
        	{
            	getchar();
			
				/* Validate if phone number is a 10-digit long number */
            	if(phno >= 1000000000 && phno <= 9999999999)
            	{
                	send(sock, &phno, sizeof(phno), 0);	  /* Send phone number to the server */
                	break;	/* Exit loop if valid phone number is entered */
            	}
            	else
            	{
					attempts++;	  /* Increment attempts on invalid phone number */
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;	 /* Increment attempts on non-numeric input */
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");	  /* Exit after 3 failed attempts */
			exit(EXIT_FAILURE);
		}
    }

	/* Reset attempts counter and receive server response */
	attempts=0;
    recv(sock, buffer, sizeof(buffer), 0);

	/* Check if user number is already registered */
    if(strcmp(buffer, "Usernumber already taken.\nPlease log in.\n") == 0)
    {
        login(sock);	/* Redirect to login if user number is already taken */
        return;
    }

	/* Check if the server asks for password */
    if (strcmp(buffer, "Enter password: ") == 0)
    {
        while(1)
        {
			if(attempts<3)	  /* Allow up to 3 failed password attempts */
			{
            	printf("%s", buffer);
           	 	fgets(password, MAXBUFF, stdin);
            	password[strlen(password)-1] = '\0';
            	send(sock, password, sizeof(password), 0);	 /* Send password to server */
           	 	printf("Enter confirm password: ");
            	fgets(conf_password, MAXBUFF, stdin);
            	conf_password[strlen(conf_password) - 1] = '\0';
            	send(sock, conf_password, sizeof(conf_password), 0);	/* Send confirm password */

				/* Receive server response */
            	recv(sock, buffer, sizeof(buffer), 0);
            	if (strcmp(buffer, "Registration successful\n") == 0)
            	{
                	break;	  /* Exit loop on successful registration */
            	}
            	else
            	{
					printf("\nPassword mismatch! Please try again.\n");	  /* Handle password mismatch */
					attempts++;	  /* Increment attempts */
            	}
			}
			else
			{
				printf("Maximum limit exceeded.\nTry again later.\n");	  /* Exit after 3 failed attempts */
				exit(EXIT_FAILURE);
			}
        }
    }
}

/****************************************************************************
*       Function Name   : login
*       Description     : Handles user login, validates user number and password, 
*                         and communicates with the server for authentication. 
*       Returns         : Nothing.
****************************************************************************/

void login(int sock)
{
	int attempts=0; ;	/*For number of failed login attempts*/
	printf("\n=============== LOGIN PAGE ===============\n");
    printf("\nStart logging in\n");
    memset(buffer, 0, sizeof(buffer));
    while(1)
    {
		if(attempts<3)	  /* Allow up to 3 failed login attempts */
		{
			printf("Enter the number: ");
        	if(scanf("%ld", &phno) == 1)	/* Input phone number (user identification) */
        	{
            	getchar();

				/* Validate if the phone number is a 10-digit long number */
            	if(phno >= 1000000000 && phno <= 9999999999)
            	{
                	send(sock, &phno, sizeof(phno), 0);	  /* Send phone number to the server */
                	break;	  /* Break the loop when phone number is valid */
            	}
            	else
            	{
					attempts++;	  /* Increment attempts if the number is invalid */
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;	 /* Increment attempts if input is not a valid long integer */
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");	 /* Exit after 3 failed attempts */
			exit(EXIT_FAILURE);
		}
    }

	/* Reset attempts counter after phone number is sent */
	attempts=0;
    recv(sock, buffer, sizeof(buffer), 0);	 /* Receive server's response */

	/* Check if the user is not registered */
    if(strcmp(buffer, "User is not registered\n") == 0)
    {
        regist(sock);	/* If user is not registered, call registration function */
        return;
    }

	/* Check if the server asks for the password */
    if(strcmp(buffer, "Enter password: ") == 0)
    {
        while(1)
        {
			if(attempts<3)	 /* Allow up to 3 failed password attempts */
			{
            	printf("%s", buffer);
            	fgets(password, MAXBUFF, stdin);
            	password[strlen(password)-1] = '\0';
            	send(sock, password, sizeof(password), 0);	 /* Send password to the server */
            	recv(sock, buffer, sizeof(buffer), 0);	 /* Receive response from server */

				/* Check if login was successful */
            	if(strcmp(buffer, "Logged in successfully.\n") == 0)
            	{
                	break;	 /* Exit loop on successful login */
            	}	
           	 	else
            	{
					attempts++;	 /* Increment attempts if password mismatch */
					printf("\nPassword mismatch! Please re-enter.\n");
            	}
			}
			else
			{
				printf("Maximum limit exceeded.\nTry again later.\n");	 /* Exit after 3 failed password attempts */
				exit(EXIT_FAILURE);
			}
		}
    }
}

/****************************************************************************
*       Function Name   : edit_menu
*       Description     : Displays and take choice as input, then sends the 
*						data to the server and handles user choices in the edit menu.
*       Returns         : Nothing.
****************************************************************************/

void edit_menu(int sock)
{
    int option=0; /* User selected option*/

	/* Display the edit menu for the user */
	printf("============ EDIT MENU ===============");
	printf("\n1. Forwarding details.\n2. Activate.\n3. Deactivate.\n4. Unregister.\n0. Exit\n");
    printf("Enter the choice: ");
    scanf("%d", &option);
    getchar();
    send(sock, &option, sizeof(option), 0);	  /* Send selected option to the server */

	/* Handle different menu options based on user input */
    switch(option)
    {
        case 1:
            give_details(sock);	  /* Call function for forwarding details */
            break;
        case 2:
            break;	  /* Placeholder for activating */
        case 3:
            break;	  /* Placeholder for deactivating */
        case 4:
            break;	  /* Placeholder for unregistering */
		case 0:
			break;	  /* Exit the menu */
		default:
			printf("Entered invalid option.\n");	/* Handle invalid menu option */
			break;
    }
}

/****************************************************************************
*       Function Name   : give_details
*       Description     : Collects and validates client number and forwarding 
*                         type, then sends the data to the server.
*       Returns         : Nothing.
****************************************************************************/

void give_details(int sock)
{
    long int rphno=0;/*stores requested client number*/

    /*type_choice: stores the users choice for forwarding type
	attempts: Number of inavlid attempts*/
	int type_choice=0,attempts=0;

    while(1)
    {
		if(attempts<3)	 /* Allow up to 3 failed attempts for entering requested client number */
		{
        	printf("Enter requested client number: ");
        	if(scanf("%ld", &rphno) == 1)	/* Read requested client phone number */
        	{
            	getchar();
            	if(rphno >= 1000000000 && rphno <= 9999999999)	 /* Validate phone number format */
            	{
                	send(sock, &rphno, sizeof(rphno), 0);	/* Send phone number to the server */
                	break;	 /* Break the loop when phone number is valid */
            	}
            	else
            	{
					attempts++;	  /* Break the loop when phone number is valid */
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;	  /* Increment attempts for non-numeric input */
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");	 /* Exit after 3 failed attempts */
			exit(EXIT_FAILURE);
		}
    }

	/* Display forwarding options */
    printf("\n##### Forwarding types #####\n");
    printf("1.Unconditional\n2.No reply\n3.Busy\n");
	attempts=0;	  /* Reset attempts for selecting forwarding type */
    while(1)
    {
		if(attempts<3)	  /* Allow up to 3 failed attempts for entering forwarding type */
		{
        	printf("\nEnter the choice: ");
        	scanf("%d", &type_choice);	  /* Read user's choice for forwarding type */

        	if(type_choice < 4 && type_choice > 0)	  /* Validate forwarding type choice (1-3) */
        	{
            	send(sock, &type_choice, sizeof(type_choice), 0);	/* Send choice to the server */
            	break;	 /* Exit loop on valid input */
        	}
			attempts++;	  /* Increment attempts for invalid input */
        	printf("\nEnter between 1 to 3");
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");	 /* Exit after 3 failed attempts */
			break;
		}
    }
}

/****************************************************************************
*       Function Name   : forward_call
*       Description     : Prompts for a client number, validates it, and 
*                         sends it to the server for call forwarding.
*       Returns         : Nothing.
****************************************************************************/

void forward_call(int sock)
{
	int attempts=0;	  /*for number of invlaid input attempts*/
    while(1)
    {
		if(attempts<3)	 /* Allow up to 3 failed attempts for entering requested client number */
		{
        	printf("Enter requested client number: ");
        	if(scanf("%ld", &phno) == 1)	/* Read requested client phone number */
        	{
            	getchar();
            	if(phno >= 1000000000 && phno <= 9999999999)	/* Validate phone number format */
            	{
                	send(sock, &phno, sizeof(phno), 0);	  /* Send phone number to the server */
                	break;	  /* Exit loop when phone number is valid */
            	}
            	else
            	{
					attempts++;	  /* Increment attempts for invalid phone number */
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;	  /* Increment attempts for non-numeric input */
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
    	}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");	 /* Exit after 3 failed attempts */
			exit(EXIT_FAILURE);
		}
	}
}
