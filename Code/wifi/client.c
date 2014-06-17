#include "client.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main()
{
	int sockfd, client_fd, sin_size;
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	int recvbytes;
	char buf[MAXDATASIZE];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("client.c : in main() - cann't create a socket fd\n");
		return 1;
	}
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(SERVPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		printf("client.c : in main() - cann't bind\n");
		return 1;
	}
	if(listen(sockfd, MAXLOG) == -1)
	{
		printf("client.c : in main() - cann't listen\n");
		return 1;
	}
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) == -1)
		{
			printf("client.c : in main() - accept error\n");
			continue;
		}
		printf("An robot has logged in!\n");

		/*if((recvbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
		{
			printf("Recive error\n");
			return;
		}
		buf[recvbytes] = '\0';
		printf("Received: %s",buf);*/
	}

	return 0;
}
