#include <common.h>
#include <clntcall.h>

/****************************************************************************
*       Function Name   : client
*       Description     : Creates a client, connects to a server, presents 
*                       a menu for user interaction (register, login, etc.), 
*						takes input from user and then sends the data to the server.
*       Returns         : returns 0 on success, 1 on failure (socket/connection error)
****************************************************************************/

int client(char *argv[]) {

	/* port_no: Stores the port number to connect to the server
	sock: Server id
	option: To read user's choice*/
    int port_no=0,sock=0,option=0;

	/*ip_addr: A character array that stores the server's IP addres*/
    char ip_addr[strlen(argv[2])];

    port_no = atoi(argv[1]);
    strcpy(ip_addr,argv[2]);
	
	 /* server_addr: A struct to store the server's address information (IP, port, etc.)*/
    struct sockaddr_in server_addr;

    /* Create socket: Using AF_INET (IPv4), SOCK_STREAM (TCP), and protocol 0 (default)*/
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
    
	printf("=============== MAIN MENU ===============");
	printf("\n1. Register\n2. Login\n3. Forward call\n0. Exit\n");
	printf("Enter the choice: ");

    scanf("%d", &option);
    getchar();
    send(sock, &option, sizeof(option), 0);
    main_menu(sock, option);

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
    switch(option)
    {
        case 1:
            regist(sock);
            edit_menu(sock);
            break;
        case 2:
            login(sock);
            edit_menu(sock);
            break;
        case 3:
            forward_call(sock);
            break;
		case 0:
			break;
        default:
            printf("\nInvalid option\n");
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
	int attempts=0;/*For number of failed registration attempts*/
	printf("\n============ REGISTRATION PAGE ===============\n");
	printf("Start registering\n");
    while(1)
    {
		if(attempts<3)
		{
			printf("Enter the usernumber: ");
        	if(scanf("%ld", &phno) == 1)
        	{
            	getchar();
			
            	if(phno >= 1000000000 && phno <= 9999999999)
            	{
                	send(sock, &phno, sizeof(phno), 0);
                	break;
            	}
            	else
            	{
					attempts++;
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");
			exit(EXIT_FAILURE);
		}
    }
	attempts=0;
    recv(sock, buffer, sizeof(buffer), 0);

    if(strcmp(buffer, "Usernumber already taken.\nPlease log in.\n") == 0)
    {
        login(sock);
        return;
    }
    if (strcmp(buffer, "Enter password: ") == 0)
    {
        while(1)
        {
			if(attempts<3)
			{
            	printf("%s", buffer);
           	 	fgets(password, MAXBUFF, stdin);
            	password[strlen(password)-1] = '\0';
            	send(sock, password, sizeof(password), 0);
           	 	printf("Enter confirm password: ");
            	fgets(conf_password, MAXBUFF, stdin);
            	conf_password[strlen(conf_password) - 1] = '\0';
            	send(sock, conf_password, sizeof(conf_password), 0);
            	recv(sock, buffer, sizeof(buffer), 0);
            	if (strcmp(buffer, "Registration successful\n") == 0)
            	{
                	break;
            	}
            	else
            	{
					printf("\nPassword mismatch! Please try again.\n");
					attempts++;
            	}
			}
			else
			{
				printf("Maximum limit exceeded.\nTry again later.\n");
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
	int attempts=0; ;/*For number of failed login attempts*/
	printf("\n=============== LOGIN PAGE ===============\n");
    printf("\nStart logging in\n");
    memset(buffer, 0, sizeof(buffer));
    while(1)
    {
		if(attempts<3)
		{
			printf("Enter the number: ");
        	if(scanf("%ld", &phno) == 1)
        	{
            	getchar();
            	if(phno >= 1000000000 && phno <= 9999999999)
            	{
                	send(sock, &phno, sizeof(phno), 0);
                	break;
            	}
            	else
            	{
					attempts++;
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");
			exit(EXIT_FAILURE);
		}
    }
	attempts=0;
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
			if(attempts<3)
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
					attempts++;
					printf("\nPassword mismatch! Please re-enter.\n");
            	}
			}
			else
			{
				printf("Maximum limit exceeded.\nTry again later.\n");
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
	printf("============ EDIT MENU ===============");
	printf("\n1. Forwarding details.\n2. Activate.\n3. Deactivate.\n4. Unregister.\n0. Exit\n");
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
		case 0:
			break;
		default:
			printf("Entered invalid option.\n");
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
		if(attempts<3)
		{
        	printf("Enter requested client number: ");
        	if(scanf("%ld", &rphno) == 1)
        	{
            	getchar();
            	if(rphno >= 1000000000 && rphno <= 9999999999)
            	{
                	send(sock, &rphno, sizeof(rphno), 0);
                	break;
            	}
            	else
            	{
					attempts++;
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");
			exit(EXIT_FAILURE);
		}
    }
    printf("\n##### Forwarding types #####\n");
    printf("1.Unconditional\n2.No reply\n3.Busy\n");
	attempts=0;
    while(1)
    {
		if(attempts<3)
		{
        	printf("\nEnter the choice: ");
        	scanf("%d", &type_choice);
        	if(type_choice < 4 && type_choice > 0)
        	{
            	send(sock, &type_choice, sizeof(type_choice), 0);
            	break;
        	}
			attempts++;
        	printf("\nEnter between 1 to 3");
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");
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
	int attempts=0;/*for number of invlaid input attempts*/
    while(1)
    {
		if(attempts<3)
		{
        	printf("Enter requested client number: ");
        	if(scanf("%ld", &phno) == 1)
        	{
            	getchar();
            	if(phno >= 1000000000 && phno <= 9999999999)
            	{
                	send(sock, &phno, sizeof(phno), 0);
                	break;
            	}
            	else
            	{
					attempts++;
                	printf("\nEnter 10 digits\n");
            	}
        	}
        	else
        	{
				attempts++;
            	printf("\nEnter only long integers\n");
            	getchar();
        	}
    	}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");
			exit(EXIT_FAILURE);
		}
	}
}
