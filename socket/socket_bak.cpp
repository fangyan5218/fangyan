#include "socket.h"




int Read(int fd, char* pBuf, ssize_t length)
{
	if((NULL == pBuf)||(length <= 0))
	{
		PRINT_LOG_ERROR("input param error");
		return -1;
	}

	ssize_t rnum = 0;
	ssize_t rrnum = 0;
	int try_times = 10;
	while((rnum < length)&&(try_times-- > 0))
	{
		rrnum = read(fd, pBuf+rnum, length-rnum);
		if(rrnum == (length - rnum))
		{
			return length;
		}
		else if((rrnum > 0)&&(rnum < (length - rnum)))
		{
			rnum += rrnum;
		}
		else if(rrnum == 0) //对端关闭连接
		{
			return 0;
		}
		else 
		{
			if((EINTR == errno)||(EAGAIN == errno)||(EWOULDBLOCK == errno))
			{
				usleep(10*1000);
				rrnum = 0;
			}
			else
			{
				PRINT_LOG_ERROR("read error, %d, %s", errno, strerror(errno));
				return -1;
			}
		}
	}

	return rnum;
}

int Write(int fd, char* pBuf, ssize_t length)
{
	if((NULL == pBuf)||(length <= 0))
	{
		PRINT_LOG_ERROR("input param error");
		return -1;
	}

	ssize_t rnum = write(fd, pBuf, length);
	if(rnum < 0)
	{
		PRINT_LOG_ERROR("write error, %d, %s", errno, strerror(errno));
		return -1;
	}

	return rnum;
}

CServerSocket::CServerSocket(string sUnixFile): nListenfd_(-1), 
												sUnixFile_(sUnixFile),
												eSockStatus_(SOCKET_UNINIT){}

CServerSocket::~CServerSocket(){}

int CServerSocket::Init()
{
	if(SOCKET_INITED == eSockStatus_)
	{
		PRINT_LOG_INFO("socket has been inited");
		return 0;
	}
	
	struct sockaddr_un tSerAddr;
	tSerAddr.sun_family = AF_LOCAL;
	strcpy(tSerAddr.sun_path, sUnixFile_.c_str());
	unlink(sUnixFile_.c_str());

	//套接字命名
	nListenfd_ = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(nListenfd_ < 0)
	{
		PRINT_LOG_ERROR("socket init error, %s", strerror(errno));
		return -1;
	}
	
	if(SetFdClosexec(nListenfd_, true) < 0)
	{
		Uninit();
		return -1;
	}

	if(SetFdReuseaddr(nListenfd_) < 0)
	{
		Uninit();
		return -1;
	}
	
	//套接字绑定
	if(bind(nListenfd_, (struct sockaddr*)(&tSerAddr), sizeof(tSerAddr)) < 0)
	{
		PRINT_LOG_ERROR("bind fd error, %s", strerror(errno));
		Uninit();
		return -1;
	}

	eSockStatus_ = SOCKET_INITED;
	return 0;
}

int CServerSocket::Listen()
{
	if(listen(nListenfd_, MAX_LISTEN) < 0)
	{
		PRINT_LOG_ERROR("listen fd error, %s", strerror(errno));
		Uninit();
		return -1;
	}

	eSockStatus_ = SOCKET_LISTEN;
	return 0;
}

int CServerSocket::Accept()
{
	struct sockaddr_un tClientAddr;
	int nClientLen = sizeof(tClientAddr);
	int nClientfd = accept(nListenfd_, (struct sockaddr*)&tClientAddr, (socklen_t*)&nClientLen);
	if(nClientfd < 0)
	{
		PRINT_LOG_ERROR("accept error, %s", strerror(errno));
		return -1;
	}

	return nClientfd;
}

int CServerSocket::Uninit()
{
	PRINT_LOG_INFO("socket uninit");
	close(nListenfd_);
	unlink(sUnixFile_.c_str());
	eSockStatus_ = SOCKET_UNINIT;

	return 0;
}

CClientSocket::CClientSocket(string sUnixFile): nConectfd_(-1), 
												sUnixFile_(sUnixFile),
												eSockStatus_(SOCKET_UNINIT){}

CClientSocket::~CClientSocket(){}

int CClientSocket::Init()
{
	nConectfd_ = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(nConectfd_ < 0)
	{
		PRINT_LOG_ERROR("socket error, %s", strerror(errno));
		return -1;
	}

	eSockStatus_ = SOCKET_INITED;
	return 0;
}

int CClientSocket::Conect()
{
	struct sockaddr_un clit_adr;
	clit_adr.sun_family = AF_LOCAL;
	strcpy(clit_adr.sun_path, sUnixFile_.c_str());
	if(connect(nConectfd_, (struct sockaddr*)&clit_adr, sizeof(clit_adr)) < 0)
	{
		PRINT_LOG_ERROR("connect error, %s", strerror(errno));
		return -1;
	}

	eSockStatus_ = SOCKET_CONNECT;
	return 0;
}

int CClientSocket::Uninit()
{
	PRINT_LOG_INFO("socket uninit");
	close(nConectfd_);
	eSockStatus_ = SOCKET_UNINIT;

	return 0;
}

CEpoll::CEpoll(const int nMaxEvents):nMaxEvents_(nMaxEvents){}

CEpoll::~CEpoll(){}

int CEpoll::Create()
{
	nEpollfd_ = epoll_create(nMaxEvents_);
	if(nEpollfd_ < 0)
	{
		PRINT_LOG_ERROR("epoll create error, %s", strerror(errno));
		return -1;
	}

	return 0;
}

int CEpoll::AddEvent(int fd, struct epoll_event* pEvent)
{
	if(NULL == pEvent)
	{
		PRINT_LOG_ERROR("input param error");
		return -1;
	}
	
	if(epoll_ctl(nEpollfd_, EPOLL_CTL_ADD, fd, pEvent) < 0)
	{
		PRINT_LOG_ERROR("epoll add event error, %s", strerror(errno));
		return -1;
	}

	return 0;
}

int CEpoll::DelEvent(int fd, struct epoll_event* pEvent)
{
	if(NULL == pEvent)
	{
		PRINT_LOG_ERROR("input param error");
		return -1;
	}
	
	if(epoll_ctl(nEpollfd_, EPOLL_CTL_DEL, fd, pEvent) < 0)
	{
		PRINT_LOG_ERROR("epoll del event error, %s", strerror(errno));
		return -1;
	}

	return 0;
}

int CEpoll::Wait(struct epoll_event* pEvents)
{
	if(NULL == pEvents)
	{
		PRINT_LOG_ERROR("input param error");
		return -1;
	}

	int nfds = epoll_wait(nEpollfd_, pEvents, nMaxEvents_, -1);
	if(nfds < 0)
	{
		PRINT_LOG_ERROR("epoll wait error, %s", strerror(errno));
		return -1;
	}

	return nfds;
}

static int SetFdClosexec(int fd, bool flag)
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

static int SetFdReuseaddr(int fd)
{
	int optval = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		PRINT_LOG_ERROR("set fd(%d) reuseaddr error", fd);
		return -1;
	}

	return 0;
}


