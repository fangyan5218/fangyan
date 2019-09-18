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

enum ESocketStatus
{
	SOCKET_UNINIT,
	SOCKET_INITED,
	SOCKET_LISTEN,
	SOCKET_CONNECT,
};

static const int MAX_LISTEN = 10;
static const int MAX_EVENTS = 10;


class CSocket
{

	public:



		CSocket(string sFilePath);
		~CSocket();
		int SockInit();
		void Close(int fd);
		void SocketUninit();
		int SocketBind();

		int SocketListen();

		int EpollCreate();

		int EpollAddEvent();

		int EpollWait();

		int SetFdClosexec_(int fd, bool flag);

		int SetFdReuseaddr_(int fd);
		
	private:

 		
		int nListenFd_;		
		
		int nMaxEvent_;
		int nEventFd_;
		string sFilePath_;
		ESocketStatus eSockStatus_;
		struct sockaddr_un tServerAddr_;
};

