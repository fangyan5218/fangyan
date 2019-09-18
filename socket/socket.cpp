#include "socket.h"

int SetFdClosexec(int fd, bool flag)
{
	int value = fcntl(fd, F_GETFD);
	if(flag)
	{
		value |= FD_CLOEXEC;
	}
	else
	{
		value &= (~FD_CLOEXEC);
	}

	if(fcntl(fd, F_SETFD, value) < 0)
	{
		PRINT_LOG_ERROR("set fd(%d) closexec error", fd);
		return -1;
	}

	return 0;
}

int SetFdReuseaddr(int fd)
{
	int optval = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		PRINT_LOG_ERROR("set fd(%d) reuseaddr error", fd);
		return -1;
	}

	return 0;
}


int recv_data(int fd)
{
	char test_read[1024] = {0};
	int num = read(fd, test_read, 10);
	if(num != 10)
	{
		PRINT_LOG_ERROR("read error");
		return -1;
	}

	PRINT_LOG_INFO("%s", test_read);

	return 0;
}

CSocket::CSocket(string sFilePath): nListenFd_(0),
									nMaxEvent_(0),
									nEventFd_(MAX_EVENTS),
									sFilePath_(sFilePath),
		 							eSockStatus_(SOCKET_UNINIT)		 							
{
	memset(&tServerAddr_, 0, sizeof(tServerAddr_));
}

CSocket::~CSocket()
{
	eSockStatus_ = SOCKET_UNINIT;
}

int CSocket::SockInit()
{
	nListenFd_ = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(nListenFd_ < 0)
	{
		PRINT_LOG_ERROR("socket init error");
		return -1;
	}

	tServerAddr_.sun_family = AF_LOCAL;
	strcpy(tServerAddr_.sun_path, sFilePath_.c_str());
	unlink(sFilePath_.c_str());
	SetFdClosexec_(nListenFd_, true);
	SetFdReuseaddr_(nListenFd_);
	eSockStatus_ = SOCKET_INITED;

	return 0;
}

void CSocket::SocketUninit()
{
	PRINT_LOG_INFO("socket uninit");
	Close(nListenFd_);
	unlink(sFilePath_.c_str());
	eSockStatus_ = SOCKET_UNINIT;

	return;
}

void CSocket::Close(int fd)
{
	close(fd);
}

int CSocket::SocketBind()
{
	if(bind(nListenFd_, (struct sockaddr*)(&tServerAddr_), sizeof(tServerAddr_)) < 0)
	{
		PRINT_LOG_ERROR("bind fd(%d) error", nListenFd_);
		SocketUninit();

		return -1;
	}
	
	return 0;
}

int CSocket::SocketListen()
{
	if(listen(nListenFd_, MAX_LISTEN) < 0)
	{
		PRINT_LOG_ERROR("listen fd(%d) error", nListenFd_);
		SocketUninit();
		return -1;
	}

	return 0;
}

int CSocket::EpollCreate()
{
	nEventFd_ = epoll_create(nMaxEvent_);
	if(nEventFd_ < 0)
	{
		PRINT_LOG_ERROR("epoll create error");
		return -1;
	}

	return 0;
}

int CSocket::EpollAddEvent()
{
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = nListenFd_;

	if(epoll_ctl(nEventFd_, EPOLL_CTL_ADD, nListenFd_, &ev) < 0)
	{
		PRINT_LOG_ERROR("epoll add event error");
		return -1;
	}

	return 0;
}

int CSocket::EpollWait()
{
	struct sockaddr_un client_addr;
	int client_len = sizeof(client_addr);
	struct epoll_event events[MAX_EVENTS];
	int nfds = 0;
	
	nfds = epoll_wait(nEventFd_, events, MAX_EVENTS, -1);
	for(int n = 0; n < nfds; n++)
	{
		if(events[n].data.fd == nListenFd_)
		{
			int clientfd = accept(nListenFd_, (struct sockaddr*)&client_addr, (socklen_t*)&client_len);
			if(clientfd < 0)
			{
				PRINT_LOG_ERROR("accept error");
				return -1;
			}

			struct epoll_event ev;
			ev.events = EPOLLIN;
			ev.data.fd = clientfd;
			if(epoll_ctl(nEventFd_, EPOLL_CTL_ADD, clientfd, &ev) < 0)
			{
				PRINT_LOG_ERROR("epoll add event error");
				return -1;
			}
		}
		else
		{
			//处理数据接收
			recv_data(events[n].data.fd);
		}
	}

	return 0;
}




