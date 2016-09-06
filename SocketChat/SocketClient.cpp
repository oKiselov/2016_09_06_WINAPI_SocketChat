#include "SocketClient.h"


SocketClient::SocketClient()
{
}


SocketClient::~SocketClient()
{
}

bool SocketClient::ServerInit(std::string&ServerIP)
{
	bool bRet = TRUE; 
	this->ConnectSAddr.sin_addr.s_addr = inet_addr(ServerIP.c_str());
	if (INADDR_NONE == this->ConnectSAddr.sin_addr.s_addr)
	{
		struct hostent *hGetByName = ::gethostbyname(ServerIP.c_str());

		if (!hGetByName)
		{
			std::cout << "Error gethostbyname: " << ::WSAGetLastError()<<std::endl;
			SocketClose(2); 
			bRet=FALSE; 
		}
		this->ConnectSAddr.sin_addr = * reinterpret_cast<in_addr*> (hGetByName->h_addr_list[0]);
	}
	this->ConnectSAddr.sin_family = AF_INET;
	this->ConnectSAddr.sin_port = ::htons(PORT_NUM);
	
	return bRet; 
}


int SocketClient::ConnectToServer()
{
	int iConnect = connect(this->currSocket, reinterpret_cast<sockaddr*>(&this->ConnectSAddr), sizeof(this->ConnectSAddr)); 
	if (iConnect==SOCKET_ERROR)
	{
		std::cout << "Error connection: " << ::WSAGetLastError()<<std::endl;
		SocketClose(2); 
	}
	return iConnect; 
}

void SocketClient::CreateThreadInServerLoop()
{
	HANDLE hThread = CreateThread(NULL, NULL, InThreadWithServer, (LPVOID)this->currSocket, NULL,NULL);
	if (!hThread)
	{
		std::cout << "Error CreateThread = " << ::GetLastError()<<std::endl;
		SocketClose(2); 
		return;
	}

	int	size_send;
	this->strBuff.resize(1024, '\0'); 
	do
	{
		size_send = send(this->currSocket, this->strBuff.c_str(), this->strBuff.size(), 0); 
		if (size_send != SOCKET_ERROR && size_send == this->strBuff.size())
		{
			this->strBuff.resize(1024, '\0');
			std::cin.getline(&this->strBuff[0], sizeof(this->strBuff) - 1);
		}
	} while ((size_send != SOCKET_ERROR) || (this->strBuff =="exit"));
}


DWORD WINAPI InThreadWithServer(LPVOID currSocket)
{
	SOCKET SocketInThread = reinterpret_cast<SOCKET>(currSocket);

	int size_recv;
	do
	{
		SocketClient::strBuff.resize(1024, '\0');
		size_recv = ::recv(SocketInThread, &SocketClient::strBuff[0], SocketClient::strBuff.size(), 0); 
		if (size_recv && size_recv != SOCKET_ERROR)
			std::cout << SocketClient::strBuff << std::endl;
	} while (size_recv && size_recv != SOCKET_ERROR);

	return 0;
}

