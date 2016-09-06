#pragma once
#include "SocketBase.h"

class SocketServer: public SocketBase
{
private: 
	SOCKET clientSocket; 
	sockaddr_in ServerSAddr;
	HANDLE hClientThread;

public:
	static std::vector<CONNECTED_CLIENT> vecClients;

	SocketServer();
	~SocketServer();

	void SocketStructInit(void); 

	int SocketBind(void); 

	int SocketListen(void) const; 

	SOCKET& SocketAccept(sockaddr*, int&); 

	void CreateThreadinLoopWithClients(void);
};

DWORD WINAPI InThreadWithClients(LPVOID);