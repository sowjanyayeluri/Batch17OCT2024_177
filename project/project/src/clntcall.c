#include <common.h>
#include <clntcall.h>

int client(int argc,char *argv[]) {
    int port_no,sock,option;
    char ip_addr[strlen(argv[2])];

    port_no = atoi(argv[1]);
    strcpy(ip_addr,argv[2]);

    struct sockaddr_in server_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }
    
	printf("=============== MAIN MENU ===============");
	printf("\n1. Register\n2. Login\n3. Forward call\n0. Exit\n");
	printf("Enter the choice: ");

    scanf("%d", &option);
    getchar();
    send(sock, &option, sizeof(option), 0);
    main_menu(sock, option);

    close(sock);
    return 0;
}

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

void regist(int sock)
{
	int attempts=0;
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
				printf("%ld",phno);
            	if(phno >= 1000000000 && phno <= 9999999999)
            	{
					printf("h");
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
            	fgets(confPassword, MAXBUFF, stdin);
            	confPassword[strlen(confPassword) - 1] = '\0';
            	send(sock, confPassword, sizeof(confPassword), 0);
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

void login(int sock)
{
	int attempts=0;
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

void edit_menu(int sock)
{
    int option=0;
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

void give_details(int sock)
{
    long int rphno;
    int type_choice,attempts=0;
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

void forward_call(int sock)
{
	int attempts=0;
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
