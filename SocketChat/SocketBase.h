#pragma once
#include "Settings.h"

class SocketBase
{
protected:
	WSAData WSStartData; 
	SOCKET currSocket; 

public:
	static std::string strBuff;

	SocketBase();
	virtual ~SocketBase();

	int SocketInit(void);

	int SocketSend(void);

	int SocketSend(SOCKET&);

	int SocketReceive(void);

	int SocketReceive(SOCKET&);

	void SocketClose(int); 
};

