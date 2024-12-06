/*
Program for tcp Client
socket()
connect()
send/recv/read/write/sento/recvfrom
close()
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#define PORTNO 4004
#define MAXBUFF 1024

typedef struct employee
{
	int id;
	int sal;
	int phno;
	char name[MAXBUFF];

}EMP;
int main()
{
	int sfd = 0, retValue=0;
	

	struct sockaddr_in serv_address;

	EMP e;

	sfd = socket(AF_INET,SOCK_STREAM,0);

	if(sfd < 0)
	{
		perror("socket() ");
		exit(EXIT_FAILURE);
	}
	printf("\nSocket created with sockfd : %d",sfd);

	//reset/set address of client 

	memset(&serv_address,'\0',sizeof(serv_address));

	// setting of Server side ipaddress and port no
	serv_address.sin_family = AF_INET;
	serv_address.sin_port = htons(PORTNO);
	serv_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	retValue = connect(sfd,(struct sockaddr *)&serv_address, sizeof(serv_address));

	if(retValue < 0)
	{
		perror("connect()");
		exit(EXIT_FAILURE);
	}

	printf("\nClient: Connected to the server\n");


	// strcpy(msg,"\nHi This is from Client\n");
		printf("Enter employee details: \n");
		printf("\nEnter id: ");
		scanf("%d",&e.id);
		printf("\nEnter name: ");
		scanf("%s",e.name);
		getchar();
		printf("\nEnter phno: ");
		scanf("%d",&e.phno);
		printf("\nEnter sal: ");
		scanf("%d",&e.sal);
		write(sfd,&e,sizeof(EMP));
		//memset(&e,'\0',sizeof(EMP));
		
	close(sfd);

	return 0;
}
