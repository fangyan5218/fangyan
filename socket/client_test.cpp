#include "common.h"
#include "log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/un.h>
#include <sys/epoll.h> 

int main()
{
	int sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		PRINT_LOG_ERROR("socket error");
		return -1;
	}

	struct sockaddr_un clit_adr;
	clit_adr.sun_family = AF_LOCAL;
	strcpy(clit_adr.sun_path, "/home/socket_test");
	if(connect(sockfd, (struct sockaddr*)&clit_adr, sizeof(clit_adr)) < 0)
	{
		PRINT_LOG_ERROR("connect error");
		return -1;
	}

	char sendline[10] = "hllo fyfs";
	if(send(sockfd, sendline, 10, 0) < 0)
	{
		PRINT_LOG_ERROR("send error");
		return -1;
	}

	return 0;
}
