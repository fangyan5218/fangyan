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

//Í¨ÓÃº¯Êý
int SetFdClosexec(int fd, bool flag);
int SetFdReuseaddr(int fd);

enum ESocketStatus
{
	SOCKET_UNINIT,
	SOCKET_INITED,
};

class CServerSocket
{
	public:
		static const int MAX_LISTEN;

		CServerSocket(string sUnixfile);
		~CServerSocket();
		int Init();
		int Listen();
		int Uninit();
		
	private:
		int nListenfd_;		
		string sUnixFile_;
		ESocketStatus eSockStatus_;
};

class CClientSocket
{
	public:

		CClientSocket(string sUnixfile);
		~CClientSocket();
		int Init();
		int Conect();
		int Uninit();
		
	private:
		int nConectfd_;		
		string sUnixFile_;
		ESocketStatus eSockStatus_;
};

class CEpoll
{
	public:
		int Create();

		int AddEvent(int fd, struct epoll_event* pEvent);

		int Wait(struct epoll_event* pEvents);

	private:
		int nEpollfd_;
		const int nMaxEvents_;
};
