/*
Program for tcp server
socket()
bind()
listen()
accept()
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

#define MAXBUFF 1024
#define PORTNO 4004

typedef struct employee
{
	int id;
	int phno;
	int sal;
	char name[MAXBUFF];
}EMP;

int main()
{
	int sfd = 0, retValue=0, csfd=0;
	int clientAddlen = 0,cpid,flag=0;

	struct sockaddr_in serv_address, client_address;
	EMP e;

	sfd = socket(AF_INET,SOCK_STREAM,0);

	if(sfd < 0)
	{
		perror("socket() ");
		exit(EXIT_FAILURE);
	}
	printf("\nSocket created with sockfd : %d\n",sfd);



	//reset/set address of server 

	memset(&serv_address,'\0',sizeof(serv_address));

	serv_address.sin_family = AF_INET;
	serv_address.sin_port = htons(PORTNO);
	serv_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	retValue = bind(sfd, (struct sockaddr *)&serv_address,sizeof(serv_address));
	if(retValue < 0)
	{
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	printf("\nBinded the server to the ipaddress with portno\n");
	
	retValue = listen(sfd, 5);
	if(retValue < 0)
	{
		perror("listen() ");
		exit(EXIT_FAILURE);
	}
	printf("\nListening to the clients now\n");

		csfd = accept(sfd,(struct sockaddr *)&client_address,&clientAddlen);
		read(csfd,&e,sizeof(EMP));

		printf("\nClient sent Msg: \n\n");
		printf("\nid: %d",e.id);
		printf("\nname: %s",e.name);
		printf("\nphno: %d",e.phno);
		printf("\nsal: %d",e.sal);
		//memset(&e,'\0',sizeof(EMP));

	close(csfd);
	close(sfd);
	
	return 0;
}
