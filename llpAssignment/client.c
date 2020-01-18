#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include "echo.h"
#include "list.h"
#include <time.h>

int main(int argc, char **argv)
{
	while (1)
	{
	char sendline[MAX_STR], recvline[MAX_STR];
	struct sockaddr_in servaddr;
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//time
	time_t current_time;
	time(&current_time);

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

	

		memset(sendline, 0, MAX_STR);
		memset(recvline, 0, MAX_STR);

		printf("Enter Command\n");

		fgets(sendline, MAX_STR - 1, stdin);

		if (write(sockfd, sendline, strlen(sendline) + 1) < 0)
		{
			fprintf(stderr, "write() failed\n");
			return 3;
		}

#ifdef LOG

		FILE *fp = fopen("logfile.log", "a");
		if (fp == NULL)
		{
			printf("Failed to open file logfile");
		}

		fprintf(fp, "%s-REQUEST:%s\n", ctime(&current_time), sendline);

#endif

		if (read(sockfd, recvline, MAX_STR - 1) < 0)
		{
			fprintf(stderr, "read() failed\n");
			return 4;
		}
		printf("Recieved: %s\n", recvline);

#ifdef LOG
		fprintf(fp, "%s-RESPONSE:%s\n", ctime(&current_time), recvline);
		fclose(fp);
#endif
	
	
	close(sockfd);
	}

	// printf("Closing client socket...\n");
	return 0;
}
