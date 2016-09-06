#pragma once

#include "Settings.h"
#include "SocketBase.h"

class SocketClient : public SocketBase
{
private:
	sockaddr_in ConnectSAddr;
protected:

public:
	SocketClient();
	~SocketClient();

	bool ServerInit(std::string &); 

	int ConnectToServer(void); 

	void CreateThreadInServerLoop(void);
};

DWORD WINAPI InThreadWithServer(LPVOID);