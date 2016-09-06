#include "SocketBase.h"

SocketBase::SocketBase()
{
	this->strBuff.resize(1024, '\0'); 
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
		std::cout << "Error init: " << WSAGetLastError() << std::endl;
	}
	return initWSA; 
}

int SocketBase::SocketSend()
{
	int iSend = send(this->currSocket, this->strBuff.c_str(), this->strBuff.length(), 0); 
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "Error sending: " << WSAGetLastError() << std::endl;
	}
	return iSend; 
}

int SocketBase::SocketSend(SOCKET&currSocket)
{
	int iSend = send(currSocket, this->strBuff.c_str(), this->strBuff.length(), 0);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "Error sending: " << WSAGetLastError() << std::endl;
	}
	return iSend;
}


int SocketBase::SocketReceive()
{
	int iRecv = recv(this->currSocket, &this->strBuff[0], this->strBuff.length(), 0);
	if (iRecv == SOCKET_ERROR)
	{
		std::cout << "Error sending: " << WSAGetLastError() << std::endl;
	}
	return iRecv;
}

int SocketBase::SocketReceive(SOCKET&currSocket)
{
	int iRecv = recv(currSocket, &this->strBuff[0], this->strBuff.length(), 0);
	if (iRecv == SOCKET_ERROR)
	{
		std::cout << "Error sending: " << WSAGetLastError() << std::endl;
	}
	return iRecv;
}

void SocketBase::SocketClose(int paramClose)
{
	// разрыв соединения как для посылки (1) так и для получения (2) 
	shutdown(this->currSocket, paramClose);
	closesocket(this->currSocket);
	WSACleanup();
}
