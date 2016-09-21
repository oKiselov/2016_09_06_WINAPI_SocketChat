#include "SocketServer.h"
using namespace SocketChat;

SocketServer::SocketServer() 
{
	for (int i=0; i < MAX_CLIENTS; i++)
	this->vecClients.push_back(NULL); 
	this->clientSocket = NULL; 
}

SocketServer::~SocketServer()
{
}

// Initialization of struct with info of current server 
void SocketServer::SocketStructInit()
{
	// initialization of data structure of servers socket 
	// тср
	this->ServerSAddr.sin_family = AF_INET;
	// for different OS
	// The htonl function converts a u_long from host to TCP/IP network byte order (which is big-endian). 
	this->ServerSAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian). 
	this->ServerSAddr.sin_port = htons(PORT_NUM);
	return; 
}

// Binding 
int SocketServer::SocketBind()
{
	int iBind = bind(this->currSocket, (sockaddr*)&this->ServerSAddr/*reinterpret_cast<sockaddr*>(&this->ServerSAddr)*/, sizeof(this->ServerSAddr)); 
	if (iBind == SOCKET_ERROR)
	{
	throw std::exception("Error bound: " );
	}
	std::cout << "Bound" << std::endl; 
	return iBind; 
}

// Starting of listening 
int SocketServer::SocketListen()const 
{
	int iListen = listen(this->currSocket, MAX_CLIENTS); 
	if (iListen == SOCKET_ERROR)
	{
		throw std::exception("Error with listening: " );
	}
	std::cout << "Listening..." << std::endl; 
	return iListen; 
}

// Creation of thread for work with accepted clients socket 
SOCKET& SocketServer::SocketAccept(/*sockaddr*clientAddr, int &clientAddrSize*/)
{
	CRITICAL_SECTION crit_section;
	InitializeCriticalSection(&crit_section);
	while (true)
	{
		SOCKET ClientSocket = NULL;
		sockaddr_in clientAddr = { 0 };
		int clientAddrSize = sizeof(clientAddr);
		
		this->clientSocket = accept(this->currSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
		if (this->clientSocket == INVALID_SOCKET)
		{
			throw std::exception("Error accept: " );
		}
		std::cout << "Accepted" << std::endl;

		if (clientCounter < MAX_CLIENTS)
		{
			EnterCriticalSection(&crit_section);

			for (int i = 0; i < this->vecClients.size(); i++)
			{
				if (this->vecClients[i] == NULL)
				{
					this->vecClients[i] = this->clientSocket;;

					HANDLE hThread = CreateThread(NULL, NULL, InThreadWithClients, reinterpret_cast<LPVOID>(i), NULL, NULL);
					if (!hThread)
					{
						throw std::exception("Error thread creation: ");
					}
					else
					{
						clientCounter++;
						CloseHandle(hThread);
					}
					break;
				}
			}
			LeaveCriticalSection(&crit_section);
		}
	}
	return this->clientSocket;
}

//thread for work with accepted clients socket 
DWORD WINAPI SocketChat::InThreadWithClients(LPVOID currClient)
{
	int index = reinterpret_cast<int>(currClient);
	CRITICAL_SECTION crit_section;

	while (SocketServer::vecClients[index] != SOCKET_ERROR)
	{
		/*****************/
		/*RECEIVE*/
		/*****************/

		std::string strBuff(1024,'\0'); 
		
		int SizeOfReceivedStr = recv(SocketServer::vecClients[index], &strBuff[0], strBuff.size(), 0);		

		if (SizeOfReceivedStr!= SOCKET_ERROR)
		{

			strBuff.erase(strBuff.find('\0'), strBuff.size()-1);

			std::cout << strBuff<< std::endl;

			InitializeCriticalSection(&crit_section);
			EnterCriticalSection(&crit_section);
			
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (SocketServer::vecClients[index] && SocketServer::vecClients[i] != SOCKET_ERROR && i != index)
				{
					/*****************/
					/*SEND*/
					/*****************/
					if (send(SocketServer::vecClients[i], strBuff.c_str(),
						strBuff.size(), 0) == SOCKET_ERROR)
					{
						closesocket(SocketServer::vecClients[i]);
						SocketServer::vecClients[i] = NULL;
						clientCounter--;
					}
				}
			}
			LeaveCriticalSection(&crit_section);
		}
	}
	EnterCriticalSection(&crit_section);
	closesocket(SocketServer::vecClients[index]);
	SocketServer::vecClients[index] = NULL;
	clientCounter--;
	LeaveCriticalSection(&crit_section);

	return 0;
}