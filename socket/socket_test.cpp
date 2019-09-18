#include "socket.h"

int main()
{
	
	CSocket* cServer = new CSocket("/home/socket_test");
	if(cServer == NULL)
	{
		PRINT_LOG_ERROR("new cServer error");
		return -1;
	}

	cServer->SockInit();
	cServer->SocketBind();
	cServer->SocketListen();
	cServer->EpollCreate();
	cServer->EpollAddEvent();

	while(true)
	{
		cServer->EpollWait();
	}
	
	return 0;
}
