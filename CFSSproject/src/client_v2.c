#include <common.h>
#include <client.h>

long int phno;

int client(int argc,char *argv[]) 
{ 
	int portNo,sock,option;
	char ip_addr[strlen(argv[2])];
	struct sockaddr_in server_addr;
 
	portNo = atoi(argv[1]);
	strcpy(ip_addr,argv[2]);
 
	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
	{
		perror("Socket creation failed");
    	return 1;
	}
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNo);
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);

	// Connect to server
	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
	{
		perror("Connection failed");
    	close(sock);
    	return 1;
	}
	printf("=============== MAIN MENU ===============");
	printf("\n1. Register\n2. Login\n3. Forward call\n");
	scanf("%d",&option);
	getchar();
	send(sock,&option,sizeof(option),0);
	menu(sock,option);
 
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
            break;
        case 2:
        	login(sock);
            edit_menu(sock);
            break;
        case 3:
			forward_call(sock);
            break;
        default:
        	printf("Enter valid");
    }
}

int validate_phno(long int *phno)
{
	int attempts = 0;
    while(1)
	{
    //	printf("%s",msg);
		if(attempts<4)
		{
    		if(scanf("%ld",phno)==1)
        	{
        		while(getchar()!='\n');;
            	if(*phno>=100000000 && *phno<9999999999)
            	{
					return 1;
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
        		printf("\nEnter only long intgers\n");
            	while(getchar()!='\n');
        	}
		}
		else
		{
			return 0;
		}

     }
}
void regist(int sock)
{
	int attempts = 0;
	char password[MAXBUFF],confPassword[MAXBUFF],buffer[MAXBUFF];
	printf("Registration\n");
   	
	while(1)
     {
	 	printf("Enter the usernumber: ");
        if(scanf("%ld",&phno)==1)
        {
            getchar();
            if(phno>=100000000 && phno<=9999999999)
            {
                send(sock,&phno,sizeof(phno),0);
                break;
            }
            else
            {
                printf("\nEnter 10 digits\n");
            }
        }
        else
        {
            printf("\nEnter only long intgers\n");
            getchar();
        }
     }
     recv(sock,buffer,sizeof(buffer),0);

     if(strcmp(buffer,"Username already taken.\nPlease log in.\n")==0)
     {
     	login(sock);
        return;
     }
     if (strcmp(buffer, "Enter password: ") == 0)
     {
     	while(1)
        {
        	printf("%s",buffer);
            fgets(password,MAXBUFF,stdin);
            password[strlen(password)-1]='\0';
            send(sock,password,sizeof(password),0);
            printf("Enter confirm password: ");
            fgets(confPassword, MAXBUFF, stdin);
            confPassword[strlen(confPassword) - 1] = '\0';
            send(sock, confPassword, sizeof(confPassword), 0);
            recv(sock, buffer, sizeof(buffer), 0);
			if(attempts<=3)
			{
            	if (strcmp(buffer, "Registration successful\n") == 0)
            	{
                	break;
            	}
            	else
            	{
					attempts++;
                	printf("\nPassword mismatch! Please re-enter.\n");
                	continue;
            	}
			}
			else
			{
				printf("Maximum limit exceeded.\nTry again later.\n");
			}
			
        }
     }

}

void login(int sock)
{
	char password[MAXBUFF],buffer[MAXBUFF];
	printf("Start logging in:");
    memset(buffer,0,sizeof(buffer));
    recv(sock,buffer,sizeof(buffer),0);
    printf("%s",buffer);
	while(1)
    {
        if(scanf("%ld",&phno)==1)
        {
            while(getchar() != '\n');
            if(phno>=100000000 && phno<=9999999999)
            {
                send(sock,&phno,sizeof(phno),0);
                break;
            }
            else
            {
                printf("\nEnter 10 digits\n");
            }
        }
        else
        {
            printf("\nEnter only long intgers\n");
            while(getchar() != '\n');
        }
    }
    recv(sock,buffer,sizeof(buffer),0);
    if(strcmp(buffer,"User is not registered\n")==0)
    {
    	regist(sock);
        return;
    }
    if(strcmp(buffer,"Enter password: ")==0)
    {
    	while(1)
        {
        	printf("%s",buffer);
            fgets(password,MAXBUFF,stdin);
            password[strlen(password)-1]='\0';
            send(sock,password,sizeof(password),0);
            recv(sock,buffer,sizeof(buffer),0);
            if(strcmp(buffer,"Logged in successfully.\n")==0)
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
    printf("1. Forwarding details.\n2. Activate.\n3. Deactivate.\n4. Unregister.\n0. Exit\n");
    printf("Enter the choice: ");
    scanf("%d",&option);
    getchar();
    send(sock,&option,sizeof(option),0);
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
				forward_call(sock);
            	break;
		case 0:
				printf("Exiting\n");
		default:
				printf("Entered invalid option.\n");
				break;
    }
}

void give_details(int sock)
{
	long int rphno;
    int type_choice,attempts =0;
    /*if(validate_phno(rphno,"Enter the requested number"))
    {
       send(sock,&rphno,sizeof(rphno),0);
    }
	else
	{
		printf("Maximum limit exceeded.\nTyr again later.\n");
		exit(EXIT_FAILURE);
	}*/
    printf("\nForwarding types\n");
    printf("1.Unconditional\n2.No reply\n3.Busy\n");
    while(1)
    {
		if(attempts<4)
		{
    		printf("\nEnter the choice: ");
        	scanf("%d",&type_choice);
        	if(type_choice<4 && type_choice>0)
        	{
        		send(sock,&type_choice,sizeof(type_choice),0);
            	break;
        	}
			else{
			attempts++;
        	printf("\nEnter between 1 to 3");
			break;}
		}
		else
		{
			printf("Maximum limit exceeded.\nTry again later.\n");
			exit(EXIT_FAILURE);
		}
    }
}

void forward_call(int sock)
{
	long int sphno;
    /*if(validate_phno(sphno,"Enter requested usernumber: "))
    {
		send(sock,&sphno,sizeof(sphno),0);
    }
	else
	{
		printf("Maximum limit exceeded.\nTry again later.\n");
		exit(EXIT_FAILURE);
	}*/
}
