#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h> /* strlen, memset */
#include "echo.h"   //port file
#include "list.h"   //linked list

/* Connect to this server using: telnet localhost 12345
   It will respond to one request and close connections */

void *echo_thread(void *socket)
{
	char str[MAX_STR];
	int *comm_fd = (int *)socket;
	FILE *fp;
	while (1)
	{
		memset(str, 0, MAX_STR);
		int num_bytes = read(*comm_fd, str, MAX_STR - 1);

		if (num_bytes == -1)
		{
			fprintf(stderr, "Error: Reading from client failed, killing thread...\n");
			return NULL;
		}
		else if (num_bytes == 0)
		{
			printf("Client disconnected normally, killing thread...\n");
			return NULL;
		}
		else
		{

			if (strncmp(str, "CLEAR", 5) == 0)
			{
				fp = fopen("myDoc.txt", "w");
				free_list();
				//printf("Clear\n");
				write(*comm_fd, "OK\n", strlen("OK\n"));
				fclose(fp);
			}

			else if (strncmp(str, "RANDOM", 6) == 0)
			{
				if (count_list_size() == 0)
				{
					//printf("Error\n");
					write(*comm_fd, "ERROR\n", strlen("ERROR\n"));
				}
				else
				{
					car *randCar = get_car_at_index(rand() % count_list_size());
					//printf("%s\n",randCar->name);
					strcpy(str, randCar->name);
					write(*comm_fd, str, strlen(str)); //the write always return a Char*
				}
			}

			else if (strncmp(str, "COUNT", 5) == 0)
			{
				int count = count_list_size();
				//printf("%d\n",count);
				sprintf(str, "%d\n", count);	   //convert to char
				write(*comm_fd, str, strlen(str)); //the write always return a Char*
			}

			else if (strncmp(str, "ADD:", 4) == 0)
			{

				char *ps = str + 4;
				fp = fopen("myDoc.txt", "a");
				//printf("%s\n",ps);
				if (search_by_name(ps) == NULL)
				{

					generate_car(ps);
					write(*comm_fd, "OK\n", strlen("OK\n"));
					fprintf(fp, "%s", ps);
					fclose(fp);
				}
				else
				{
					//printf("Error\n");
					write(*comm_fd, "ERROR\n", strlen("ERROR\n"));
				}
			}
			else
			{
				write(*comm_fd, "UNKNOWN\n", strlen("UNKNOWN\n"));
			}
			if (write(*comm_fd, str, strlen(str) + 1) < 0)
			{
				fprintf(stderr, "Error: failed to write to client, killing thread...\n");
				return NULL;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	char string[MAX_STR];
	FILE *fp = fopen("myDoc.txt", "r");

	if (fp != NULL)
	{
		//while (fscanf(fp, "%s", string) == 1)
		while (fgets(string ,MAX_STR,fp))		
		{
			printf("Read %s\n", string);

			generate_car(string);
		}

		fclose(fp);
	}

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t cllen = sizeof(cli_addr);
	int sockfd, thread_res;
	pthread_t mythread;

	/* Create a socket */
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 1;
	}

	/* Initialize socket structure (sockarrd_in) */
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //or INADDR_ANY
	serv_addr.sin_port = htons(LISTEN_PORT);

	//Allows re-use of the port as soon as the server terminates
	int enable = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

	/* Bind the host address */
	printf("New Connection\n");
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		fprintf(stderr, "ERROR: bind() failed\n");
		return 2;
	}

	/* Start listening for the clients (thread blocks) */
	if (listen(sockfd, 5) != 0)
	{
		fprintf(stderr, "ERROR: listen() failed\n");
		return 3;
	}

	while (1)
	{
		/* Accept connection from a client */
		int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cllen);
		if (newsockfd < 0)
		{
			fprintf(stderr, "ERROR: accept() failed\n");
			return 4;
		}

		thread_res = pthread_create(&mythread, NULL, echo_thread, &newsockfd);
		printf(" -> New Client\n");
		if (thread_res != 0)
		{
			fprintf(stderr, "ERROR: pthread_create() failed\n");
			return 5;
		}
	}

	return 0;
}