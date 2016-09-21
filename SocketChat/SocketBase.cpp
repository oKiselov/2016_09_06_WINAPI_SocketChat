#include "SocketBase.h"
using namespace SocketChat;


SocketBase::SocketBase() :currSocket(NULL)
{
}

SocketBase::~SocketBase()
{
	if (this->currSocket)
		SocketClose(2); 
}

int SocketBase::SocketInit()
{
	int initWSA = WSAStartup(MAKEWORD(1, 0), &WSStartData);
	if (initWSA != 0)
	{
		throw std::exception("Error initialization of socket lib: ");
	}
	return initWSA; 
}

int SocketBase::SocketStart(int typeConnect)
{
	// Server socket 
	if (typeConnect == 1)
	{
		this->currSocket = socket(AF_INET, SOCK_STREAM, 0);
	}
	
	// Client socket 
	else if (typeConnect == 2)
	{
		this->currSocket = socket(AF_INET, SOCK_STREAM,	IPPROTO_IP);
	}

	if (this->currSocket == SOCKET_ERROR)
	{
		throw std::exception("Error socket creation:");
	}
	return this->currSocket;
}

void SocketBase::SocketClose(int paramClose)const 
{
	// breach of connection for sending (1) and receiving (2) 
	shutdown(this->currSocket, paramClose);
	closesocket(this->currSocket);
	WSACleanup();
}
