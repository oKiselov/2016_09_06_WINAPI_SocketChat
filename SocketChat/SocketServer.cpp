#include "SocketServer.h"


SocketServer::SocketServer() 
{
}


SocketServer::~SocketServer()
{
}

void SocketServer::SocketStructInit()
{
	// инициализаци€ структуры данных о сокете
	// тср
	this->ServerSAddr.sin_family = AF_INET;
	// дл€ реагировани€ на запросы любых систем, примен€€ функцию 
	// The htonl function converts a u_long from host to TCP/IP network byte order (which is big-endian). 
	this->ServerSAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian). 
	this->ServerSAddr.sin_port = htons(PORT_NUM);
	return; 
}


int SocketServer::SocketBind()
{
	int iBind = bind(this->currSocket, reinterpret_cast<sockaddr*>(&this->ServerSAddr), sizeof(this->ServerSAddr)); 
	if (iBind == SOCKET_ERROR)
	{
		std::cout << "Error bind: " << WSAGetLastError() << std::endl;
	}
	return iBind; 
}

int SocketServer::SocketListen()const 
{
	int iListen = listen(this->currSocket, MAX_CLIENTS); 
	if (iListen == SOCKET_ERROR)
	{
		std::cout << "Error listen: " << WSAGetLastError() << std::endl;
	}
	return iListen; 
}

SOCKET& SocketServer::SocketAccept(sockaddr*clientAddr, int &clientAddrSize)
{
	this->clientSocket = accept(this->currSocket, clientAddr, &clientAddrSize);
	if (this->clientSocket == INVALID_SOCKET)
	{
		{
			std::cout << "Error accept: " << WSAGetLastError() << std::endl;
		}
	}
	return this->clientSocket;
}

void SocketServer::CreateThreadinLoopWithClients()
{
	if (clientCounter < MAX_CLIENTS)
	{
		EnterCriticalSection(&crit_section);
		for (int i = 0; i < MAX_CLIENTS; i++)
		{
			HANDLE hThread = CreateThread(NULL, NULL, InThreadWithClients, reinterpret_cast<LPVOID>(i), NULL, NULL);
			if (!hThread)
			{
				std::cout << "Error CreateThread = " << ::GetLastError() << std::endl;
			}	
			else
			{
				clientCounter++;
				CloseHandle(hThread);
			}
			break;
		}
	}
	LeaveCriticalSection(&::crit_section);
}

DWORD WINAPI InThreadWithClients(LPVOID currClient)
{
	int index = reinterpret_cast<int>(currClient);

	while (SocketServer::vecClients[index].clientSock != SOCKET_ERROR)
	{
		if (recv(SocketServer::vecClients[index].clientSock, &SocketServer::strBuff[0], 
			SocketServer::strBuff.size(), 0) != SOCKET_ERROR)
		{
			std::cout << SocketServer::vecClients[index].login << " has just entered this chat." << std::endl;

			EnterCriticalSection(&crit_section);
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (SocketServer::vecClients[index].clientSock && SocketServer::vecClients[i].clientSock != SOCKET_ERROR && i != index)
				{
					if (send(SocketServer::vecClients[i].clientSock, SocketServer::strBuff.c_str(), 
						SocketServer::strBuff.size(), 0) == SOCKET_ERROR);
					{
						closesocket(SocketServer::vecClients[i].clientSock);
						SocketServer::vecClients[i].login = '\0';
						clientCounter--;
					}
				}
			}
			LeaveCriticalSection(&crit_section);
		}
	}

	EnterCriticalSection(&crit_section);
	closesocket(SocketServer::vecClients[index].clientSock);
	SocketServer::vecClients[index].login = '\0';
	clientCounter--;
	LeaveCriticalSection(&crit_section);

	return 0;
}
