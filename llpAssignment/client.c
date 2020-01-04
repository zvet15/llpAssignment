#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "echo.h"
#include "list.h"

int main(int argc,char **argv)
{
	char sendline[MAX_STR], recvline[MAX_STR];
	struct sockaddr_in servaddr;
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd == -1)
	{
		fprintf(stderr, "socket() failed\n");
		return 1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	servaddr.sin_port = htons(LISTEN_PORT);

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		fprintf(stderr, "connect() failed\n");
		return 2;
	}

	
	while(1)
	{	
		memset(sendline, 0, MAX_STR);
		memset(recvline, 0, MAX_STR);
		printf("Enter Command\n");
		fgets(sendline, MAX_STR-1, stdin);
		if (write(sockfd, sendline, strlen(sendline)+1) < 0)
		{
			fprintf(stderr, "write() failed\n");
			return 3;
		}		
		
		if (read(sockfd, recvline, MAX_STR-1) < 0)
		{
			fprintf(stderr, "read() failed\n");
			return 4;
		}	
		printf("Recieved: %s\n",recvline);	
		
	}

	printf("Closing client socket...\n");		
	close(sockfd);
	return 0;
}

 
